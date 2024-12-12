/*
 * adventofcode.cpp
 *
 * Author: Mihai Feier
 * Copyright QTronic GmbH. All rights reserved.
 */

#include <string>
#include <chrono>
#include <exception>

#include "days.h"
#include "utils.h"

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 4) {
        printf("Invalid arguments. Usage: %s day [part [input_file]]", argv[0]);
        return EXIT_FAILURE;
    }

    std::string day = argv[1];
    const int part = argc > 2? atoi(argv[2]): 3;
    std::string inputFile = argc > 3? argv[3]: "";

    auto start = std::chrono::steady_clock::now();

    try {
        if (day == "today") {
            const time_t now = time(0);
            const int day_of_month = localtime(&now)->tm_mday;
            day = std::to_string(day_of_month);
        }
        if (day == "all") {
            for (auto [day, func] : functions) {
                printf("day %s:\n", day.c_str());
                for (int part : {1, 2}) {
                    auto reader = utils::FileReader("data/input" + day + ".txt");
                    func(reader, part);
                }
            }
        } else {
            if (inputFile.empty()) {
                inputFile = "data/input" + day + ".txt";
            }
            if (!std::filesystem::exists(inputFile)) {
                if (FILE* f = fopen(inputFile.c_str(), "a"); f) {
                    fclose(f);
                }
            }
            for (int p : {1, 2}) {
                if (part & p) {
                    auto reader = utils::FileReader(inputFile);
                    functions.at(day)(reader, p);
                }
            }
        }
    }
    catch (std::exception &ex) {
        printf("Exception caught: %s\n", ex.what());
    }

    auto finish = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
    printf("Day %s part %d: Done in %.3fs\n", day.c_str(), part, elapsed);

    return 0;
}
