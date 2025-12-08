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
DAY(7, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<long long> v;
    if (reader.nextLine()) {
        auto line = reader.getLine().get();
        v.resize(line.size());
        for (int i = 0; i < line.size(); ++i) {
            v[i] = line[i] == 'S';
        }
    }
    for (auto line : reader) {
        auto l = line.get();
        for (int i = 0; i < l.size(); ++i) {
            if (v[i] && l[i] == '^') {
                v[i - 1] += v[i];
                v[i + 1] += v[i];
                v[i] = 0;
                if (part == 1) {
                    res++;
                }
            }
        }
    }
    if (part == 2) {
        res = std::accumulate(v.begin(), v.end(), 0ll);
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(6, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<std::vector<long long>> v;
    std::vector<char> ops;
    // OK, I don't like this solution, I need to refactor it at some point
    if (part == 1) {
        for (auto line : reader) {
            std::vector<long long> lineV;
            for (auto v : line.split(" ")) {
                if (isdigit(v[0])) {
                    lineV.push_back(atoll(v.data()));
                } else {
                    ops.push_back(v[0]);
                }
            }
            if (!lineV.empty()) {
                v.push_back(lineV);
            } else {
                break;
            }
        }
    } else {
        auto [text, n, m] = reader.allLines();
        v.resize(n - 1);
        for (auto& l : text) {
            m = std::max(m, static_cast<int>(l.size()));
        }
        int k = 0;
        for (int j = 0; j < m; ++j) {
            if (text[n - 1].size() > j && text[n - 1][j] != ' ') {
                for (int i = 0; i < n - 1; ++i) {
                    v[i].emplace_back();
                }
                ops.push_back(text[n - 1][j]);
                k = 0;
            } else {
                k++;
            }
            for (int i = 0; i < n - 1; ++i) {
                if (text[i].size() > j && text[i][j] != ' ') {
                    v[k].back() = v[k].back() * 10 + text[i][j] - '0';
                }
            }
        }
    }
    for (int i = 0; i < ops.size(); ++i) {
        long long x = v[0][i];
        for (int j = 1; j < v.size(); ++j) {
            if (ops[i] == '+') {
                x += v[j][i];
            } else if (ops[i] == '*') {
                x *= std::max(1LL, v[j][i]);
            }
        }
        res += x;
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(5, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<std::pair<long long, long long>> v;
    for (auto line : reader) {
        long long a = 0, b = 0;
        if (!line.read("%lld-%lld", &a, &b)) {
            break;
        }
        v.emplace_back(a, b);
    }
    if (part == 1) {
        for (auto line : reader) {
            long long x = 0;
            if (!line.read("%lld", &x)) {
                break;
            }
            bool in = false;
            for (const auto [a, b] : v) {
                if (a <= x && x <= b) {
                    in = true;
                    break;
                }
            }
            if (in) {
                res++;
            }
        }
    } else {
        std::sort(v.begin(), v.end());
        long long s = 0, e = -1;
        for (const auto [a, b] : v) {
            if (a <= e) {
                e = std::max(e, b);
            } else {
                res += e - s + 1;
                s = a;
                e = b;
            }
        }
        res += e - s + 1;
    }

    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(4, utils::FileReader& reader, int part) {
    long long res = 0;
    auto [map, n, m] = reader.allLines();
    bool done = false;
    while (!done) {
        done = true;
        for (int i = 0; i < n; ++i) {
            const auto& line = map[i];
            for (int j = 0; j < m; ++j) {
                if (line[j] == '@') {
                    int c = 0;
                    for (auto d : utils::dir::ALL8) {
                        const auto p = utils::Coord(i, j) + d;
                        if (p.inside(n, m) && map[p.i][p.j] != '.') {
                            c++;
                        }
                    }
                    if (c < 4) {
                        res++;
                        if (part == 2) {
                            done = false;
                            map[i][j] = '.';
                        }
                    }
                }
            }
        }
    }
    printf("Res%d = %lld\n", part, res);
}
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
        // try a smart scan, by computing parts of number in the range
        // and assembling them by repetition
        std::unordered_set<long long> nbs;
        int d = utils::digits(a);
        for (auto x = a; x <= b; x = std::pow(10, d), ++d) {
            for (int k = 2; k <= d; ++k) {
                if (d % k == 0) {
                    long long q = x / std::pow(10, d - d / k);
                    const long long e = std::pow(10, d / k);
                    while (q < e) {
                        auto w = q;
                        for (int i = 1; i < k; ++i) {
                            w = w * e + q;
                        }
                        if (w >= a && w <= b) {
                            nbs.insert(w);
                        }
                        q++;
                    }
                }
                if (part == 1) break;
            }
        }
        for (const auto nb : nbs) {
            res += nb;
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
            // count all other passes through 0
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
