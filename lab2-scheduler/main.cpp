//
// Created by CONG YU on 2019/10/24.
//
#include <cstdio>
#include <iostream>

#include <cstdio>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <fstream>
#include "Scheduler.h"
#include "Event.h"

using namespace std;

bool IS_VERBOSE = false;
SchedulerEnum SCHED_SPEC;
int QUANTUM_NUM;
int PRIOS_NUM = 4;
string INPUT_FILE;
string RAND_FILE;

vector<long> RANDVALS;
int RANDVALS_SIZE;
int OBF = 0;

vector<Process*> PROCESS_POOL;
queue<Event*> eventQueue;

int MyRandom(int burst) {
    return 1+RANDVALS[(OBF++)%RANDVALS_SIZE]%burst;
}

void Simulation() {
    // init
    Event* curEvt;
    for (auto p : PROCESS_POOL) {
        curEvt = new Event(p, p->arrival_time, StateEnum::CREATE, StateEnum::READY);
        eventQueue.push(curEvt);
    }
//    queue<Event*> q(eventQueue);
    curEvt = eventQueue.front();
    eventQueue.pop();

    switch(curEvt->transitionTo) {
        case StateEnum ::READY:
            break;
        case StateEnum ::RUN:
            break;
        case StateEnum ::BLOCK:
            break;
        case StateEnum ::PREEMPT:
            break;
    }

}

int main(int argc, char **argv) {
    int opt;
    char* schedOpt = nullptr;
    while ((opt = getopt(argc, argv, "vs:")) != -1) {
        switch (opt) {
            case 'v':
                IS_VERBOSE = true;
                break;
            case 's':
                schedOpt = optarg;
                break;
            case '?':
                cout<<"unknown option";
                break;
            default:
                break;
        }
    }

    /// no extra error is parsed, only valid arguments is consider for simplicity
    char* readPtr = schedOpt;
    // process scheduler type
    if (readPtr != nullptr && *readPtr != '\0') {
        switch (readPtr[0]) {
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
        readPtr++;
        // read priority number if there is any
        if (*readPtr != '\0') {
            string schedRestOpt(readPtr);
            int colonLoc = schedRestOpt.rfind(':');
            QUANTUM_NUM = stoi(schedRestOpt.substr(0, colonLoc));
            if (colonLoc != string::npos) {
                PRIOS_NUM = stoi(schedRestOpt.substr(colonLoc+1, -1));
            }
        }
    }

    if (optind+1 < argc) {
        INPUT_FILE = string(argv[optind]);
        RAND_FILE = string(argv[optind+1]);
    }

    // read processes
    ifstream ifsInputFile(INPUT_FILE);
    Process* temp = nullptr;
    int arriveT, totalT, cpuBurst, ioBurst;
    while (ifsInputFile >> arriveT) {
        ifsInputFile >> totalT >> cpuBurst >> ioBurst;
        temp = new Process(arriveT, totalT, cpuBurst, ioBurst);
        PROCESS_POOL.push_back(temp);
    }
    ifsInputFile.close();

    // read random file
    ifstream ifsRandFile(RAND_FILE);
    ifsRandFile >> RANDVALS_SIZE;
    long nextInt;
    int count = 0;
    while (ifsRandFile >> nextInt && count < RANDVALS_SIZE) {
        RANDVALS.push_back(nextInt);
        count++;
    }
    ifsRandFile.close();

    // assign static priority and dynamic priority
    for (auto p : PROCESS_POOL) {
        p->static_priority = MyRandom(PRIOS_NUM);
        p->dynamic_priority = p->static_priority - 1;
    }

    // we cannot assure the process is listed in ascending order of arrival time
    std::stable_sort(PROCESS_POOL.begin(), PROCESS_POOL.end(),
            [](auto a, auto b) {return a->arrivalTime < b->arrivalTime; });

    Simulation();

}