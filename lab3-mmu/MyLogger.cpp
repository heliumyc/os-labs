//
// Created by CONG YU on 2019/11/18.
//

#include "MyLogger.h"

MyLogger::MyLogger(ostream *output) : output(output) {}

void MyLogger::PrintTransition(unsigned long long instruction_counter, char operation, int operand,
                               const Transition &transition) {

    *output << instruction_counter << ": ==> " << operation << " " << operand << endl;
    if (transition.segment_error) {
        *output << " " << "SEGV" << endl;
    } else if (transition.is_start){
        *output << endl;
    } else if (transition.is_end) {
        *output << "EXIT current process " << transition.pid << endl;
        for (auto p: transition.unmap_frames) {
            *output << " UNMAP " << p.first << ":" << p.second << endl;
        }
    } else {
        // possible unload frame
        if (transition.unmap) {
            for (auto p: transition.unmap_frames) {
                *output << " UNMAP " << p.first << ":" << p.second << endl;
            }
        }
        // possible some frame is out
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
    }
}

void MyLogger::PrintPageTable(const vector<PageTableEntry*>& page_table, unsigned int pid) {
    *output << "PT[" << pid << "]: ";
    int i = 0;
    for (const auto& pte: page_table) {
        if (!pte->PRESENT) {
            *output << ( pte->PAGEDOUT? "# " : "* " );
        } else {
            *output << i << ":";
            *output << ( pte->REFERENCED ? "R" : "-" );
            *output << ( pte->MODIFIED? "M" : "-" );
            *output << ( pte->PAGEDOUT? "M" : "-" );
            *output << " ";
        }
        i++;
    }
    *output << endl;
}

void MyLogger::PrintFrameTable(const vector<Frame*>& frame_table) {
    *output << "FT: ";
    for (const auto& frame: frame_table) {
        if (frame == nullptr) {
            *output << "* ";
        } else {
            *output << frame->pid << ":" << frame->vpage << " ";
        }
    }
    *output << endl;
}



