//
// Created by CONG YU on 2019/11/10.
//

#ifndef LAB3_MMU_DATATYPES_H
#define LAB3_MMU_DATATYPES_H

#include <vector>

typedef struct {
    int fid = -1;
    int pid = -1;
    int vpage = -1;
    bool mapped = false;

    void Unmap() {
        pid = -1;
        vpage = -1;
        mapped = false;
    }
} Frame;

typedef struct {
    char operation;
    int operand;
} Instruction;

typedef struct {
    int pid = -1;
    bool segment_error = false;
    bool segment_protect = false;

    bool is_start = false;
    bool is_end = false;

    bool unmap = false;
    std::vector<std::tuple<int,int,bool>> unmap_frames;

    bool page_in = false;
    bool in_page_intact = false; // ゼロ!
    bool in_page_file_mapped = false;

    bool page_out = false;
    bool out_page_file_mapped = false;

    bool map = false;
    int map_frame = -1;

} Transition;

typedef struct {
    unsigned long unmaps;
    unsigned long maps;
    unsigned long ins;
    unsigned long outs;
    unsigned long fins;
    unsigned long fouts;
    unsigned long zeros;
    unsigned long segv;
    unsigned long segprot;
    unsigned long access; // R or M
} ProcStats;

typedef struct {
    unsigned long inst_count;
    unsigned long ctx_switches;
    unsigned long process_exits;
    unsigned long long cost;
} CostStats;

#endif //LAB3_MMU_DATATYPES_H
