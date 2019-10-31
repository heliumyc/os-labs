//
// Created by CONG YU on 2019/10/28.
//

#include "Event.h"

Event::Event(Process *event_process, int event_timestamp, StateEnum transition_from, StateEnum transition_to)
        : event_process(event_process), event_timestamp(event_timestamp), transition_from(transition_from),
          transition_to(transition_to) {}

std::string StateEnumToString(StateEnum s) {
    switch (s) {
        case StateEnum::CREATE: return "CREATE";
        case StateEnum::BLOCK: return "BLOCK";
        case StateEnum::RUN: return "RUN";
        case StateEnum::PREEMPT: return "PREEMPT";
        case StateEnum::READY: return "READY";
        case StateEnum ::DONE: return "DONE";
    }
}
