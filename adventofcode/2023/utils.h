#pragma once

#include <exception>
#include <string>
#include <ctime>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <thread>
#include <optional>

namespace utils
{

// adds string constructor capability to std::exception
class MyException : public std::runtime_error
{
public:
    MyException(const std::string &msg) : std::runtime_error(msg.c_str()) {}
};

struct FileReader {
    struct LineReader {
        LineReader(const std::string& _line) : line(_line) {}
        const std::string& get() { return line; }

        template<typename... Args>
        bool read(const char* fmt, Args... args);

     private:
        const std::string& line;
        int pos = 0;
    };

    FileReader(const std::filesystem::path& fileName);
    ~FileReader();

    bool nextLine();
    LineReader getLine();
    std::vector<std::string> allLines();

    FILE* f;
    std::string line;
};

inline std::vector<std::pair<int, int>> splitWork(int size);
int digits(int n);

//-----------------------------------------------------------------------------

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

std::vector<std::string> FileReader::allLines() {
    std::vector<std::string> lines;
    while (nextLine()) {
        getLine();
        lines.emplace_back(std::move(line));
    }
    return lines;
}

FileReader::LineReader FileReader::getLine() {
    return {line};
}

template<typename... Args>
bool FileReader::LineReader::read(const char* fmt, Args... args) {
    int n = -1;
    const std::string format = std::string(fmt) + "%n";
    if (sscanf(line.data() + pos, format.c_str(), args..., &n) == sizeof...(args) && n != -1) {
        pos += n;
        return true;
    }
    return {};
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

int digits(int n) {
    int res = 0;
    while (n > 0) res++, n /= 10;
    return res;
}

} // namespace utils