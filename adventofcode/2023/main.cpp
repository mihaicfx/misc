/*
 * adventofcode.cpp
 *
 * Author: Mihai Feier
 * Copyright QTronic GmbH. All rights reserved.
 */

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

#include "utils.h"

//-----------------------------------------------------------------------------
void day1(utils::FileReader& reader) {
    static constexpr int PART = 2;
    char buf[100];
    int res = 0;
    const std::array<std::string, 9> words = {
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };
    while (fscanf(reader.f, "%99s", buf) == 1) {
        int a = -1, b = -1;
        const int n = strlen(buf);
        for (int i = 0; i < n; ++i) {
            int val = -1;
            if (isdigit(buf[i])) {
                val = buf[i] - '0';
            }
            if (PART == 2) {
                for (int k = 0; k < words.size(); ++k) {
                    if (strncmp(buf + i, words[k].c_str(), words[k].length()) == 0) {
                        val = k + 1;
                    }
                }
            }
            if (val != -1) (a == -1? a: b) = val;
        }
        printf("%s = %d/%d\n", buf, a, (b == -1? a: b));
        res += a * 10 + (b == -1? a: b);
    }
    printf("Res = %d\n", res);
}
//-----------------------------------------------------------------------------
void day2(utils::FileReader& reader) {
    static constexpr int PART = 2;
    int res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        int game = 0;
        if (!line.read("Game %d:", &game)) {
            throw utils::MyException("error reading game");
        }
        char type[10] = {};
        int value = 0;
        int rgb[3] = {};
        bool gg = true;
        int mrgb[3] = {};
        while (line.read("%d", &value) && line.read(" %9[a-z]", type)) {
            char delim = 0;
            line.read("%c", &delim);
            const int index = type[0] == 'r'? 0: type[0] == 'g'? 1: type[0] == 'b'? 2: 3;
            if (index > 2) {
                throw utils::MyException("invalid type " + std::string(type));
            }
            rgb[index] = value;
            if (delim != ',') {
                if (PART == 1) {
                    if (rgb[0] > 12 || rgb[1] > 13 || rgb[2] > 14) {
                        gg = false;
                    }
                } else {
                    for (int i = 0; i < 3; ++i) {
                        mrgb[i] = std::max(mrgb[i], rgb[i]);
                    }
                }
                std::fill(std::begin(rgb), std::end(rgb), 0);
            }
        }
        if (PART == 1) {
            if (gg) {
                res += game;
            }
        } else {
            res += mrgb[0] * mrgb[1] * mrgb[2];
        }
    }
    printf("Res = %d\n", res);
}
//-----------------------------------------------------------------------------
void day3(utils::FileReader& reader) {
    int res = 0;
    std::vector<std::string> m = reader.allLines();
    std::unordered_map<int, std::vector<int>> gears;
    auto issym = [&m](char c) { return c != '.' && c != 0 && !isdigit(c); };
    for (int i = 0; i < m.size(); ++i) {
        int val = 0;
        int nstart = -1;
        for (int j = 0; j <= m[i].length(); ++j) {
            if (isdigit(m[i][j])) {
                val = val * 10 + (m[i][j] - '0');
                if (nstart == -1) nstart = j;
            } else if (val) {
                bool good = false;
                for (int ii = std::max(0, i - 1); ii < std::min<int>(m.size(), i + 2); ++ii) {
                    for (int jj = std::max(0, nstart - 1); jj <= j; ++jj) {
                        good |= issym(m[ii][jj]);
                        if (m[ii][jj] == '*') {
                            gears[ii * 1000 + jj].push_back(val);
                        }
                    }
                }
                if (good) {
                    res += val;
                }
                val = 0;
                nstart = -1;
            }
        }
    }
    printf("Res part1 = %d\n", res);
    long long res2 = 0;
    for (const auto g : gears) {
        if (g.second.size() == 2) {
            res2 += g.second[0] * g.second[1];
        }
    }
    printf("Res part2 = %lld\n", res2);
}
//-----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        printf("Invalid arguments. Usage: %s day [input_file]", argv[0]);
        return EXIT_FAILURE;
    }

    const std::string day = argv[1];
    const std::string inputFile = argc > 2? argv[2]: "input" + day + ".txt";

    using namespace std::placeholders;
    const std::map<std::string, std::function<void(utils::FileReader&)>> functions = {
        { "1", day1 },
        { "2", day2 },
        { "3", day3 },
    };

    auto start = std::chrono::steady_clock::now();

    try {
        auto reader = utils::FileReader("data/" + inputFile);
        functions.at(day)(reader);
    }
    catch (std::exception &ex) {
        printf("Exception caught: %s\n", ex.what());
    }

    auto finish = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
    printf("Done in %.3fs\n", elapsed);

    return 0;
}
