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
DAY(5, utils::FileReader& reader, int part) {
    int res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(4, utils::FileReader& reader, int part) {
    int res = 0;
    const char xmas[5] = "XMAS";
    const auto is_ms = [](char a, char b) { return (a == 'M' && b == 'S') || (b == 'M' && a == 'S');};
    auto lines = reader.allLines();
    const int n = lines.size();
    const int m = lines[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (part == 1) {
                if (lines[i][j] == xmas[0]) {
                    for (const auto d : utils::dir::ALL8) {
                        int k = 0;
                        for (k = 1; k < 4; ++k) {
                            const int ii = i + d.i * k;
                            const int jj = j + d.j * k;
                            if (!(ii >= 0 && ii < n && jj >= 0 & jj < m && lines[ii][jj] == xmas[k])) {
                                break;
                            }
                        }
                        res += (k == 4);
                    }
                }
            } else {
                if (lines[i][j] == 'A' && i && j && i < n - 1 && j < m - 1) {
                    char x[4] = {lines[i - 1][j - 1], lines[i + 1][j + 1], lines[i - 1][j + 1], lines[i + 1][j - 1]};
                    if (is_ms(x[0], x[1]) && is_ms(x[2], x[3])) {
                        res++;
                    }
                }
            }
        }
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(3, utils::FileReader& reader, int part) {
    int res = 0;
    bool en = true;
    while (reader.nextLine()) {
        const char *p = reader.getLine().get().data();
        int a = 0, b = 0;
        int z = 0;
        while (*p) {
            const char c = *p;
            const int prevz = z;
            switch (z) {
                case 0: a = b = 0; if (c == 'm') z++; else if (part == 2 && c == 'd') z = 7; else z = 0; break;
                case 1: if (c == 'u') z++; else z = 0; break;
                case 2: if (c == 'l') z++; else z = 0; break;
                case 3: if (c == '(') z++; else z = 0; break;
                case 4: if (isdigit(c)) a = a * 10 + c - '0'; else if (c == ',') z++; else z = 0; break;
                case 5: if (isdigit(c)) b = b * 10 + c - '0'; else if (c == ')') z++; else z = 0; break;
                 case 6: if (en) res += a * b; z = 0; break;
                case 7: if (c == 'o') z++; else z = 0; break;
                case 8: if (c == '(') z++; else if (c == 'n') z = 10; else z = 0; break;
                 case 9: if (c == ')') en = true; z = 0; break;
                case 10: if (c == '\'') z++; else z = 0; break;
                case 11: if (c == 't') z++; else z = 0; break;
                case 12: if (c == '(') z++; else z = 0; break;
                 case 13: if (c == ')') en = false; z = 0; break;
            }
            if (!(prevz && (!z ||  z == 6))) ++p;
        }
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(2, utils::FileReader& reader, int part) {
    int res = 0;
    const auto vec_ok = [](std::vector<int>& v) {
        int prevd = 0;
        for (int i = 1; i < v.size(); ++i) {
            int dd = v[i] - v[i - 1];
            if (!dd || prevd * dd < 0 || std::abs(dd) > 3) {
                return false;
            }
            prevd = dd;
        }
        return true;
    };
    while (reader.nextLine()) {
        auto line = reader.getLine();
        int x = 0;
        std::vector<int> v;
        while (line.read("%d", &x)) {
            v.push_back(x);
        }
        bool ok = vec_ok(v);
        if (!ok && part == 2) {
            std::vector<int> vv(v.begin() + 1, v.end());
            for (int i = 0; !ok && i < v.size(); ++i) {
                ok = vec_ok(vv);
                if (i < vv.size()) {
                    vv[i] = v[i];
                }
            }
        }
        res += ok;
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(1, utils::FileReader& reader, int part) {
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
