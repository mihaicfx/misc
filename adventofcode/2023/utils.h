#pragma once

#include <exception>
#include <string>
#include <ctime>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <thread>

namespace utils
{

// adds string constructor capability to std::exception
class MyException : public std::runtime_error
{
public:
    MyException(const std::string &msg) : std::runtime_error(msg.c_str()) {}
};


// progress reporter for large tasks
class ProgressReport
{
    std::clock_t start;
    std::clock_t lastReport;
    std::clock_t lastEstimate;
    float intervalReport;
    float intervalEstimate;

    inline float difft(std::clock_t start, std::clock_t end);

public:
    inline ProgressReport(float interval = 5.0);
    inline void tick(int progress, int end);
};


// helper class
struct StringToIdMap {
    std::unordered_map<std::string, int> fwd;
    std::unordered_map<int, const std::string*> bwd;

    int set(std::string s);
    const std::string& get(int id);
};


inline std::vector<std::pair<int, int>> splitWork(int size);

inline FILE* checkedFopen(const std::filesystem::path& fileName);
inline int readInt(FILE *f, const std::string& what="?");
inline std::string readString(FILE *f, int maxLen, const std::string& what="?");
inline void printInt(FILE *f, int value, bool nl=false);
inline void printString(FILE *f, const std::string& what, bool nl=false);

//-----------------------------------------------------------------------------

float ProgressReport::difft(std::clock_t start, std::clock_t end)
{
    return (end - start) / (float)CLOCKS_PER_SEC;
}

ProgressReport::ProgressReport(float interval) {
    this->start = std::clock();
    this->lastReport = this->start;
    this->lastEstimate = this->start;
    this->intervalReport = interval;
    this->intervalEstimate = 10 * interval;
}

void ProgressReport::tick(int progress, int end) {
    std::clock_t now = std::clock();
    if (this->difft(this->lastReport, now) >= this->intervalReport) {
        printf("~%d%%", int(100ll * progress / end));
        this->lastReport = now;
    }
    if (this->difft(this->lastEstimate, now) >= this->intervalEstimate) {
        float elapsed = this->difft(this->start, now);
        int remaining = int((end - progress) * (elapsed / progress));
        printf("E:%dm%02ds\n", remaining / 60, remaining % 60);
        this->lastEstimate = now;
    }
}



FILE* checkedFopen(const std::filesystem::path& fileName) {
    FILE* f = fopen(fileName.string().c_str(), "rt");
    if (!f) {
        throw MyException("error opening file " + fileName.string());
    }
    return f;
}

int readInt(FILE *f, const std::string& what) {
    int x;
    if (fscanf(f, "%d", &x) != 1) {
        throw MyException("error reading int " + what);
    }
    return x;
}

std::string readString(FILE *f, int maxLen, const std::string& what) {
    std::string buf(maxLen + 1, '\0');
    const std::string format = "%" + std::to_string(maxLen) + "s";
    if (fscanf(f, format.c_str(), buf.data()) != 1) {
        throw MyException("error reading string " + what);
    }
    return buf;
}

void printInt(FILE *f, int value, bool nl) {
    fprintf(f, "%d", value);
    if (nl) fprintf(f, "\n");
}

void printString(FILE *f, const std::string& what, bool nl) {
    fprintf(f, "%s", what.c_str());
    if (nl) fprintf(f, "\n");
}


int StringToIdMap::set(std::string s) {
    auto it = fwd.find(s);
    if (it == fwd.end()) {
        it = fwd.insert(it, {std::move(s), fwd.size() + 1});
    }
    bwd.insert({it->second, &it->first});
    return it->second;
}

const std::string& StringToIdMap::get(int id) {
    return *bwd.at(id);
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

} // namespace utils