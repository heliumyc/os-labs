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
    for (auto it = event_list.begin(); it != event_list.end(); ++it) {
        if ((*it)->event_timestamp > event->event_timestamp) {
            event_list.insert(it, event);
            break;
        }
        else if ((*it)->event_timestamp == event->event_timestamp) {
            if ((*it)->event_process->dynamic_priority < event->event_process->dynamic_priority) {
                event_list.insert(it, event);
                break;
            }
            else if ((*it)->event_process->dynamic_priority == event->event_process->dynamic_priority
                     && (*it)->event_process->pid > event->event_process->pid) {
                event_list.insert(it, event);
                break;
            }
        }
    }
}

void PrintEvent(Event* event) {
    cout << event->event_timestamp << " ";
    cout << event->event_process->pid << " ";
    cout << event->event_timestamp-event->event_process->state_timestamp << ": ";
    if (event->transition_to == StateEnum::DONE) {
        cout << "DONE";
    } else {
        cout << StateEnumToString(event->transition_from) << " -> " << StateEnumToString(event->transition_to) << " ";
        if (event->transition_to == StateEnum::BLOCK) {
            cout << "ib=" << event->event_process->remain_io_burst << " rem=" << event->event_process->remain_cpu_time;
            cout << " prio=" << event->event_process->dynamic_priority;
        } else if (event->transition_to == StateEnum::RUN) {
            cout << "cb=" << event->burst_log << " rem=" << event->event_process->remain_cpu_time;
            cout << " prio=" << event->event_process->dynamic_priority;
        }
    }
    cout << endl;
}

void Simulation(list<Event*>& event_list, Scheduler& scheduler) {
    Event* cur_event;
    int cur_time;
    int time_in_previous_state;
    bool call_scheduler = false;
    Process* cur_running_process = nullptr;

    while (!event_list.empty()) {
        cur_event = event_list.front();
        event_list.pop_front();
        Process* process = cur_event->event_process;
        cur_time = cur_event->event_timestamp;
        time_in_previous_state = cur_time - process->state_timestamp;

        switch(cur_event->transition_to) {
            case StateEnum::READY:
                scheduler.AddProcess(process);
                call_scheduler = true;
                break;
            case StateEnum::RUN: {
                int burst = 0;

                if (process->remain_cpu_burst > 0) {
                    // continue the cpu burst that was preempted
                    burst = process->remain_cpu_burst;
                } else {
                    // roll the dice and choose a new burst
                    burst = MyRandom(process->cpu_burst);
                }

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
                    process->remain_cpu_time -= burst;
                    process->remain_cpu_burst = 0;
                    auto* new_event = new Event(process, cur_time+burst, StateEnum::RUN, StateEnum::DONE);
                    AddEvent(event_list, new_event);
                }
                break;
            }
            case StateEnum::BLOCK: {
                cur_running_process = nullptr;
                int io_time = MyRandom(process->io_burst);
                auto* new_event = new Event(process, cur_time+io_time, StateEnum::BLOCK, StateEnum::READY);
                process->io_time_total += io_time;
                AddEvent(event_list, new_event);
                call_scheduler = true;
                break;
            }
            case StateEnum::PREEMPT:
                call_scheduler = true;
                break;
            case StateEnum::DONE:
                process->finish_time = cur_time;
                break;
            default:
                break;
        }

//        PrintEvent(cur_event);
        delete cur_event;

        if (call_scheduler) {
            if (event_list.front()->event_timestamp == cur_time) {
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

    Simulation(event_list, *scheduler);
}