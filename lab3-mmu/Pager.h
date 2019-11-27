//
// Created by CONG YU on 2019/11/7.
//

#ifndef LAB3_MMU_PAGER_H
#define LAB3_MMU_PAGER_H

#include "DataTypes.h"
#include "Process.h"
#include "MyRandomGenerator.h"

using namespace std;

enum class PagerEnum {FIFO, RANDOM, CLOCK, ESC_NRU, AGING, WORKING_SET};

class Pager {
protected:
    MyRandomGenerator* random_generator{};

public:
    int frame_table_size;
    vector<Frame*>* frame_table{};
    vector<Process*>* process_list;
    virtual Frame* SelectVictimFrame() = 0;
    void SetFrameTable(vector<Frame*>* global_frame_table, int global_frame_table_size);
    void SetRandomGenerator(MyRandomGenerator* global_random_generator);
    void SetProcessList(vector<Process*>* global_process_list);
};

class PagerFactory {
public:
    static Pager* CreatePager(PagerEnum pager_type);
};

#endif //LAB3_MMU_PAGER_H
