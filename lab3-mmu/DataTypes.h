//
// Created by CONG YU on 2019/11/10.
//

#ifndef LAB3_MMU_DATATYPES_H
#define LAB3_MMU_DATATYPES_H

#include <vector>
#include <tuple>
#include <utility>
#include <cstdint>

typedef struct {
    int fid = -1;
    int pid = -1;
    int vpage = -1;
    bool mapped = false;
    uint32_t aging = 0;
    int last_use = 0;

    void Unmap() {
        pid = -1;
        vpage = -1;
        mapped = false;
        aging = 0;
        last_use = 0;
    }
} Frame;

typedef struct {
    char operation;
    int operand;
} Instruction;

typedef struct {
    int hand = -1;
    bool reset = false;
    int lowest = 0;
    int victim_frame = -1;
    int scan_num = 0;
} ESC_NRU_Aging_Msg;

typedef struct {
    int scan_start = -1;
    int scan_end = -1;
    std::vector<std::pair<int, uint32_t>> frame_info;
    int frame_selected = -1;
} Plain_Aging_Msg;

typedef struct {
    int scan_start = -1;
    int scan_end = -1;
    std::vector<std::tuple<int, int, int, int, int>> frame_info;
    bool early_stop = false;
    int scan_cnt = 0;
    int frame_selected = -1;
} WS_Aging_Msg;

typedef struct {
    int instruction_idx = 0;
    char instruction_operation = 0;
    int instruction_operand = 0;

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

    bool print_aging = false;
    ESC_NRU_Aging_Msg* esc_nru_msg = nullptr;
    Plain_Aging_Msg* plain_aging_msg = nullptr;
    WS_Aging_Msg* ws_aging_msg = nullptr;

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
