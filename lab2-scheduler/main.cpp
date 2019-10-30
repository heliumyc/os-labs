//
// Created by CONG YU on 2019/10/24.
//
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <fstream>
#include "Scheduler.h"
#include "Event.h"

using namespace std;

bool IS_VERBOSE = false;
SchedulerEnum SCHED_SPEC;
int QUANTUM;
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

void Simulation(queue<Event*>& event_queue, Scheduler& scheduler) {
    Event* cur_event;

    cur_event = event_queue.front();
    event_queue.pop();

    switch(cur_event->transition_to) {
        case StateEnum::READY:
            break;
        case StateEnum::RUN:
            break;
        case StateEnum::BLOCK:
            break;
        case StateEnum::PREEMPT:
            break;
        default:
            break;
    }

}

int main(int argc, char **argv) {

    ReadArgs(argc, argv);

    vector<Process*> process_pool;
    queue<Event*> event_queue;

    // read processes
    ifstream ifs_input_file(INPUT_FILE);
    Process* process_ptr = nullptr;
    int arriveT, totalT, cpuBurst, ioBurst;
    while (ifs_input_file >> arriveT) {
        ifs_input_file >> totalT >> cpuBurst >> ioBurst;
        process_ptr = new Process(arriveT, totalT, cpuBurst, ioBurst);
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

    // we cannot assure the process is listed in ascending order of arrival time
    std::stable_sort(process_pool.begin(), process_pool.end(),
                     [](Process* a, Process* b) {return a->arrival_time < b->arrival_time; });

    Event* cur_event;
    for (auto p : process_pool) {
        cur_event = new Event(p, p->arrival_time, StateEnum::CREATE, StateEnum::READY);
        event_queue.push(cur_event);
    }

    Scheduler* scheduler = SchedulerFactory::CreateScheduler(SCHED_SPEC);
    scheduler->setQuantum(QUANTUM);
    scheduler->setMaxPriority(MAX_PRIORITY);

    Simulation(event_queue, *scheduler);

}