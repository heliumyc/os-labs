//
// Created by CONG YU on 2019/11/26.
//

#ifndef LAB3_MMU_MYRANDOMGENERATOR_H
#define LAB3_MMU_MYRANDOMGENERATOR_H

#include <vector>

using namespace std;
class MyRandomGenerator {

public:
    vector<int> random_vals;
    unsigned int index = 0;
    unsigned int random_size;
    int GetRandomInt();
};


#endif //LAB3_MMU_MYRANDOMGENERATOR_H
