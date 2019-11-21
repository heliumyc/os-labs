//
// Created by CONG YU on 2019/11/10.
//

#ifndef LAB3_MMU_DATATYPES_H
#define LAB3_MMU_DATATYPES_H

#include <vector>

typedef struct {
    int pid;
    int vpage;
} Frame;

typedef struct {
    char operation;
    int operand;
} Instruction;

typedef struct {
    int pid = -1;
    bool segment_error = false;

    bool is_start = false;
    bool is_end = false;

    bool unmap = false;
    std::vector<std::pair<int, int>> unmap_frames;

    bool page_in = false;
    bool in_page_intact = false; // ゼロ!
    bool in_page_file_mapped = false;

    bool page_out = false;
    bool out_page_file_mapped = false;

    bool map = false;
    int map_frame = -1;

} Transition;

#endif //LAB3_MMU_DATATYPES_H
