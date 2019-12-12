#include <iostream>
#include <fstream>
#include <getopt.h>
#include <memory>
#include "MyReader.h"
#include "IOScheduler.h"

using namespace std;

// global variables
string INPUT_PATH;
IOSchedType IO_SCHED_SPEC;
bool IS_VERBOSE = false;
bool IS_DETAIL = false;
bool IS_FLOOK_QUEUE_INFO = false;

void ReadArgs(int argc, char** argv) {
    int opt;
    string algorithm_choice;
    string options_choice;

    while ((opt = getopt(argc, argv, "s:vqf")) != -1) {
        switch (opt) {
            case 's':
                algorithm_choice = string(optarg); break;
            case 'v':
                IS_VERBOSE = true; break;
            case 'q':
                IS_DETAIL = true; break;
            case 'f':
                IS_FLOOK_QUEUE_INFO = true; break;
            default:
                cout << "unknown args: " << opt << endl; break;
        }
    }

    // -s options
    if (!algorithm_choice.empty()) {
        switch (algorithm_choice[0]) {
            case 'i':
                IO_SCHED_SPEC = IOSchedType::FIFO;break;
            case 'j':
                IO_SCHED_SPEC = IOSchedType::SSTF;break;
            case 's':
                IO_SCHED_SPEC = IOSchedType::LOOK;break;
            case 'c':
                IO_SCHED_SPEC = IOSchedType::CLOOK;break;
            case 'f':
                IO_SCHED_SPEC = IOSchedType::FLOOK;break;
            default:
                cout << "unknown args " << algorithm_choice[0] << endl;
                break;
        }
    }

    if (optind < argc) {
        INPUT_PATH = string(argv[optind]);
    }
}

void Simulation(unique_ptr<IOScheduler> scheduler, queue<unique_ptr<Request>> &io_requests) {
    for (int current_time = 0;!io_requests.empty() || scheduler->IsActive() || scheduler->IsPending(); ++current_time) {
        if (current_time == io_requests.front()->timestamp) {
            // a new request comes at this time
            // add it to io queue
            scheduler->AddNewIORequest(std::move(io_requests.front()));
            io_requests.pop();
        }
        if (scheduler->IsActive()) {
            if (scheduler->IsCompleted()) {
                // logging summary info
                scheduler->ClearActive();
            } else {
                scheduler->MoveForward();
            }
        }
        // if completed, must clear active state so next pending io will be issued immediately
        // no one is active now but some is pending
        if (!scheduler->IsActive() && scheduler->IsPending()){
            // this must happen immediately after last io has finished
            scheduler->FetchNextAndStartNewIO();
        }

        scheduler->IncrementTime();
    }
}

int main(int arc, char** argv) {

    ReadArgs(arc, argv);
    MyReader reader(std::make_unique<ifstream>(INPUT_PATH));

    queue<unique_ptr<Request>> io_requests;
//     load IO request from input
    int timestamp, track_num;
    while (reader >> timestamp >> track_num) {
        io_requests.push(make_unique<Request>(Request{.timestamp=timestamp, .track_num=track_num}));
    }
    Simulation(IOSchedulerFactory::CreateScheduler(IO_SCHED_SPEC), io_requests);
    return 0;
}