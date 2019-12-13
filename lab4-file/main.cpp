#include <iostream>
#include <fstream>
#include <getopt.h>
#include <memory>
#include "MyReader.h"
#include "IOScheduler.h"
#include "MyLogger.h"

using namespace std;

// global variables
string INPUT_PATH;
IOSchedType IO_SCHED_SPEC;
bool LOG_VERBOSE = false;
bool LOG_DETAIL = false;
bool LOG_FLOOK_QUEUE_INFO = false;

void ReadArgs(int argc, char** argv) {
    int opt;
    string algorithm_choice;
    string options_choice;

    while ((opt = getopt(argc, argv, "s:vqf")) != -1) {
        switch (opt) {
            case 's':
                algorithm_choice = string(optarg); break;
            case 'v':
                LOG_VERBOSE = true; break;
            case 'q':
                LOG_DETAIL = true; break;
            case 'f':
                LOG_FLOOK_QUEUE_INFO = true; break;
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
    scheduler->Start();
    for (int current_time = 0;!io_requests.empty() || scheduler->IsActive() || scheduler->IsPending(); ++current_time) {
        scheduler->SetTime(current_time);
        if (!io_requests.empty() && current_time == io_requests.front()->timestamp) {
            // a new request comes at this time
            // add it to io queue
            scheduler->LogNew(io_requests.front());
            scheduler->AddNewIORequest(io_requests.front());
            io_requests.pop();
        }

        if (!scheduler->IsActive() && scheduler->IsPending()) {
            scheduler->FetchNext();
            scheduler->LogNext();
        }

        // why this? because multiple request may have the same track number, so either you stop the world, or loop it
        while (scheduler->IsActive()) {
            if (scheduler->IsCompleted()) {
                scheduler->ClearActive();
                if (scheduler->IsPending()) {
                    scheduler->FetchNext();
                    scheduler->LogNext();
                } else {
                    break;
                }
            } else {
                break;
            }
        }

        if (scheduler->IsActive()) {
            scheduler->MoveForward();
        }

    }
    scheduler->LogSummary();
}

int main(int arc, char** argv) {

    ReadArgs(arc, argv);
    MyReader reader(std::make_unique<ifstream>(INPUT_PATH));
    MyLogger logger(std::cout);
    logger.SetLogDetail(LOG_DETAIL);
    logger.SetLogVerbose(LOG_VERBOSE);
    logger.SetLogFlookDetail(LOG_FLOOK_QUEUE_INFO);

    queue<unique_ptr<Request>> io_requests;
    int timestamp, track_num;
    int op_idx = 0;
    while (reader >> timestamp >> track_num) {
        io_requests.push(make_unique<Request>(Request{.op_idx=op_idx, .timestamp=timestamp, .track_num=track_num}));
        op_idx++;
    }

    auto scheduler = IOSchedulerFactory::CreateScheduler(IO_SCHED_SPEC);
    scheduler->SetLogger(logger);
    Simulation(std::move(scheduler), io_requests);

    return 0;
}