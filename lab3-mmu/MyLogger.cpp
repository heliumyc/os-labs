//
// Created by CONG YU on 2019/11/18.
//

#include <iomanip>
#include "MyLogger.h"

MyLogger::MyLogger(ostream *output) : output(output) {}

void MyLogger::PrintTransition(const Transition &transition) {

    *output << transition.instruction_idx << ": ==> " << transition.instruction_operation << " ";
    *output << transition.instruction_operand << endl;

    if (transition.segment_error) {
        *output << " " << "SEGV" << endl;
    } else if (transition.is_start){
//        *output << endl;
    } else if (transition.is_end) {
        *output << "EXIT current process " << transition.pid << endl;
        for (auto triplet: transition.unmap_frames) {
            *output << " UNMAP " << std::get<0>(triplet) << ":" << std::get<1>(triplet) << endl;
            if (std::get<2>(triplet)) {
                *output << " FOUT" << endl;
            }
        }
    } else {
        // possible unload frame
        if (transition.unmap) {
            if (transition.print_aging) {
                if (transition.esc_nru_msg != nullptr) {
                    auto* aging = transition.esc_nru_msg;
                    *output << "ASELECT: hand=" << std::setw(2) << aging->hand << " " << aging->reset << " | " << aging->lowest;
                    *output << " " << std::setw(2) << aging->victim_frame << " " << std::setw(2) << aging->scan_num << endl;
                    delete transition.esc_nru_msg;
                } else if (transition.plain_aging_msg != nullptr) {
                    auto* aging = transition.plain_aging_msg;
                    *output << "ASELECT " << aging->scan_start << "-" << aging->scan_end << " | ";
                    for (auto& p: aging->frame_info) {
                        *output << p.first << ":" << std::hex << p.second << std::dec<< " ";
                    }
                    *output << "| " << aging->frame_selected << endl;
                    delete transition.plain_aging_msg;
                } else if (transition.ws_aging_msg != nullptr) {
                    auto* aging = transition.ws_aging_msg;
                    *output << "ASELECT " << aging->scan_start << "-" << aging->scan_end << " | ";
                    for (auto& t: aging->frame_info) {
                        *output << std::get<0>(t) << "(" << std::get<1>(t) << " ";
                        *output << std::get<2>(t) << ":" << std::get<3>(t) << " " << std::get<4>(t) << ") ";
                    }
                    if (aging->early_stop) {
                        *output << "STOP(" << aging->scan_cnt << ") ";
                    }
                    *output << "| " << aging->frame_selected << endl;
                    delete transition.ws_aging_msg;
                }
            }
            auto unmapped_frame = transition.unmap_frames[0];
            *output << " UNMAP " << std::get<0>(unmapped_frame) << ":" << std::get<1>(unmapped_frame) << endl;
        }
        // possible some frame is swapped out into the swap device
        if (transition.page_out) {
            *output << (transition.out_page_file_mapped ? " FOUT" : " OUT") << endl;
        }
        // zero has the highest priority
        if (transition.page_in) {
            if (transition.in_page_intact) {
                *output << " ZERO" << endl;
            } else {
                *output << (transition.in_page_file_mapped ? " FIN" : " IN") << endl;
            }
        }

        // map a new frame
        if (transition.map) {
            *output << " MAP " <<  transition.map_frame << endl;
        }

        // check write protect
        if (transition.segment_protect) {
            *output << " SEGPROT" << endl;
        }
    }
}

void MyLogger::PrintPageTable(const vector<PageTableEntry*>& page_table, unsigned int pid) {
    *output << "PT[" << pid << "]: ";
    int i = 0;
    for (const auto& pte: page_table) {
        if (!pte->PRESENT) {
            *output << (pte->SWAPPED ? "# " : "* " );
        } else {
            *output << i << ":";
            *output << ( pte->REFERENCED ? "R" : "-" );
            *output << ( pte->MODIFIED? "M" : "-" );
            *output << (pte->SWAPPED ? "S" : "-" );
            *output << " ";
        }
        i++;
    }
    *output << endl;
}

void MyLogger::PrintFrameTable(const vector<Frame*>& frame_table) {
    *output << "FT: ";
    for (const auto& frame: frame_table) {
        if (frame == nullptr || !frame->mapped) {
            *output << "* ";
        } else {
            *output << frame->pid << ":" << frame->vpage << " ";
        }
    }
    *output << endl;
}

void MyLogger::PrintSummary(const vector<Process *> &process_list, const CostStats &cost_stats) {
    for(const auto& process: process_list) {
        printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%lu Z=%lu SV=%lu SP=%lu\n",
               process->pid,
               process->pstats.unmaps,
               process->pstats.maps,
               process->pstats.ins,
               process->pstats.outs,
               process->pstats.fins,
               process->pstats.fouts,
               process->pstats.zeros,
               process->pstats.segv,
               process->pstats.segprot);
    }
    printf("TOTALCOST %lu %lu %lu %llu\n", cost_stats.inst_count, cost_stats.ctx_switches, cost_stats.process_exits, cost_stats.cost);
}



