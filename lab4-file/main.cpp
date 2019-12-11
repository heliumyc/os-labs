#include <iostream>
#include <fstream>
#include <getopt.h>
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

void Simulation(IOScheduler &scheduler) {
    long long current_time = 0;
}

int main() {
    ifstream ifs(INPUT_PATH);
    MyReader reader(&ifs);
    IOScheduler *scheduler = IOSchedFactory::CreatePager(IO_SCHED_SPEC);
    Simulation(*scheduler);
    return 0;
}