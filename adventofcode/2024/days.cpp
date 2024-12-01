/*
 * adventofcode.cpp
 *
 * Author: Mihai Feier
 * Copyright QTronic GmbH. All rights reserved.
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
void day2(utils::FileReader& reader, int part) {
    while (reader.nextLine()) {
        auto line = reader.getLine();
    }
    int res = 0;
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
void day1(utils::FileReader& reader, int part) {
    std::vector<int> aa, bb;
    std::map<int, int> ab;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        int a = 0, b = 0;
        if (!line.read("%d %d", &a, &b)) {
            throw utils::MyException("error reading game");
        }
        aa.push_back(a);
        bb.push_back(b);
        ab[b]++;
    }
    std::sort(aa.begin(), aa.end());
    std::sort(bb.begin(), bb.end());
    int res = 0;
    for (int i = 0; i < aa.size(); ++i) {
        res += part == 1? std::abs(aa[i] - bb[i]) : aa[i] * ab[aa[i]];
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
