#include <iostream>
#include <deque>
#include <getopt.h>
#include <vector>
#include <fstream>
#include "DataTypes.h"
#include "Pager.h"
#include "MyReader.h"
#include "Process.h"

using namespace std;
const unsigned int PTE_SIZE = 64;

PagerEnum PAGER_SPEC;

// fucking verbose options about printing
bool PRINT_OUTPUT = false;
bool PRINT_FINAL_PAGE_TABLE = false;
bool PRINT_FINAL_FRAME_TABLE = false;
bool PRINT_SUMMARY = false;
bool PRINT_CURRENT_PAGE_TABLE_PER_INSTRUCTION= false;
bool PRINT_CURRENT_ALL_PAGE_TABLE_PER_INSTRUCTION = false;
bool PRINT_FRAME_TABLE_PER_INSTRUCTION = false;
bool PRINT_AGING = false;

long FRAME_NUM = 0;

string INPUT_PATH;
string RANDOM_PATH;

vector<int> RANDVALS;
int RANDVALS_SIZE;
int OBF = 0;

int MyRandom(int burst) {
    return RANDVALS[(OBF++) % RANDVALS_SIZE] % burst;
}

void ReadArgs(int argc, char** argv) {
    int opt;
    string algorithm_choice;
    string options_choice;

    while ((opt = getopt(argc, argv, "a:o:f:")) != -1) {
        switch (opt) {
            case 'a':
                algorithm_choice = string(optarg); break;
            case 'o':
                options_choice = string(optarg); break;
            case 'f':
                FRAME_NUM = strtol(optarg, nullptr, 0); break;
            default:
                cout << "unknown args" << endl; break;
        }
    }

    // -a options
    if (!algorithm_choice.empty()) {
        switch (algorithm_choice[0]) {
            case 'f':
                PAGER_SPEC = PagerEnum::FIFO; break;
            case 'r':
                PAGER_SPEC = PagerEnum::RANDOM; break;
            case 'c':
                PAGER_SPEC = PagerEnum::CLOCK; break;
            case 'e':
                PAGER_SPEC = PagerEnum ::ESC_NRU; break;
            case 'a':
                PAGER_SPEC = PagerEnum ::AGING; break;
            case 'w':
                PAGER_SPEC = PagerEnum ::WORKING_SET; break;
            default:
                cout << "unknown args" << endl;
                break;
        }
    }

    // -o options
    if (!options_choice.empty()) {
        for (auto option: options_choice) {
            switch (option) {
                case 'O':
                    PRINT_OUTPUT = true; break;
                case 'P':
                    PRINT_FINAL_PAGE_TABLE = true; break;
                case 'F':
                    PRINT_FINAL_FRAME_TABLE = true; break;
                case 'S':
                    PRINT_SUMMARY = true; break;
                case 'x':
                    PRINT_CURRENT_PAGE_TABLE_PER_INSTRUCTION = true; break;
                case 'y':
                    PRINT_CURRENT_ALL_PAGE_TABLE_PER_INSTRUCTION = true; break;
                case 'f':
                    PRINT_FRAME_TABLE_PER_INSTRUCTION = true; break;
                case 'a':
                    PRINT_AGING = true; break;
                default:
                    cout << "unkown args" << endl; break;
            }
        }
    }

    if (optind+1 < argc) {
        INPUT_PATH = string(argv[optind]);
        RANDOM_PATH = string(argv[optind+1]);
    }
}

Frame* GetFrame(deque<Frame*>& free_frame_list, Pager& pager) {

    // allocate frame from free list
    Frame* frame = free_frame_list.front();
    if (frame == nullptr) {
        frame = pager.SelectVictimFrame();
    }
    return frame;
}

void Simulation(MyReader& reader, deque<Frame*>& free_frame_list, vector<Process*>& process_list) {
    Instruction current_instruction = {};
    Process* current_process;

    while (reader.GetNextInstruction(current_instruction)) {
        switch (current_instruction.operation) {
            case 'c':
                current_process = process_list[current_instruction.operand];
                break;
            case 'r':

                break;
            case 'w':
                break;
            case 'e':
                // just ignore it!
                break;
        }
    }

}

int main(int argc, char** argv) {

    ReadArgs(argc, argv);

    ifstream input_ifs(INPUT_PATH);
    MyReader reader(&input_ifs);

    // read random file
    ifstream random_file_ifs(RANDOM_PATH);
    random_file_ifs >> RANDVALS_SIZE;
    long next_int;
    int count = 0;
    while (random_file_ifs >> next_int && count < RANDVALS_SIZE) {
        RANDVALS.push_back(next_int);
        count++;
    }
    random_file_ifs.close();

    // init frame pool
    deque<Frame*> free_frame_list;
    Frame* new_frame;
    for (int i = 0; i < 128; ++i) {
        new_frame = {};
        free_frame_list.push_back(new_frame);
    }

    // init process pool
    vector<Process*> process_list;

    // read process
    int process_count = reader.GetInt();
    for (int i = 0; i < process_count; i++) {
        int vma_count = reader.GetInt();
        Process process(i);
        while (vma_count--) {
            VirtualMemoryArea vma = {};
            reader.GetNextVma(vma);
            process.AddVirtualMemoryArea(&vma);
        }
    }

    // start simulation
    Simulation(reader, free_frame_list, process_list);

    return 0;
}