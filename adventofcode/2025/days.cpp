/*
 * adventofcode.cpp
 *
 * Author: Mihai Feier
 */
#include "days.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <array>
#include <functional>
#include <numeric>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <thread>

//-----------------------------------------------------------------------------
DAY(1, utils::FileReader& reader, int part) {
    long long res = 0;
    for (auto line : reader) {
        int a = 0;
        if (!line.read("%d", &a)) {
            throw utils::MyException("error reading");
        }
    }
    printf("Res%d = %ld\n", part, res);
}
//-----------------------------------------------------------------------------
