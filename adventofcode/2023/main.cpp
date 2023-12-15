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
#include <cstdint>

#include "utils.h"

static constexpr int PART = 2;
//-----------------------------------------------------------------------------
void day1(utils::FileReader& reader) {
    char buf[100];
    int res = 0;
    const std::array<std::string, 9> words = {
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };
    while (reader.nextLine() && reader.getLine().read("%99s", buf)) {
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
    int64_t res2 = 0;
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
    std::vector<std::pair<int64_t, int64_t>> seeds;
    reader.nextLine();
    auto line = reader.getLine();
    if (!line.read("seeds: ")) {
        throw utils::MyException("error reading seeds");
    }
    int64_t seed, range;
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
        std::vector<std::array<int64_t, 3>> mp;
        while (reader.nextLine()) {
            int64_t a,b,c;
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
    int64_t res = 1;
    std::vector<int64_t> timedist[2];
    for (auto& td : timedist) {
        int64_t v = 0;
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
        int64_t n = 0;
        for (int t = 1; t < time[i]; ++t) {
            int64_t travel = t * (time[i] - t);
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
    int64_t res = 0;
    std::vector<std::pair<std::string, int>> hands;
    while (reader.nextLine()) {
        char hand[10];
        int bid = 0;
        auto line = reader.getLine();
        if (!line.read("%5s %d", hand, &bid)) {
            throw utils::MyException("error reading line " + std::string{line.get()});
        }
        hands.emplace_back(hand, bid);
    }
    std::sort(hands.begin(), hands.end(), day7_compareHand);
    for (int i = 0; i < hands.size(); ++i) {
        const auto& [h,b] = hands[i];
        res += (i + 1) * b;
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day8(utils::FileReader& reader) {
    int64_t res = 0;
    std::unordered_map<std::string, std::pair<std::string, std::string>> mp;
    reader.nextLine();
    const auto lr = std::string{reader.getLine().get()};
    while (reader.nextLine()) {
        char node[4], left[4], right[4];
        if (reader.getLine().read("%3s = (%3s, %3s)", node, left, right)) {
            mp[node] = std::make_pair<std::string, std::string>(left, right);
        }
    }
    std::vector<const std::string*> starts;
    const auto checkNode = [](const auto& s, char az) {
        return PART == 1? s == std::string(3, az): s[2] == az;
    };
    for (const auto& [k,v] : mp) {
        if (checkNode(k, 'A')) {
            starts.push_back(&k);
        }
    }
    std::vector<int64_t> dist(starts.size(), 0);
    for (int i = 0; i < starts.size(); ++i) {
        const auto* node = starts[i];
        while (!checkNode(*node, 'Z')) {
            for (char c : lr) {
                auto& [l,r] = mp[*node];
                node = c == 'L'? &l: &r;
            }
            dist[i] += 1;
        }
        //printf("%s: reaches %s in %lld steps\n", starts[i]->c_str(), node->c_str(), dist[i]);
    }
    res = 1;
    for (auto d : dist) {
        res *= d / std::gcd(res, d);
    }
    res *= lr.length();

    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day9(utils::FileReader& reader) {
    int64_t res1 = 0, res2 = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        std::vector<int64_t> v;
        int64_t val = 0;
        while (line.read("%lld", &val)) {
            v.push_back(val);
        }
        int j = 0;
        int64_t e1 = 0, e2 = 0;
        while (std::any_of(v.begin(), v.end(), [](auto x) { return x != 0; })) {
            e1 += v.back();
            e2 = e2 + (j % 2? -1: 1) * v[j];
            for (int i = v.size() - 1; i > j; --i) {
                v[i] -= v[i - 1];
            }
            v[j++] = 0;
        }
        PRINT(v, e1, e2);
        res1 += e1;
        res2 += e2;
    }
    printf("Res1 = %lld, Res2 = %lld\n", res1, res2);
}
//-----------------------------------------------------------------------------
void day10(utils::FileReader& reader) {
    auto lines = reader.allLines();
    std::pair<int, int> s{};
    const int n = lines.size();
    const int m = lines[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == 'S') {
                s = {i, j};
            }
        }
    }
    const auto dirs = [](char c) -> std::array<std::pair<int, int>, 2> {
        switch (c) {
            case '|': return {{{-1, 0}, {+1, 0}}}; break;
            case '-': return {{{0, -1}, {0, +1}}}; break;
            case 'L': return {{{-1, 0}, {0, +1}}}; break;
            case 'J': return {{{-1, 0}, {0, -1}}}; break;
            case '7': return {{{0, -1}, {+1, 0}}}; break;
            case 'F': return {{{+1, 0}, {0, +1}}}; break;
        }
        throw utils::MyException("invalid cell type " + std::to_string(c));
    };
    const auto findFirst = [&]() {
        for (const auto [x, y] : utils::DIR4) {
            const std::pair<int, int> pos = {s.first + x, s.second + y};
            for (const auto [dx, dy] : dirs(lines[pos.first][pos.second])) {
                if (lines[pos.first + dx][pos.second + dy] == 'S') {
                    return pos;
                }
            }
        }
        throw utils::MyException("failed to find direction from S");
    };
    std::vector<std::pair<int, int>> path;
    path.push_back(s);
    s = findFirst();
    while (lines[s.first][s.second] != 'S') {
        auto next = dirs(lines[s.first][s.second]);
        for (const auto [dx, dy] : next) {
            const std::pair<int, int> pos = {s.first + dx, s.second + dy};
            if (pos.first >= 0 && pos.first < n && pos.second >= 0 && pos.second < m && path.back() != pos) {
                path.push_back(s);
                s = pos;
                break;
            }
        }
    }
    printf("Res1 = %d\n", path.size() / 2);
    // winding number algorithm
    for (const auto [x, y] : path) {
        lines[x][y] = strchr("|LJ", lines[x][y])? '$': ' ';
    }
    int res2 = 0;
    for (int i = 0; i < n; ++i) {
        for (int k = 0, j = 0; j < m; ++j) {
            if (lines[i][j] == '$') k++;
            else if (lines[i][j] != ' ' && k % 2) res2++;
        }
    }
    printf("Res2 = %d\n", res2);
}
//-----------------------------------------------------------------------------
void day11(utils::FileReader& reader) {
    int64_t res = 0;
    auto lines = reader.allLines();
    const int n = lines.size();
    const int m = lines[0].size();
    std::vector<int> er, ec;
    for (int i = 0; i < n; ++i) {
        if (std::all_of(lines[i].begin(), lines[i].end(), [](char x) { return x == '.'; })) {
            er.push_back(i);
        }
    }
    for (int j = 0; j < m; ++j) {
        if (std::all_of(lines.begin(), lines.end(), [j](const auto& line) { return line[j] == '.'; })) {
            ec.push_back(j);
        }
    }
    std::vector<std::pair<int64_t, int64_t>> nodes;
    constexpr int64_t EXP = 1000000 - 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == '#') {
                int64_t di = i + EXP * (std::lower_bound(er.begin(), er.end(), i) - er.begin());
                int64_t dj = j + EXP * (std::lower_bound(ec.begin(), ec.end(), j) - ec.begin());
                nodes.emplace_back(di, dj);
                //PRINT(i,j, di, dj);
            }
        }
    }
    const int mm = nodes.size();
    for (int i = 0; i < mm; ++i) {
        for (int j = i + 1; j < mm; ++j) {
            int64_t dist = std::abs(nodes[i].first - nodes[j].first) + std::abs(nodes[i].second - nodes[j].second);
            //PRINT(i, j, nodes[i], nodes[j], dist);
            res += dist;
        }
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day12(utils::FileReader& reader) {
    int64_t res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        char buf[200];
        if (!line.read("%s", buf)) {
            throw utils::MyException("failed to read row");
        }
        std::vector<int> vs;
        int v = 0;
        while (line.read("%d", &v)) {
            vs.push_back(v);
            line.read(",");
        }
        if (PART == 2) {
            std::string cp = buf;
            for (int i = 0; i < 4; ++i) {
                strcat(buf, "?");
                strcat(buf, cp.c_str());
            }
            for (int n = vs.size(), i = 0; i < 4 * n; ++i) {
                vs.push_back(vs[i]);
            }
        }
        const int n = strlen(buf);
        const auto match = [buf](int j, int k) {
            return (j == 0 || buf[j - 1] != '#') && // no leading #
                    std::all_of(buf + j, buf + j + k, [](char c) { return c == '?' || c == '#'; }) && // match ? or #
                    buf[j + k] != '#'; // no trailing #
        };
        std::vector<int64_t> dp(n + 2, 0);
        for (int i = dp.size() - 1; i >= 0 && (i >= n || buf[i] != '#'); --i) {
            dp[i] = 1;
        }
        for (int i = vs.size() - 1; i >= 0; --i) {
            std::vector<int64_t> dp2(n + 2, 0);
            int64_t a = 0;
            for (int j = n - vs[i]; j >= 0; --j) {
                if (buf[j] == '#') {
                    a = 0;
                }
                if (match(j, vs[i])) {
                    a += dp[j + vs[i] + 1];
                }
                dp2[j] = a;
            }
            std::swap(dp2, dp);
        }
        res += dp[0];
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day13(utils::FileReader& reader) {
    int64_t res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day14(utils::FileReader& reader) {
    int64_t res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
const std::map<std::string, std::function<void(utils::FileReader&)>> functions = {
    { "1", day1 }, { "2", day2 }, { "3", day3 }, { "4", day4 }, { "5", day5 },
    { "6", day6 }, { "7", day7 }, { "8", day8 }, { "9", day9 }, { "10", day10 },
    { "11", day11 }, { "12", day12 }, { "13", day13 }, { "14", day14 },
};

int main(int argc, char **argv)
{
    if (argc > 3) {
        printf("Invalid arguments. Usage: %s [day [input_file]]", argv[0]);
        return EXIT_FAILURE;
    }

    const std::string day = argc > 1? argv[1]: "13";
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
