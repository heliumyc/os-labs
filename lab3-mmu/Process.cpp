//
// Created by CONG YU on 2019/11/15.
//

#include "Process.h"

Process::Process(int pid) : pid(pid) {
    for (int i = 0; i < PTE_SIZE; ++i) {
        page_table.push_back(new PageTableEntry);
    }
}

void Process::AddVirtualMemoryArea(VirtualMemoryArea *vma) {
    vma_list.push_back(vma);
}
