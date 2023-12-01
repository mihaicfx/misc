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

void day1(FILE* f) {
    static constexpr int PART = 2;
    char buf[100];
    int res = 0;
    const std::array<std::string, 9> words = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    while (fscanf(f, "%99s", buf) == 1) {
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

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        printf("Invalid arguments. Usage: %s day [input_file]", argv[0]);
        return EXIT_FAILURE;
    }

    const std::string day = argv[1];
    const std::string inputFile = argc > 2? argv[2]: "input" + day + ".txt";

    using namespace std::placeholders;
    const std::map<std::string, std::function<void(FILE*)>> functions = {
        { "1", day1 },
    };

    auto start = std::chrono::steady_clock::now();

    try {
        FILE* f = utils::checkedFopen("data/" + inputFile);
        functions.at(day)(f);
        fclose(f);
    }
    catch (std::exception &ex) {
        printf("Exception caught: %s\n", ex.what());
    }

    auto finish = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
    printf("Done in %.3fs\n", elapsed);

    return 0;
}
