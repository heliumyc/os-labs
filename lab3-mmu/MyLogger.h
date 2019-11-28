//
// Created by CONG YU on 2019/11/18.
//

#ifndef LAB3_MMU_MYLOGGER_H
#define LAB3_MMU_MYLOGGER_H

#include <vector>
#include <ostream>
#include "DataTypes.h"
#include "Process.h"

using namespace std;

class MyLogger {
private:
    ostream* output;
public:
    explicit MyLogger(ostream *output);

    void PrintTransition(const Transition& transition);
    void PrintPageTable(const vector<PageTableEntry*>& page_table, unsigned int pid);
    void PrintFrameTable(const vector<Frame*>& frame_table);
    static void PrintSummary(const vector<Process*>& process_list, const CostStats& cost_stats);
};
#endif //LAB3_MMU_MYLOGGER_H
