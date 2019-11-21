//
// Created by CONG YU on 2019/11/15.
//

#ifndef LAB3_MMU_PROCESS_H
#define LAB3_MMU_PROCESS_H

#include <vector>
#include "DataTypes.h"

using namespace std;

const unsigned int PTE_SIZE = 64;

typedef struct {
    unsigned int PRESENT : 1;
    unsigned int WRITE_PROTECT : 1;
    unsigned int MODIFIED : 1;
    unsigned int REFERENCED : 1;
    unsigned int PAGEDOUT : 1;
    unsigned int FRAME_NUM: 7;
    // following is 20 bits of free allocation
    unsigned int UNK: 20;
} PageTableEntry;

typedef struct {
    int start_vpage;
    int end_vpage;
    bool write_protected;
    bool file_mapped;
} VirtualMemoryArea;

class Process {

public:
    int pid;
    vector<VirtualMemoryArea*> vma_list;
    vector<PageTableEntry*> page_table;

    explicit Process(int pid);
    void AddVirtualMemoryArea(VirtualMemoryArea* vma);

};

#endif //LAB3_MMU_PROCESS_H
