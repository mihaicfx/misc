#include "utils.h"

#include <thread>

namespace utils
{

FileReader::FileReader(const std::filesystem::path& fileName) {
    f = fopen(fileName.string().c_str(), "rt");
    if (!f) {
        throw MyException("error opening file " + fileName.string());
    }
}

FileReader::~FileReader() {
    if (f) fclose(f);
}

bool FileReader::nextLine() {
    line.clear();
    char buf[256];
    bool read = false;
    while (fgets(buf, std::size(buf), f)) {
        read = true;
        line += buf;
        if (line.back() == '\n') {
            while (!line.empty() && isspace(line.back())) {
                line.pop_back();
            }
            break;
        }
    }
    return read;
}

std::tuple<std::vector<std::string>, int, int> FileReader::allLines() {
    int m = 0;
    std::vector<std::string> lines;
    while (nextLine()) {
        getLine();
        if (m != line.size()) {
            m = !m? line.size(): -1;
        }
        lines.emplace_back(std::move(line));
    }
    return {lines, static_cast<int>(lines.size()), m};
}

FileReader::LineReader FileReader::getLine() {
    return {line};
}

std::vector<std::string_view> FileReader::LineReader::split(char delim) {
    std::vector<std::string_view> res;
    auto i = pos;
    for (; i < line.length(); ++i) {
        if (line[i] == delim) {
            if (pos != i) {
                res.emplace_back(line.substr(pos, i - pos));
            }
            pos = i + 1;
        }
    }
    if (pos != i) { // last
        res.emplace_back(line.substr(pos, i - pos));
        pos = i;
    }
    return res;
}


int StringToIdMap::set(std::string s) {
    auto it = fwd.find(s);
    if (it == fwd.end()) {
        it = fwd.insert(it, {s, static_cast<int>(fwd.size() + 1)});
    }
    bwd.insert({it->second, std::move(s)});
    return it->second;
}

const std::string& StringToIdMap::get(int id) {
    return bwd.at(id);
}


std::vector<std::pair<int, int>> splitWork(int size)
{
    int n = std::min(size, (int)std::thread::hardware_concurrency());
    int chunksSize = size / n;
    int remainder = size % n;
    std::vector<std::pair<int, int>> result;
    int start = 0;
    for (int i = 0; i < n; i++) {
        int end = start + chunksSize + (i < remainder);
        result.push_back({start, end});
        start = end;
    }
    return result;
}

void runWorkParallel(int size, std::function<void(int start, int end)> worker) {
    auto split = utils::splitWork(size);
    std::vector<std::thread> threads;
    threads.reserve(split.size());
    for (const auto [start, end] : split) {
        threads.push_back(std::thread(worker, start, end));
    }
    for (auto& t : threads) {
        t.join();
    }
}

int digits(long long n) {
    int res = 0;
    while (n > 0) res++, n /= 10;
    return res;
}

namespace detail
{

void printNextName(std::ostream& os, const char*& p) {
    // note: should it handle quotes?
    if (*p) {
        while (isspace(*p)) {
            ++p;
        }
        const char* start = p;
        int paren = 0;
        while (*p) {
            if (!paren && *p == ',') {
                break;
            } else if (strchr("([{", *p)) {
                ++paren;
            } else if (strchr(")]}", *p)) {
                --paren;
            }
            ++p;
        }
        const char* end = p;
        while (end > start && isspace(end[-1])) {
            --end;
        }
        os << std::string_view(start, end - start) << " = ";
        if (*p) {
            ++p;
        }
    }
}

} // namespace detail

} // namespace utils