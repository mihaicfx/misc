#pragma once

#include "utils.h"

void day1(utils::FileReader& reader, int part);
void day2(utils::FileReader& reader, int part);

const std::map<std::string, std::function<void(utils::FileReader&, int)>> functions = {
    { "1", day1 }, { "2", day2 },
};