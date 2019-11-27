//
// Created by CONG YU on 2019/11/26.
//

#include "MyRandomGenerator.h"

int MyRandomGenerator::GetRandomInt() {
    return random_vals[(index++) % random_size];
}
