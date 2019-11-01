//
// Created by CONG YU on 2019/10/24.
//
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <list>
#include <fstream>
#include "Scheduler.h"
#include "Event.h"

using namespace std;

bool IS_VERBOSE = false;
SchedulerEnum SCHED_SPEC;
int QUANTUM = 10000;
int MAX_PRIORITY = 4;
string INPUT_FILE;
string RAND_FILE;

vector<int> RANDVALS;
int RANDVALS_SIZE;
int _obf = 0;

int MyRandom(int burst) {
    return 1+ RANDVALS[(_obf++) % RANDVALS_SIZE] % burst;
}

void ReadArgs(int argc, char **argv) {
    int opt;
    char* sched_opt = nullptr;
    while ((opt = getopt(argc, argv, "vs:")) != -1) {
        switch (opt) {
            case 'v':
                IS_VERBOSE = true;
                break;
            case 's':
                sched_opt = optarg;
                break;
            case '?':
                cout<<"unknown option";
                break;
            default:
                break;
        }
    }

    /// no extra error is parsed, only valid arguments is consider for simplicity
    char* read_ptr = sched_opt;
    // process scheduler type
    if (read_ptr != nullptr && *read_ptr != '\0') {
        switch (read_ptr[0]) {
            case 'F':
                SCHED_SPEC = SchedulerEnum::FCFS; break;
            case 'L':
                SCHED_SPEC = SchedulerEnum::LCFS; break;
            case 'S':
                SCHED_SPEC = SchedulerEnum::SRTF; break;
            case 'R':
                SCHED_SPEC = SchedulerEnum::RR; break;
            case 'P':
                SCHED_SPEC = SchedulerEnum::PRIO; break;
            case 'E':
                SCHED_SPEC = SchedulerEnum::PREPRIO; break;
            default: break;
        }
        read_ptr++;
        // read priority number if there is any
        if (*read_ptr != '\0') {
            string sched_rest_opt(read_ptr);
            int colon_loc = sched_rest_opt.rfind(':');
            QUANTUM = stoi(sched_rest_opt.substr(0, colon_loc));
            if (colon_loc != string::npos) {
                MAX_PRIORITY = stoi(sched_rest_opt.substr(colon_loc + 1, -1));
            }
        }
    }

    if (optind+1 < argc) {
        INPUT_FILE = string(argv[optind]);
        RAND_FILE = string(argv[optind+1]);
    }
}

void AddEvent(list<Event*>& event_list, Event* event) {
    if (event_list.empty()) {
        event_list.push_back(event);
        return;
    }
    auto it = event_list.begin();
    while (it != event_list.end()) {
        if ((*it)->event_timestamp > event->event_timestamp) {
            break;
        }
//        else if ((*it)->event_timestamp == event->event_timestamp) {
//            if ((*it)->event_process->dynamic_priority < event->event_process->dynamic_priority) {
//                break;
//            }
//            else if ((*it)->event_process->dynamic_priority == event->event_process->dynamic_priority
//                     && (*it)->event_process->pid > event->event_process->pid) {
//                break;
//            }
//        }
        it++;
    }
    event_list.insert(it, event);
}

void PrintEvent(Event* event) {
    cout << event->event_timestamp << " ";
    cout << event->event_process->pid << " ";
    cout << event->previous_time_log << ": ";
    if (event->transition_to == StateEnum::DONE) {
        cout << "DONE";
    } else {
        cout << StateEnumToString(event->transition_from) << " -> " << StateEnumToString(event->transition_to) << " ";
        if (event->transition_to == StateEnum::BLOCK) {
            cout << "ib=" << event->burst_log << " rem=" << event->rem_log;
//            cout << " prio=" << event->priority_log;
        } else if (event->transition_to == StateEnum::RUN) {
            cout << "cb=" << event->burst_log << " rem=" << event->rem_log;
            cout << " prio=" << event->priority_log;
        }
    }
    cout << endl;
}

void Simulation(list<Event*>& event_list, Scheduler& scheduler, int* total_io_time) {
    Event* cur_event;
    int cur_time;
    int time_in_previous_state;
    bool call_scheduler = false;
    Process* cur_running_process = nullptr;
    Process* cur_blocked_process = nullptr;
    int last_io_timestamp = 0;

    while (!event_list.empty()) {
        cur_event = event_list.front();
        event_list.pop_front();
        Process* process = cur_event->event_process;
        cur_time = cur_event->event_timestamp;
        time_in_previous_state = cur_time - process->state_timestamp;
        cur_event->previous_time_log = time_in_previous_state;

        switch(cur_event->transition_to) {
            case StateEnum::READY: {
                if (cur_blocked_process == process) {
                    // current lasted blocked process is unblock
                    cur_blocked_process = nullptr;
                }
                process->state_timestamp = cur_time;
                scheduler.AddProcess(process);
                call_scheduler = true;
                break;
            }
            case StateEnum::RUN: {
                process->cpu_waiting_time += cur_time - process->state_timestamp;
                process->state_timestamp = cur_time;
                cur_running_process = process;
                int burst = 0;

                if (process->remain_cpu_burst > 0) {
                    // continue the cpu burst that was preempted
                    burst = process->remain_cpu_burst;
                } else {
                    // roll the dice and choose a new burst
                    burst = MyRandom(process->cpu_burst);
                }

                cur_event->rem_log = process->remain_cpu_time;
                cur_event->priority_log = process->dynamic_priority;

                if (burst > QUANTUM) {
                    // take the preempt
                    // TODO
                } else if (process->remain_cpu_time-burst > 0) {
                    // just normal run, need to new a block event
                    process->remain_cpu_time -= burst;
                    process->remain_cpu_burst = 0;
                    auto* new_event = new Event(process, cur_time+burst, StateEnum::RUN, StateEnum::BLOCK);
                    AddEvent(event_list, new_event);
                } else if (process->remain_cpu_time-burst <= 0) {
                    // exceed the total time, so DONE!
                    burst = process->remain_cpu_time;
                    process->remain_cpu_time = 0;
                    process->remain_cpu_burst = 0;
                    auto* new_event = new Event(process, cur_time+burst, StateEnum::RUN, StateEnum::DONE);
                    AddEvent(event_list, new_event);
                }

                // log
                cur_event->burst_log = burst;
                break;
            }
            case StateEnum::BLOCK: {
                process->state_timestamp = cur_time;
                cur_running_process = nullptr;
                int burst = MyRandom(process->io_burst);

                // log
                cur_event->burst_log = burst;
                cur_event->rem_log = process->remain_cpu_time;
                cur_event->priority_log = process->dynamic_priority;

                auto* new_event = new Event(process, cur_time + burst, StateEnum::BLOCK, StateEnum::READY);
                process->io_time_total += burst;
                AddEvent(event_list, new_event);
                call_scheduler = true;

                // update total_io_time
                if (cur_blocked_process == nullptr) {
                    *total_io_time += burst;
                    last_io_timestamp = cur_time + burst;
                    cur_blocked_process = process;
                } else if (cur_time + burst > last_io_timestamp) {
                        *total_io_time += cur_time + burst - last_io_timestamp;
                        last_io_timestamp = cur_time + burst;
                        cur_blocked_process = process;
                }
                break;
            }
            case StateEnum::PREEMPT: {
                process->state_timestamp = cur_time;
                call_scheduler = true;
                break;
            }
            case StateEnum::DONE:
                process->state_timestamp = cur_time;
                process->finish_time = cur_time;
                cur_running_process = nullptr;
                call_scheduler = true;
                break;
            default:
                break;
        }

        if (IS_VERBOSE) PrintEvent(cur_event);
        delete cur_event;

        if (call_scheduler) {
            if (!event_list.empty() && event_list.front()->event_timestamp == cur_time) {
                continue;
            }
            call_scheduler = false;
            if (cur_running_process == nullptr) {
                cur_running_process = scheduler.GetNextProcess();
                if (cur_running_process == nullptr) {
                    // means that no process is in the run_queue
                    continue;
                }
                // create event
//                cur_running_process->state_timestamp = cur_time;
                auto* new_event = new Event(cur_running_process, cur_time, StateEnum::READY, StateEnum::RUN);
                AddEvent(event_list, new_event);
            }
        }
    }

}

int main(int argc, char **argv) {

    ReadArgs(argc, argv);

    vector<Process*> process_pool;
    list<Event*> event_list;

    // read processes
    ifstream ifs_input_file(INPUT_FILE);
    Process* process_ptr = nullptr;
    int arrival_time, total_time, cpu_burst, io_burst;
    int pid = 0;
    while (ifs_input_file >> arrival_time) {
        ifs_input_file >> total_time >> cpu_burst >> io_burst;
        process_ptr = new Process(pid++, arrival_time, total_time, cpu_burst, io_burst);
        process_pool.push_back(process_ptr);
    }
    ifs_input_file.close();

    // read random file
    ifstream ifs_rand_file(RAND_FILE);
    ifs_rand_file >> RANDVALS_SIZE;
    long next_int;
    int count = 0;
    while (ifs_rand_file >> next_int && count < RANDVALS_SIZE) {
        RANDVALS.push_back(next_int);
        count++;
    }
    ifs_rand_file.close();

    // assign static priority and dynamic priority
    for (auto p : process_pool) {
        p->static_priority = MyRandom(MAX_PRIORITY);
        p->dynamic_priority = p->static_priority - 1;
    }

    // we cannot assure the process is listed in ascending order of arrival time, so just sort it!
//    std::stable_sort(process_pool.begin(), process_pool.end(),
//                     [](Process* a, Process* b) {return a->arrival_time < b->arrival_time; });
    /// according to the reference program, if it is not increasing, then raise error

    Event* cur_event;
    for (auto p : process_pool) {
        cur_event = new Event(p, p->arrival_time, StateEnum::CREATE, StateEnum::READY);
        event_list.push_back(cur_event);
        p->state_timestamp = p->arrival_time;
    }

    Scheduler* scheduler = SchedulerFactory::CreateScheduler(SCHED_SPEC);
    scheduler->setQuantum(QUANTUM);
    scheduler->setMaxPriority(MAX_PRIORITY);

    int total_io_time = 0;
    Simulation(event_list, *scheduler, &total_io_time);

    // print out summary
    cout << SchedulerEnumToString(SCHED_SPEC);
    if (SCHED_SPEC == SchedulerEnum::RR || SCHED_SPEC == SchedulerEnum::PRIO || SCHED_SPEC == SchedulerEnum::PREPRIO) {
        cout << " " << QUANTUM;
    }
    cout << endl;
    cout << flush;

    // not using cout because format constrain, in case some annoying bugs
    int total_cpu_time = 0;
    int total_process_turnaround_time = 0;
    int total_cpu_waiting_time = 0;
    int last_finish_time = 0;
    for (auto p : process_pool) {
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
                p->pid, p->arrival_time, p->total_cpu_time, p->cpu_burst, p->io_burst, p->static_priority,
                p->finish_time, (p->finish_time-p->arrival_time), p->io_time_total, p->cpu_waiting_time);
        total_cpu_time += p->total_cpu_time;
        total_process_turnaround_time += p->finish_time - p->arrival_time;
        total_cpu_waiting_time += p->cpu_waiting_time;
        last_finish_time = max(last_finish_time, p->finish_time);
    }

    double cpu_util = total_cpu_time/(double)last_finish_time;
    double io_util = total_io_time/(double)last_finish_time;
    double time_units = (double) last_finish_time / 100;

    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", last_finish_time, 100*cpu_util, 100*io_util,
            total_process_turnaround_time/(double)process_pool.size(),
            total_cpu_waiting_time/(double)process_pool.size(),
           process_pool.size() / time_units);
}