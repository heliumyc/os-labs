//
// Created by CONG YU on 2019/11/15.
//

#include "Process.h"

Process::Process(int pid) : pid(pid) {}

void Process::AddVirtualMemoryArea(VirtualMemoryArea *vma) {
    vma_list.push_back(vma);
}
