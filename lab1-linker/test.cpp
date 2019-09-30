//
// Created by CONG YU on 2019/9/27.
//
#define testmacro(statement) if (input > 0){statement}

#include <iostream>
using namespace std;
int main() {
    int input = -10;
    testmacro((cout << input);)
}
