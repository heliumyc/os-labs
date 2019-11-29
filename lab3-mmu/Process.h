//
// Created by CONG YU on 2019/11/15.
//

#ifndef LAB3_MMU_PROCESS_H
#define LAB3_MMU_PROCESS_H

#include <vector>
#include <cstring>
#include "DataTypes.h"

using namespace std;

const unsigned int PTE_SIZE = 64;

typedef struct PageTableEntry {
    unsigned int PRESENT : 1;
    unsigned int WRITE_PROTECT : 1;
    unsigned int MODIFIED : 1;
    unsigned int REFERENCED : 1;
    unsigned int SWAPPED : 1;
    unsigned int FRAME_NUM: 7;
    // following is 20 bits of free allocation
    unsigned int CREATED: 1; // cuz of hierarchical table, some pte does not created until needed
    unsigned int FILE_MAPPED: 1; // actually can get from VMA, but this is more clear
    unsigned int UNK: 18;

    PageTableEntry() {
        memset(this, 0, sizeof(PageTableEntry));
//        PRESENT = 0;
//        WRITE_PROTECT = 0;
//        MODIFIED = 0;
//        REFERENCED = 0;
//        SWAPPED = 0;
//        FRAME_NUM = 0;
//        CREATED = 0;
//        FILE_MAPPED = 0;
//        UNK = 0;
    }

    void Unmap() {
        PRESENT = 0;
        FRAME_NUM = 0;
        MODIFIED = 0;
        REFERENCED = 0;
    }
} PageTableEntry;

typedef struct {
    int start_vpage = 0;
    int end_vpage = 0;
    bool write_protected = false;
    bool file_mapped = false;
} VirtualMemoryArea;

class Process {

public:
    int pid;
    vector<VirtualMemoryArea*> vma_list;
    vector<PageTableEntry*> page_table;
    ProcStats pstats = {};

    explicit Process(int pid);
    void AddVirtualMemoryArea(VirtualMemoryArea* vma);

};

#endif //LAB3_MMU_PROCESS_H
