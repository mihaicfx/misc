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
#include <numeric>

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
        while (line.read("%d %9[a-z]", &value, type)) {
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
void day4(utils::FileReader& reader) {
    int res = 0;
    std::array<int, 215> cards{};
    while (reader.nextLine()) {
        auto line = reader.getLine();
        int card = 0;
        if (!line.read("Card %d: ", &card)) {
            throw utils::MyException("error reading card");
        }
        int value = 0;
        std::set<int> win;
        while (line.read("%d", &value)) {
            win.insert(value);
        }
        line.read(" | ");
        int n = 0;
        while (line.read("%d", &value)) {
            n += win.count(value) != 0;
        }
        if (n) res += (1 << (n - 1));
        cards[card]++;
        for (int i = 1; i <= n; ++i) {
            cards[card + i] += cards[card];
        }
    }
    printf("Res1 = %d\n", res);
    int res2 = 0;
    for (int i = 0; i < std::size(cards); ++i) {
        res2 += cards[i];
    }
    printf("Res2 = %d\n", res2);
}
//-----------------------------------------------------------------------------
void day5(utils::FileReader& reader) {
    static constexpr int PART = 2;
    std::vector<std::pair<long long, long long>> seeds;
    reader.nextLine();
    auto line = reader.getLine();
    if (!line.read("seeds: ")) {
        throw utils::MyException("error reading seeds");
    }
    long long seed, range;
    if (PART == 1) {
        while (line.read("%lld", &seed)) {
            seeds.emplace_back(seed, seed + 1);
        }
    } else {
        while (line.read("%lld %lld", &seed, &range)) {
            seeds.emplace_back(seed, seed + range);
        }
    }
    reader.nextLine();
    while (reader.nextLine()) {
        std::vector<std::array<long long, 3>> mp;
        while (reader.nextLine()) {
            long long a,b,c;
            if (reader.getLine().read("%lld %lld %lld", &a, &b, &c)) {
                mp.push_back(std::array{a, b, c});
            } else break;
        }
        decltype(seeds) nseeds{};
        for (int i = 0; i < seeds.size(); ++i) {
            auto [ss, se] = seeds[i];
            bool keep = true;
            for (auto [md, ms, mn] : mp) {
                if (ss < ms && se > ms) {
                    seeds.emplace_back(ss, ms);
                    ss = ms;
                    keep = false;
                }
                if (ss < ms + mn && se > ms + mn) {
                    nseeds.emplace_back(ms + mn, se);
                    se = ms + mn;
                    keep = false;
                }
                if (se != ss && ss >= ms && se <= ms + mn) {
                    nseeds.emplace_back(ss + (md - ms), se + (md - ms));
                    keep = false;
                }
            }
            if (keep) {
                nseeds.emplace_back(ss, se);
            }
        }
        std::swap(seeds, nseeds);
    }
    printf("Res = %lld\n", std::min_element(seeds.begin(), seeds.end())->first);
}
//-----------------------------------------------------------------------------
void day6(utils::FileReader& reader) {
    static constexpr int PART = 2;
    long long res = 1;
    std::vector<long long> timedist[2];
    for (auto& td : timedist) {
        long long v = 0;
        reader.nextLine();
        auto line1 = reader.getLine();
        line1.read("%*s");
        while (line1.read("%lld", &v)) {
            if (PART == 1 || td.empty()) {
                td.push_back(v);
            } else {
                td.back() = td.back() * std::pow(10, utils::digits(v)) + v;
            }
        }
    }
    const auto& [time, dist] = timedist;
    for (int i = 0; i < time.size(); ++i) {
        long long n = 0;
        for (int t = 1; t < time[i]; ++t) {
            long long travel = t * (time[i] - t);
            n += travel > dist[i];
        }
        res *= n;
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
int day7_score(std::string h, bool part2=false) {
    std::sort(h.begin(), h.end());
    int s = 0, n = 1;
    int j = 0, mn = 0;
    for (int i = 0; i < h.length(); ++i) {
        if (part2 && h[i] == 'J') j++;
        else {
            n = i && h[i] == h[i - 1]? n + 1: 1;
            s += n;
            mn = std::max(mn, n);
        }
    }
    while (j--) s += ++mn;
    return s;
}

bool day7_compareHand(std::pair<std::string, int>& hand1, std::pair<std::string, int>& hand2) {
    static constexpr int PART = 2;
    static std::array rv = {'A', 'K', 'Q', PART == 1? 'J': '-', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};
    int h1 = day7_score(hand1.first, PART == 2);
    int h2 = day7_score(hand2.first, PART == 2);
    if (h1 != h2) return h1 < h2;
    for (int i = 0; i < hand1.first.length(); ++i) {
        auto i1 = std::find(rv.begin(), rv.end(), hand1.first[i]);
        auto i2 = std::find(rv.begin(), rv.end(), hand2.first[i]);
        if (i1 != i2) return i1 > i2;
    }
    return false;
}

void day7(utils::FileReader& reader) {
    long long res = 0;
    std::vector<std::pair<std::string, int>> hands;
    while (reader.nextLine()) {
        char hand[10];
        int bid = 0;
        auto line = reader.getLine();
        if (!line.read("%5s %d", hand, &bid)) {
            throw utils::MyException("error reading line " + line.get());
        }
        hands.emplace_back(hand, bid);
    }
    std::sort(hands.begin(), hands.end(), day7_compareHand);
    for (int i = 0; i < hands.size(); ++i) {
        const auto& [h,b] = hands[i];
        res += (i + 1) * b;
        //printf("%s %d = %d\n", h.c_str(), b, day7_score2(h));
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day8(utils::FileReader& reader) {
    long long res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
const std::map<std::string, std::function<void(utils::FileReader&)>> functions = {
    { "1", day1 }, { "2", day2 }, { "3", day3 }, { "4", day4 }, { "5", day5 },
    { "6", day6 }, { "7", day7 }, { "8", day8 },
};

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        printf("Invalid arguments. Usage: %s day [input_file]", argv[0]);
        return EXIT_FAILURE;
    }

    const std::string day = argv[1];
    const std::string inputFile = argc > 2? argv[2]: "input" + day + ".txt";

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
