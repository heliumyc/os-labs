//
// Created by CONG YU on 2019/11/15.
//

#ifndef LAB3_MMU_PROCESS_H
#define LAB3_MMU_PROCESS_H

#include <vector>

using namespace std;

typedef struct {
    int start_vpage;
    int end_vpage;
    bool write_protected;
    bool file_mapped;
} VirtualMemoryArea;

class Process {

private:
    int pid;
    vector<VirtualMemoryArea*> vma_list;

public:
    explicit Process(int pid);
    void AddVirtualMemoryArea(VirtualMemoryArea* vma);

};

#endif //LAB3_MMU_PROCESS_H
