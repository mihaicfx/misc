#pragma once

#include "utils.h"

inline std::map<std::string, std::function<void(utils::FileReader&, int)>> functions = {};

#define DAY(n, ...) \
void day ## n(__VA_ARGS__); \
static const auto _day ## n ## _ = []() { \
    functions[#n] = day ## n;\
    return 0;\
}(); \
void day ## n(__VA_ARGS__)