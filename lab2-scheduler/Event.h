//
// Created by CONG YU on 2019/10/28.
//

#ifndef LAB2_SCHEDULER_EVENT_H
#define LAB2_SCHEDULER_EVENT_H


#include <string>
#include "Process.h"

enum class StateEnum {
    CREATE,
    READY,
    RUN,
    BLOCK,
    PREEMPT,
    DONE
};

std::string StateEnumToString(StateEnum s);

class Event {
public:
    Process* event_process;
    int event_timestamp;

    StateEnum transition_from;
    StateEnum transition_to;

    Event(Process *event_process, int event_timestamp, StateEnum transition_from, StateEnum transition_to);

    int burst_log = 0;
};


#endif //LAB2_SCHEDULER_EVENT_H
