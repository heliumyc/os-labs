#include <iostream>
#include <deque>
#include <getopt.h>
#include <vector>
#include <fstream>
#include "DataTypes.h"
#include "Pager.h"
#include "MyReader.h"
#include "Process.h"
#include "MyLogger.h"
#include "MyRandomGenerator.h"

using namespace std;

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

int FRAME_NUM = 0;

string INPUT_PATH;
string RANDOM_PATH;

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
                FRAME_NUM = (int)strtol(optarg, nullptr, 0); break;
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

Frame* GetFrame(deque<Frame*>& free_frame_list, Pager& pager, bool& page_out, Transition& transition) {
    // allocate frame from free list
    Frame* frame;
    if (!free_frame_list.empty()) {
        frame = free_frame_list.front();
        free_frame_list.pop_front();
        page_out = false;
    }
    else {
        frame = pager.SelectVictimFrame(transition);
        page_out = true;
    }
    return frame;
}

bool IterateCheckVma(const vector<VirtualMemoryArea*>& vma_list, int vpage, const function<bool(const VirtualMemoryArea& vma)>& func) {
    // this can be optimized by interval tree or segment tree
    // BUT premature optimization is the root of evil. so just forget it
    for (const auto& vma: vma_list) {
        if (vpage >= vma->start_vpage && vpage <= vma->end_vpage) {
            return func(*vma);
        }
    }
    return false;
}

void ExitProcess(Process& process, deque<Frame*>& free_frame_list, vector<Frame*> frame_table, Transition& transition) {
   for (auto& pte: process.page_table) {
       if (pte->PRESENT) {
           auto* frame = frame_table[pte->FRAME_NUM];
           transition.unmap_frames.emplace_back(frame->pid, frame->vpage,  pte->FILE_MAPPED && pte->MODIFIED);
           frame->Unmap();
           free_frame_list.push_back(frame);
           // note that when exit, you unmap some frame and page, and they count too
           process.pstats.unmaps++;
           (pte->FILE_MAPPED && pte->MODIFIED) ? process.pstats.fouts++ : 0;
       }

       pte->CREATED = 0;
       pte->FILE_MAPPED = 0;
       pte->WRITE_PROTECT = 0;
       pte->SWAPPED = 0;
       pte->Unmap();
       // count on unmap of exit process?
   }
}

void CalcCost(const vector<Process*>& process_list, CostStats& cost_stats) {
    for(const auto& p: process_list) {
        cost_stats.cost += 400 * (p->pstats.maps + p->pstats.unmaps);
        cost_stats.cost += 3000 * (p->pstats.ins + p->pstats.outs);
        cost_stats.cost += 2500 * (p->pstats.fins + p->pstats.fouts);
        cost_stats.cost += 150 * (p->pstats.zeros);
        cost_stats.cost += 240 * (p->pstats.segv);
        cost_stats.cost += 300 * (p->pstats.segprot);
        cost_stats.cost += 1 * (p->pstats.access);
    }
    cost_stats.cost += 121 * (cost_stats.ctx_switches);
    cost_stats.cost += 175 * (cost_stats.process_exits);
}

void Simulation(MyReader& reader, MyLogger& logger, Pager& pager, deque<Frame*>& free_frame_list, vector<Process*>& process_list) {
    /// premature optimization is the root of evil
    CostStats cost_stats = {};
    Instruction current_instruction = {};
    Process* current_process = nullptr;
    int instruction_counter = 0;

    while (reader.GetNextInstruction(current_instruction)) {
        cost_stats.inst_count++;
        Transition transition = {.instruction_idx=instruction_counter,
                                 .instruction_operation=current_instruction.operation,
                                 .instruction_operand=current_instruction.operand};
        if (current_instruction.operation == 'c') {
            /// what if pid invalid?
            cost_stats.ctx_switches++;
            current_process = process_list[current_instruction.operand];
            transition.is_start = true;
            transition.pid = current_process->pid;
        }
        else if (current_instruction.operation == 'e') {
            // need to unmap all your stuffs in the frametable and also in pagetable, but keep others intact
            transition.is_end = true;
            transition.pid = current_process->pid;

            ExitProcess(*current_process, free_frame_list, *pager.frame_table, transition);

            current_process = nullptr;
            cost_stats.process_exits++;
        }
        // check for w and r
        else {
            int current_vpage = current_instruction.operand;

            // check if the operand is valid or not
            bool operandValid = IterateCheckVma(current_process->vma_list, current_vpage, [](const VirtualMemoryArea& vma){
                return 1;
            });
            if (!operandValid) {
                transition.segment_error = true;
                current_process->pstats.segv++;
            }
            else {
                // check if a victim frame is selected and evicted
                // if not just insert it to
                auto& pte = current_process->page_table[current_vpage];

                // check if this page is created or not
                if (!pte->CREATED) {
                    pte->CREATED = 1;
                    pte->FILE_MAPPED = IterateCheckVma(current_process->vma_list, current_vpage, [](const VirtualMemoryArea& vma){
                        return vma.file_mapped == 1;
                    });
                    pte->WRITE_PROTECT = IterateCheckVma(current_process->vma_list, current_vpage, [](const VirtualMemoryArea& vma){
                        return vma.write_protected == 1;
                    });
                }

                // page fault occurs
                if (!pte->PRESENT) {
                    // get a new frame to map the page required
                    pte->PRESENT = 1;

                    bool page_out = false;
                    auto victim_frame = GetFrame(free_frame_list, pager, page_out, transition);

                    // frame table is full, some frame is out
                    if (page_out) {
                        // unmap, unmap means remove information in the pagetable
                        // means there is a page out
                        auto victim_process = process_list[victim_frame->pid];
                        auto& out_page = victim_process->page_table[victim_frame->vpage];

                        // !!! note that if file mapped, you do not throw page to swap device but directly the mapped file
                        out_page->SWAPPED = (out_page->MODIFIED || out_page->SWAPPED) && !out_page->FILE_MAPPED;

                        // logging
                        transition.unmap = true; // UNMAP
                        transition.unmap_frames.emplace_back(victim_frame->pid, victim_frame->vpage);
                        transition.page_out = out_page->MODIFIED; // FOUT/OUT
                        transition.out_page_file_mapped = out_page->FILE_MAPPED;

                        if (transition.page_out) {
                            transition.out_page_file_mapped? victim_process->pstats.fouts++: victim_process->pstats.outs++;
                        }

                        // unmap
                        // no, unmaps count belongs to its owner, not current process
                        victim_process->pstats.unmaps++;
                        out_page->Unmap();
                        victim_frame->Unmap();
                    }
                    pte->FRAME_NUM = victim_frame->fid;

                    current_process->pstats.maps++;

                    // fill in frame with the new page
                    victim_frame->pid = current_process->pid;
                    victim_frame->vpage = current_vpage;
                    victim_frame->mapped = true;
                    victim_frame->last_use = instruction_counter;

                    // logging
                    transition.page_in = true;
                    transition.in_page_intact = !pte->FILE_MAPPED && !pte->SWAPPED;
                    transition.in_page_file_mapped = pte->FILE_MAPPED;
                    transition.in_page_intact ? current_process->pstats.zeros++:
                        (transition.in_page_file_mapped? current_process->pstats.fins++: current_process->pstats.ins++);

                    transition.map = true;
                    transition.map_frame = victim_frame->fid;
                }

                // set ref or mod bit
                pte->REFERENCED = 1;
                if (current_instruction.operation == 'w') {
                    if (pte->WRITE_PROTECT) {
                        transition.segment_protect = true;
                        current_process->pstats.segprot++;
                    } else {
                        pte->MODIFIED = 1;
                    }
                }

            }
            current_process->pstats.access++;
        }

        // -a
        if (PRINT_AGING) {
            transition.print_aging = true;
        }

        // -O
        if (PRINT_OUTPUT) {
            logger.PrintTransition(transition);
        }
        if (!transition.is_start && !transition.is_end && !transition.segment_error) {
            // -y
            if (PRINT_CURRENT_ALL_PAGE_TABLE_PER_INSTRUCTION) {
                for (const auto& p: process_list) {
                    logger.PrintPageTable(p->page_table, p->pid);
                }
            } else if (PRINT_CURRENT_PAGE_TABLE_PER_INSTRUCTION) {
                // -x
                logger.PrintPageTable(current_process->page_table, current_process->pid);
            }
            // -f
            if (PRINT_FRAME_TABLE_PER_INSTRUCTION) logger.PrintFrameTable(*pager.frame_table);
        }
        instruction_counter++;
    }

    CalcCost(process_list, cost_stats);

    // -P
    if (PRINT_FINAL_PAGE_TABLE) {
        for (const auto& p: process_list) {
            logger.PrintPageTable(p->page_table, p->pid);
        }
    }
    // -F
    if (PRINT_FINAL_FRAME_TABLE) logger.PrintFrameTable(*pager.frame_table);
    // -S
    if (PRINT_SUMMARY) logger.PrintSummary(process_list, cost_stats);
}

int main(int argc, char** argv) {

    ReadArgs(argc, argv);

    ifstream input_ifs(INPUT_PATH);
    MyReader reader(&input_ifs);


    MyRandomGenerator random_gen;

    // read random file
    ifstream random_file_ifs(RANDOM_PATH);
    random_file_ifs >> random_gen.random_size;
    long next_int;
    int count = 0;
    while (random_file_ifs >> next_int && count < random_gen.random_size) {
        random_gen.random_vals.push_back(next_int);
        count++;
    }
    random_file_ifs.close();

    // init frame table
    // init frame pool
    vector<Frame*> global_frame_table( FRAME_NUM );
    deque<Frame*> free_frame_list;

    for (int i = 0; i < FRAME_NUM; ++i) {
        auto* new_frame = new Frame{.fid = i};
        global_frame_table[i] = new_frame;
        free_frame_list.push_back(new_frame);
    }

    // init process pool
    vector<Process*> process_list;

    // read process
    int process_count = reader.GetInt();
    for (int i = 0; i < process_count; i++) {
        int vma_count = reader.GetInt();
        auto* process_ptr = new Process(i);
        while (vma_count--) {
            auto* vma = new VirtualMemoryArea;
            reader.GetNextVma(*vma);
            process_ptr->AddVirtualMemoryArea(vma);
        }
        process_list.push_back(process_ptr);
    }

    // using FIFO as default pager
    Pager *pager = PagerFactory::CreatePager(PAGER_SPEC);
    pager->SetFrameTable(&global_frame_table, FRAME_NUM);
    pager->SetRandomGenerator(&random_gen);
    pager->SetProcessList(&process_list);

    MyLogger logger(&cout);
    // is_start simulation
    Simulation(reader, logger, *pager, free_frame_list, process_list);

    return 0;
}