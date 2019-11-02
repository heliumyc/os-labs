//
// Created by CONG YU on 2019/10/28.
//

#include "Event.h"

Event::Event(Process *event_process, int event_timestamp, StateEnum transition_from, StateEnum transition_to)
        : event_process(event_process), event_timestamp(event_timestamp), transition_from(transition_from),
          transition_to(transition_to) {}

std::string StateEnumToString(StateEnum s) {
    std::string str;
    switch (s) {
        case StateEnum::CREATE: str = "CREATE"; break;
        case StateEnum::BLOCK: str = "BLOCK"; break;
        case StateEnum::RUN: str = "RUNNG"; break;
        case StateEnum::PREEMPT: str = "READY"; break;
        case StateEnum::READY: str = "READY"; break;
        case StateEnum ::DONE: str = "DONE"; break;
    }
    return str;
}
