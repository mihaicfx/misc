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
DAY(3, utils::FileReader& reader, int part) {
    long long res = 0;
    for (auto line : reader) {
        const auto l = line.get();
        long long v = 0;
        auto s = l.begin();
        for (int k = part == 2? 11: 1; k >= 0; --k) {
            const auto it = std::max_element(s, l.end() - k);
            v = v * 10 + *it - '0';
            s = it + 1;
        }
        res += v;
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(2, utils::FileReader& reader, int part) {
    long long res = 0;
    reader.nextLine();
    auto line = reader.getLine();
    for (auto range : line.split(",")) {
        long long a = 0, b = 0;
        sscanf(range.data(), "%lld-%lld", &a, &b);
        for (auto i = a; i <= b; ++i) {
            const int d = utils::digits(i);
            for (int k = 2; k <= d; ++k) {
                if (d % k == 0) {
                    const long long f = std::pow(10, d / k);
                    bool ok = true;
                    const auto r = i % f;
                    for (auto x = i / f; x > 0; x /= f) {
                        if (x % f != r) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        res += i;
                        break;
                    }
                }
                if (part == 1) break;
            }
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(1, utils::FileReader& reader, int part) {
    long long res = 0;
    int dial = 50;
    for (auto line : reader) {
        char c = 0;
        int a = 0;
        if (!line.read("%c%d", &c, &a)) {
            throw utils::MyException("error reading dial");
        }

        const int d = c == 'L'? -1: 1;
        const int rot = d * a;
        if (part == 2) {
            int d1 = dial + rot - d;
            if (d1 <= 0) d1 = -d1 + (dial != 0? 100: 0);
            res += d1 / 100;
        }
        dial = ((dial + rot) % 100 + 100) % 100;
        res += dial == 0;
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
