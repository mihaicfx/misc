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
#include <unordered_set>
#include <unordered_map>
#include <thread>

#include "utils.h"

namespace { // empty namespace for folding older problems
//-----------------------------------------------------------------------------
void day1(utils::FileReader& reader, int part) {
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
            if (part == 2) {
                for (int k = 0; k < words.size(); ++k) {
                    if (strncmp(buf + i, words[k].c_str(), words[k].length()) == 0) {
                        val = k + 1;
                    }
                }
            }
            if (val != -1) (a == -1? a: b) = val;
        }
        //printf("%s = %d/%d\n", buf, a, (b == -1? a: b));
        res += a * 10 + (b == -1? a: b);
    }
    printf("Res = %d\n", res);
}
//-----------------------------------------------------------------------------
void day2(utils::FileReader& reader, int part) {
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
                if (part == 1) {
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
        if (part == 1) {
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
void day3(utils::FileReader& reader, int part) {
    if (part != 1) return; // solves both parts
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
void day4(utils::FileReader& reader, int part) {
    if (part != 1) return; // solves both parts
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
void day5(utils::FileReader& reader, int part) {
    std::vector<std::pair<int64_t, int64_t>> seeds;
    reader.nextLine();
    auto line = reader.getLine();
    if (!line.read("seeds: ")) {
        throw utils::MyException("error reading seeds");
    }
    int64_t seed, range;
    if (part == 1) {
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
void day6(utils::FileReader& reader, int part) {
    int64_t res = 1;
    std::vector<int64_t> timedist[2];
    for (auto& td : timedist) {
        int64_t v = 0;
        reader.nextLine();
        auto line1 = reader.getLine();
        line1.read("%*s");
        while (line1.read("%lld", &v)) {
            if (part == 1 || td.empty()) {
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

bool day7_compareHand(const std::pair<std::string, int>& hand1, const std::pair<std::string, int>& hand2, int part) {
    static std::array rv = {'A', 'K', 'Q', part == 1? 'J': '-', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};
    int h1 = day7_score(hand1.first, part == 2);
    int h2 = day7_score(hand2.first, part == 2);
    if (h1 != h2) return h1 < h2;
    for (int i = 0; i < hand1.first.length(); ++i) {
        auto i1 = std::find(rv.begin(), rv.end(), hand1.first[i]);
        auto i2 = std::find(rv.begin(), rv.end(), hand2.first[i]);
        if (i1 != i2) return i1 > i2;
    }
    return false;
}

void day7(utils::FileReader& reader, int part) {
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
    std::sort(hands.begin(), hands.end(), [part](auto& h1, auto& h2) { return day7_compareHand(h1, h2, part); });
    for (int i = 0; i < hands.size(); ++i) {
        const auto& [h,b] = hands[i];
        res += (i + 1) * b;
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day8(utils::FileReader& reader, int part) {
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
    const auto checkNode = [part](const auto& s, char az) {
        return part == 1? s == std::string(3, az): s[2] == az;
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
void day9(utils::FileReader& reader, int part) {
    if (part != 1) return; // solves both parts
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
        //PRINT(v, e1, e2);
        res1 += e1;
        res2 += e2;
    }
    printf("Res1 = %lld, Res2 = %lld\n", res1, res2);
}
//-----------------------------------------------------------------------------
void day10(utils::FileReader& reader, int part) {
    if (part != 1) return; // solves both parts
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
        for (const auto [x, y] : utils::dir::UDLR) {
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
void day11(utils::FileReader& reader, int part) {
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
    const int64_t EXP = part == 1? 2: 1000000 - 1;
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
void day12(utils::FileReader& reader, int part) {
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
        if (part == 2) {
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
void day13(utils::FileReader& reader, int part) {
    int64_t res = 0;
    auto lines = reader.allLines();
    int s = 0;
    for (int i = 0; i <= lines.size(); ++i) {
        if (i == lines.size() || lines[i].empty()) {
            // check column mirror
            for (int k = 1; k < lines[s].length() ; ++k) {
                int diff = 0;
                for (int j = s; j < i; ++j) {
                    for (int d = 0; k - d - 1 >= 0 && k + d < lines[s].length(); ++d) {
                        diff += lines[j][k - d - 1] != lines[j][k + d];
                    }
                }
                if (diff == part - 1) {
                    res += k;
                    break;
                }
            }
            // check row mirror
            for (int k = s + 1; k < i; ++k) {
                int diff = 0;
                for (int j = 0; j < lines[s].length(); ++j) {
                    for (int d = 0; k - d - 1 >= s && k + d < i; ++d) {
                        diff += lines[k - d - 1][j] != lines[k + d][j];
                    }
                }
                if (diff == part - 1) {
                    res += (k - s) * 100;
                    break;
                }
            }
            s = i + 1;
        }
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day14_roll(int n, int m, const std::function<char&(int,int)>& getter) {
    for (int j = 0; j < m; ++j) {
        int p = 0;
        for (int i = 0; i < n; ++i) {
            switch (getter(i,j)) {
                case 'O': std::swap(getter(p,j), getter(i,j)); p++; break;
                case '#': p = i + 1; break;
            }
        }
    }
}

void day14_roll4(std::vector<std::string>& lines, int n, int m) {
    day14_roll(n, m, [&](int i, int j) -> char& { return lines[i][j]; });
    day14_roll(m, n, [&](int i, int j) -> char& { return lines[j][i]; });
    day14_roll(n, m, [&](int i, int j) -> char& { return lines[n - i - 1][j]; });
    day14_roll(m, n, [&](int i, int j) -> char& { return lines[j][n - i - 1]; });
}

void day14(utils::FileReader& reader, int part) {
    int64_t res = 0;
    auto lines = reader.allLines();
    const int n = lines.size();
    const int m = lines[0].size();
    if (part == 1) {
        day14_roll(n, m, [&](int i, int j) -> char& { return lines[i][j]; });
    } else {
        // generic tortoise and hare algorithm revealed that the cycle starts very soon (< 100 iterations), so we can brute check
        auto lines1 = lines;
        std::vector<std::vector<std::string>> alllines;
        std::pair<int, int> found = {-1,-1};
        for (int i = 0; found.first == -1; ++i) {
            day14_roll4(lines1, n, m);
            for (int j = 0; j < i; ++j) {
                if (lines1 == alllines[j]) {
                    found = {j + 1, i + 1};
                    break;
                }
            }
            alllines.push_back(lines1);
        }
        // PRINT(found);
        for (int i = 0; i < found.first + ((1'000'000'000 - found.second) % (found.second - found.first)); ++i) {
            day14_roll4(lines, n, m);
        }
    }
    // score
    for (int j = 0; j < lines[0].size(); ++j) {
        for (int i = 0; i < n; ++i) {
            if (lines[i][j] == 'O') {
                res += (n - i);
            }
        }
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day15(utils::FileReader& reader, int part) {
    if (part != 1) return; // solves both parts
    int64_t res[2] = {};
    if (!reader.nextLine()) {
        throw utils::MyException("failed to read file");
    }
    std::array<std::vector<std::pair<std::string, int>>, 256> mp;
    auto line = reader.getLine();
    for (auto str : line.split(',')) {
        int h1 = 0, h2 = 0, k = 0;
        for (int i = 0; i < str.length(); ++i) {
            h1 = ((h1 + str[i]) * 17) % 256;
            if (isalpha(str[i])) {
                h2 = h1;
                k = i + 1;
            }
        }
        res[0] += h1;
        // part 2
        auto s2 = str.substr(0, k);
        auto it = std::find_if(mp[h2].begin(), mp[h2].end(), [&s2](auto& kv) { return kv.first == s2;} );
        if (str[k] == '=') {
            const int val = str[k + 1] - '0';
            if (it != mp[h2].end()) {
                it->second = val;
            } else {
                mp[h2].emplace_back(s2, val);
            }
        } else if (str[k] == '-' && it != mp[h2].end()) {
            mp[h2].erase(it);
        }
    }
    for (int i = 0; i < mp.size(); ++i) {
        for (int j = 0; j < mp[i].size(); ++j) {
            res[1] += (i + 1) * (j + 1) * mp[i][j].second;
        }
    }
    printf("Res1 = %lld\n", res[0]);
    printf("Res2 = %lld\n", res[1]);
}
//-----------------------------------------------------------------------------
int day16_go(std::vector<std::string> lines, int n, int m, const utils::Coord start, const utils::Dir dir) {
    std::queue<std::pair<utils::Coord, utils::Dir>> q;
    std::unordered_set<std::pair<utils::Coord, utils::Dir>> vis(m * n);
    std::unordered_set<utils::Coord> locations(m * n);
    q.push({start, dir});
    while (!q.empty()) { // bfs
        auto [p, d] = q.front();
        q.pop();
        if (vis.count({p, d}) != 0) {
            continue;
        }
        vis.insert({p, d});
        locations.insert(p);
        const auto np = p + d;
        if (np.i >= 0 && np.i < n && np.j >= 0 && np.j < m) {
            switch (lines[np.i][np.j]) {
                case '.' : break; // go on
                case '|' :
                    if (d.i == 0) { // split
                        q.push({np, utils::dir::U});
                        d = utils::dir::D;
                    } // else go on
                    break;
                case '-' :
                    if (d.j == 0) { // split
                        q.push({np, utils::dir::L});
                        d = utils::dir::R;
                    } // else go on
                    break;
                case '/' :
                    d = utils::Dir{-d.j, -d.i};
                    break;
                case '\\' :
                    d = utils::Dir{d.j, d.i};
                    break;
            }
            q.push({np, d});
        }
    }
    return locations.size() - 1;
}

void day16(utils::FileReader& reader, int part) {
    int res = 0;
    auto lines = reader.allLines();
    const int n = lines.size();
    const int m = lines[0].size();
    if (part == 1) {
        res = day16_go(lines, n, m, {0,-1}, utils::dir::R);
    } else {
        std::vector<std::pair<utils::Coord, utils::Dir>> vs;
        for (int i = 0; i < n; ++i) {
            vs.push_back({{i,-1}, utils::dir::R});
            vs.push_back({{i, m}, utils::dir::L});
        }
        for (int j = 0; j < m; ++j) {
            vs.push_back({{-1, j}, utils::dir::D});
            vs.push_back({{ n, j}, utils::dir::U});
        }
        std::vector<int> results(vs.size(), 0);
        auto worker = [&lines, n, m, &vs, &results](int start, int end) {
            for (int i = start; i < end; ++i) {
                const auto& [coord, dir] = vs[i];
                results[i] = day16_go(lines, n, m, coord, dir);
            }
        };
        utils::runWorkParallel(vs.size(), worker);
        res = *std::max_element(results.begin(), results.end());
    }
    printf("Res = %d\n", res);
}
//-----------------------------------------------------------------------------
void day17(utils::FileReader& reader, int part) {
    int64_t res = 0;
    auto lines = reader.allLines();
    const int n = lines.size();
    const int m = lines[0].size();
    using T3 = std::tuple<int, utils::Coord, utils::Dir>;
    std::priority_queue<T3, std::vector<T3>, std::greater<T3>> q;
    std::unordered_map<std::pair<utils::Coord, utils::Dir>, int> vis;
    const int d0 = lines[0][0] - '0';
    q.push({-d0, {-1, 0}, {utils::dir::R}});
    q.push({-d0, {0, -1}, {utils::dir::D}});
    const utils::Coord dest = {n - 1, m - 1};
    const auto [ks, ke] = (part == 1)? std::pair{1, 3}: std::pair{4, 10};
    while (!q.empty()) { // dijkstra
        auto [dist, pos, dir] = q.top();
        q.pop();
        if (pos == dest) {
            res = dist;
            break;
        }
        if (auto it = vis.find({pos, dir}); it != vis.end() && dist > it->second) {
            continue;
        }
        for (auto nd : {utils::Dir{-dir.j, -dir.i}, utils::Dir{dir.j, dir.i}}) {
            auto np = pos;
            int cd = 0;
            for (int k = 1; k <= ke; ++k) {
                np = np + nd;
                if (!(np.i >= 0 && np.i < n && np.j >= 0 && np.j < m)) {
                    continue;
                }
                cd += lines[np.i][np.j] - '0';
                if (k < ks) {
                    continue;
                }
                if (auto it = vis.find({np, nd}); it == vis.end() || dist + cd < it->second) {
                    q.push({dist + cd, np, nd});
                    if (it == vis.end()) {
                        vis.insert({{np, nd}, dist + cd});
                    } else {
                        it->second = dist + cd;
                    }
                }
            }
        }
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day18(utils::FileReader& reader, int part) {
    int64_t res = 0;
    std::vector<std::tuple<utils::Coord, utils::Dir, int>> path;
    const std::array intToDir = { utils::dir::R, utils::dir::D, utils::dir::L, utils::dir::U};
    utils::Coord pos = {0, 0};
    int minj = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        char dirCode = 0;
        int steps = 0, color = 0;
        if (!line.read("%c %d (#%x)", &dirCode, &steps, &color)) {
            throw EXCEPTION("failed to read line: ", line.get());
        }
        steps = part == 1? steps: color / 16;
        utils::Dir dir = part == 1? utils::dir::char2Dir.at(dirCode): intToDir[color % 4];
        path.push_back({pos, dir, steps});
        pos = pos + dir * steps;
        minj = std::min(minj, pos.j);
    }
    int64_t area = 0, length = 0;
    for (auto& [pos, dir, steps] : path) {
        area += static_cast<int64_t>(dir.i) * steps * (pos.j - minj);
        length += steps;
    }
    res = std::abs(area) + length/2 + 1;
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day19(utils::FileReader& reader, int part) {
    int64_t res = 0;
    using Rule = std::tuple<char, char, int, std::string>;
    std::map<std::string, std::vector<Rule>> rules;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        char rule[10];
        if (!line.read("%9[a-z]{", rule)) {
            break;
        }
        char part = 0, op = 0;
        int val = 0;
        char next[10] = {};
        auto& work = rules[rule];
        while (line.read("%c%c%d:%9[a-zAR],", &part, &op, &val, next)) {
            work.push_back({part, op, val, next});
        }
        line.read("%9[a-zAR]}", next);
        work.push_back({0, 0, 0, next});
    }
    std::stack<std::pair<std::string, std::array<std::pair<int, int>, 4>>> st;
    if (part == 1) { // rewritten to use part 2 algorithm. previously was direct solution
        int x, m, a, s;
        while (reader.nextLine() && reader.getLine().read("{x=%d,m=%d,a=%d,s=%d}", &x, &m, &a, &s)) {
            st.push({"in", {{{x,x}, {m,m}, {a,a}, {s,s}}}});
        }
    } else {
        const std::pair<int, int> range{1, 4000};
        st.push({"in", {{range, range, range, range}}});
    }
    while (!st.empty()) { // DFS
        auto [next_rule, ranges] = st.top();
        st.pop();
        if (next_rule == "A") {
            if (part == 1) {
                res += std::accumulate(ranges.begin(), ranges.end(), 0ll,
                    [](int64_t x, auto& range) { return x + range.first; });
            } else {
                res += std::accumulate(ranges.begin(), ranges.end(), 1ll,
                    [](int64_t x, auto& range) { return x * (range.second - range.first + 1); });
            }
        } else if (next_rule != "R") {
            auto& vrule = rules.at(next_rule);
            for (auto& [part, op, val, next] : vrule) {
                if (part != 0) {
                    const int idx = part == 'x'? 0: part == 'm'? 1: part == 'a'? 2: 3;
                    if (op == '<' && ranges[idx].first < val) {
                        auto new_ranges = ranges;
                        new_ranges[idx].second = std::min(new_ranges[idx].second, val - 1);
                        st.push({next, new_ranges});
                        ranges[idx].first = val;
                    } else if (op == '>' && ranges[idx].second > val) {
                        auto new_ranges = ranges;
                        new_ranges[idx].first = std::max(new_ranges[idx].first, val + 1);
                        st.push({next, new_ranges});
                        ranges[idx].second = val;
                    }
                    if (ranges[idx].first > ranges[idx].second) {
                        break;
                    }
                } else {
                    st.push({next, ranges});
                }
            }
        }
    }
    printf("Res = %lld\n", res);
}
} // empty namespace for folding
//-----------------------------------------------------------------------------
void day20(utils::FileReader& reader, int part) {
    utils::StringToIdMap sid;
    std::array<std::pair<char, std::vector<int>>, 64> mp = {}; // {type, receivers}
    std::array<int64_t, 64> vals = {}, masks = {};
    std::array<int, 64> sends[2] = {};
    while (reader.nextLine()) {
        auto line = reader.getLine();
        char but[16];
        if (!line.read("%16s -> ", but)) throw utils::MyException("failed to read " + std::string{line.get()});
        const int id = sid.set(but + 1);
        auto& [type, dest] = mp[id];
        type = but[0];
        while (line.read("%16[a-z]", but)) {
            line.read(", ");
            dest.push_back(sid.set(but));
            masks[sid.get(but)] |= 1ll << id;
        }
    }
    int64_t pulses[2] = {};
    auto push = [&](int time) {
        std::queue<std::tuple<int, int, bool>> q;
        q.push({sid.get("roadcaster"), 0, false}); // b has been taken as type
        while (!q.empty()) {
            const auto [b, src, pulse] = q.front();
            q.pop();
            pulses[pulse]++;
            const auto& [type, dest] = mp[b];
            auto& val = vals[b];
            bool send = pulse;
            if (type == '%') {
                if (pulse) {
                    continue;
                }
                send = val = !val;
            } else if (type == '&') {
                val = (val & ~(1ll << src)) | (static_cast<int64_t>(pulse) << src);
                send = val != masks[b];
            }
            for (auto d : dest) {
                q.push({d, b, send});
            }
            sends[send][b] = time;
        }
    };
    if (part == 1) {
        for (int i = 0; i < 1000; ++i) {
            push(i);
        }
        printf("Res1 = %lld\n", pulses[0] * pulses[1]);
    } else if (part == 2) {
        // based on observation of the target, we can solve this by finding the last group of inputs that contribute to rx
        std::vector<int> targets{sid.get("rx")};
        while (targets.size() == 1) {
            const int tid = targets.back();
            targets.pop_back();
            for (int i = 0; i < mp.size(); ++i) {
                if (std::find(mp[i].second.begin(), mp[i].second.end(), tid) != mp[i].second.end()) {
                    targets.push_back(i);
                }
            }
        }
        std::array<int, 64> loops{};
        for (int i = 1; std::any_of(targets.begin(), targets.end(), [&loops](int id) { return loops[id] == 0;}); ++i) {
            push(i);
            for (int j = 1; j <= loops.size(); ++j) {
                if (loops[j] == 0 && sends[1][j] == i) {
                    loops[j] = i;
                }
            }
        }
        int64_t res = std::accumulate(targets.begin(), targets.end(), 1ll, [&loops](int64_t r, int id) { return r * loops[id];});
        printf("Res2 = %lld\n", res);
    }
}
//-----------------------------------------------------------------------------
void day21(utils::FileReader& reader, int part) {
    int64_t res = 0;
    auto lines = reader.allLines();
    const int n = lines.size();
    const int m = lines[0].size();
    utils::Coord start = [&]() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == 'S') {
                    return utils::Coord{i, j};
            }
        }
    }
        return utils::Coord{};
    }();
    lines[start.i][start.j] = '.';
    std::queue<utils::Coord> q;
    std::unordered_set<utils::Coord> vis;
    std::map<utils::Coord, int> counts;
    const int steps = part == 1? 64: 26'501'365;
    const int reduced_steps = steps < 3 * n? steps: 2 * n + (steps % n); // don't perform too many steps
    q.push(start);
    int step = 0, qsize = 1;
    auto div_floor = [](int n, int d) { return n >= 0 ? n / d : (n + 1) / d - 1; };
    while (!q.empty() && step < reduced_steps) {
        auto pos = q.front();
        q.pop();
        for (auto dir : utils::dir::UDLR) {
            auto p = pos + dir;
            if (lines[((p.i % n) + n) % n][((p.j % m) + m) % m] == '.' && vis.count(p) == 0) {
                q.push(p);
                vis.insert(p);
                if ((step ^ reduced_steps) & 1) {
                    counts[{div_floor(p.i, n), div_floor(p.j, m)}]++;
                    res++;
                }
            }
        }
        if (--qsize == 0) {
            step++;
            qsize = q.size();
        }
    }
    if (reduced_steps < steps) {
        // generally, the shape will be a diamon with interior fixed pattern and same margins at step mod n
        // formula observed on the current input. on the general case, it could require more terms/iterations
        const int64_t N = steps / n;
        res = N * N * counts[{0,1}] + (N-1)*(N-1) * counts[{0,0}]; // interior
        for (auto d : utils::dir::UDLR) {
            res += counts[d * 2]; // diamond corners
        }
        res += (N - 1) * (counts[{-1,-1}] + counts[{-1,1}] + counts[{1,-1}] + counts[{1,1}]); // edges
        res += N * (counts[{-2,-1}] + counts[{-2,1}] + counts[{2,-1}] + counts[{2,1}]); // edges
    }
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day22(utils::FileReader& reader, int part) {
    if (part != 1) return; // solves both parts
    int64_t res1 = 0, res2 = 0;
    using Brick = std::array<std::array<int, 3>, 2>;
    std::vector<Brick> bks;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        Brick b = {};
        if (!line.read("%d,%d,%d~%d,%d,%d", &b[0][0], &b[0][1], &b[0][2], &b[1][0], &b[1][1], &b[1][2])) {
            throw utils::MyException("failed to read " + std::string{line.get()});
        }
        bks.push_back(b);
    }
    const auto touches = [](const Brick& b, const Brick& bs) {
        const auto inbetw = [](int a, int b, int c, int d) { return a <= d && b >= c; };
        if (b[0][2] == bs[1][2] + 1 &&
            inbetw(b[0][0], b[1][0], bs[0][0], bs[1][0]) && inbetw(b[0][1], b[1][1], bs[0][1], bs[1][1])) {
                return true;
        }
        return false;
    };
    std::sort(bks.begin(), bks.end(), [](const Brick& b1, const Brick& b2) { return b1[0][2] < b2[0][2]; });
    std::vector<bool> support(bks.size(), false);
    std::vector<std::vector<int>> suppby(bks.size());
    std::vector<std::vector<int>> suppwho(bks.size());
    for (int i = 0; i < bks.size(); ++i) {
        auto& b = bks[i];
        int supported = false;
        while (!supported && b[0][2] > 1) {
            for (int j = 0; j < i; ++j) {
                if (touches(b, bks[j])) {
                    suppwho[j].push_back(i);
                    suppby[i].push_back(j);
                    supported++;
                }
            }
            if (!supported) {
                b[0][2]--;
                b[1][2]--;
            } else if (supported == 1) {
                support[suppby[i].front()] = true;
            }
        }
    }
    res1 = std::count(support.begin(), support.end(), false);
    printf("Res1 = %lld\n", res1);
    for (int i = 0; i < suppwho.size(); ++i) {
        if (suppwho[i].empty()) continue;
        std::vector<bool> vis(bks.size(), false);
        std::queue<int> q;
        q.push(i);
        vis[i] = true;
        while (!q.empty()) {
            int b = q.front();
            q.pop();
            for (int x : suppwho[b]) {
                if (!vis[x] &&
                    std::all_of(suppby[x].begin(), suppby[x].end(), [&vis](int k) { return vis[k] == true; })) {
                        q.push(x);
                        vis[x] = true;
                }
            }
        }
        res2 += std::count(vis.begin(), vis.end(), true) - 1;
    }
    printf("Res2 = %lld\n", res2);
}
//-----------------------------------------------------------------------------
int day23_longest_dfs1(std::vector<std::string>& lines, std::unordered_set<utils::Coord>& vis, utils::Coord p, int l, utils::Coord& end, int part) {
    if (p == end) return l;
    int maxl = 0;
    vis.insert(p);
    for (auto d : utils::dir::UDLR) {
        if (part == 1 && lines[p.i][p.j] != '.' && !(d == (lines[p.i][p.j] == 'v'? utils::dir::D: utils::dir::R))) { // only v and > exist
            continue;
        }
        auto np = p + d;
        if (np.i >= 0 && lines[np.i][np.j] != '#' && vis.count(np) == 0) {
            maxl = std::max(maxl, day23_longest_dfs1(lines, vis, np, l + 1, end, part));
        }
    }
    vis.erase(p);
    return maxl;
}
void day23(utils::FileReader& reader, int part) {
    int64_t res = 0;
    auto lines = reader.allLines();
    utils::Coord start = {0, 1}, end = {static_cast<int>(lines.size()) - 1, static_cast<int>(lines[0].size()) - 2};
    std::unordered_set<utils::Coord> vis = {};
    res = day23_longest_dfs1(lines, vis, start, 0, end, part);
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
void day24(utils::FileReader& reader, int part) {
    int64_t res = 0;
    struct Hail {
        int64_t x, y, z;
        int vx, vy, vz;
    };
    std::vector<Hail> hails;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        Hail h = {};
        if (!line.read("%lld, %lld, %lld @ %d, %d, %d", &h.x, &h.y, &h.z, &h.vx, &h.vy, &h.vz)) {
            throw utils::MyException("failed to read " + std::string{line.get()});
        }
        hails.push_back(h);
    }
    if (part == 1) {
        const auto inters2d = [](const Hail& h1, const Hail&h2) -> std::pair<double, double> {
            auto denom = h1.vy * h2.vx - h2.vy * h1.vx;
            if (denom != 0) {
                // Line represented as vy*x + vx*y = c
                double c1 = h1.vy * h1.x - h1.vx * h1.y;
                double c2 = h2.vy * h2.x - h2.vx * h2.y;

                double x = (h2.vx * c1 - h1.vx * c2) / denom;
                double y = -(h1.vy * c2 - h2.vy * c1) / denom;

                if ((x - h1.x) * h1.vx >= 0 && (y - h1.y) * h1.vy >= 0 &&
                    (x - h2.x) * h2.vx >= 0 && (y - h2.y) * h2.vy >= 0) {
                        return {x, y}; // intersects in the past
                    }
            }
            return {DBL_MAX, DBL_MAX}; // The lines are parallel
        };
        std::pair<int64_t, int64_t> range = {200000000000000, 400000000000000}; //{7, 27};
        for (int i = 0; i < hails.size(); ++i) {
            for (int j = i + 1; j < hails.size(); ++j) {
                auto [x,y] = inters2d(hails[i], hails[j]);
                if (range.first <= x && x <= range.second && range.first <= y && y < range.second) {
                    res++;
                }
            }
    }
    printf("Res = %lld\n", res);
    } else {
        // for (int i = 0; i < 3; ++i) {
        //     for (int d = 0; d < 3; ++d) {
        //         printf("(%c - %lld) / (%c - %d)%c", 'x' + d, (&hails[i].x)[d], 'a' + d, (&hails[i].vx)[d], d < 2? '=': '\n');
        //     }
        // }
        std::array<std::set<int>, 3> speeds;
        for (int i = 0; i < hails.size(); ++i) {
            for (int j = i + 1; j < hails.size(); ++j) {
                for (int d = 0; d < 3; ++d) {
                    auto ve = (&hails[i].vx)[d];
                    if (ve == (&hails[j].vx)[d]) {
                        auto dp = (&hails[i].x)[d] - (&hails[j].x)[d];
                        std::set<int> newspeed;
                        for (int v = -999; v <= 999; ++v) {
                            if (v && v != ve && (dp % (ve-v)) == 0) {
                                if (speeds[d].empty() || speeds[d].count(v)) {
                                    newspeed.insert(v);
                                }
                            }
                        }
                        std::swap(speeds[d], newspeed);
                    }
                }
            }
        }
        if (std::any_of(speeds.begin(), speeds.end(), [](auto &s) { return s.empty(); })) {
            throw utils::MyException("failed to find speed");
        }
        const int vx = *speeds[0].begin();
        const int vy = *speeds[1].begin();
        const int vz = *speeds[2].begin();
        // PRINT(vx, vy, vz);

        double m1 = static_cast<double>(hails[0].vy - vy) / (hails[0].vx - vx);
        double m2 = static_cast<double>(hails[1].vy - vy) / (hails[1].vx - vx);
        double c1 = hails[0].y - m1 * hails[0].x;
        double c2 = hails[1].y - m2 * hails[1].x;
        int64_t x = (c2 - c1) / (m1 - m2);
        int64_t y = m1 * x + c1;
        int64_t z = hails[0].z + (hails[0].vz - vz) * (x - hails[0].x) / (hails[0].vx - vx);

        printf("Res = %lld\n", x + y + z);
    }
}
//-----------------------------------------------------------------------------
void day25(utils::FileReader& reader, int part) {
    if (part != 1) return; // solves both parts
    int64_t res = 1;
    utils::StringToIdMap sid;
    std::unordered_map<int, std::vector<std::pair<int, int>>> adj;
    std::unordered_map<std::pair<int, int>, int> cost;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        char buf[10];
        line.read("%9[a-z]:", buf);
        const int id = sid.set(buf);
        auto& list = adj[id];
        while (line.read(" %9[a-z]", buf)) {
            const int id2 = sid.set(buf);
            list.push_back({id2, 1});
            adj[id2].push_back({id, 1});
            cost[{id, id2}] = 1;
        }
    }
    const int maxid = sid.set("---") - 1;
    std::vector<int> vis(maxid, 0); // next id is max

    auto reset_cost = [&adj]() {
        for (auto& [id,list] : adj) {
            for (auto& [k,c] : list) {
                c = 1;
            }
        }
    };
    const auto get_edge = [&adj](int from, int to) {
        return std::find_if(adj[from].begin(), adj[from].end(), [to](auto& kc) { return kc.first == to; });
    };
    auto bfs_flow = [&adj,&vis,&get_edge](int sid, int tid) {
        std::queue<int> q;
        q.push(sid);
        vis[sid] = -1;
        while (!q.empty()) {
            int id = q.front();
            q.pop();
            if (id == tid) {
                while (id != sid) {
                    int par = vis[id];
                    get_edge(par, id)->second -= 1;
                    get_edge(id, par)->second += 1;
                    id = par;
                }
                return true;
            }
            for (auto [k,c] : adj[id]) {
                if (c > 0 && !vis[k]) {
                    q.push(k);
                    vis[k] = id;
                }
            }
        }
        return false;
    };
    auto one_side = [&]() -> int {
        for (int id = maxid; id > 0; --id) {
            for (auto& [k,c] : adj[id]) {
                if (k < id) continue;
                reset_cost();
                get_edge(k, id)->second = 0;
                c = 0;
                for (int i = 0; i < 3; ++i) {
                    std::fill(vis.begin(), vis.end(), 0);
                    if (!bfs_flow(id, k)) {
                        if (i == 2) { // found exactly two other paths
                            // no need to find min cut here, we are just interested in the number
                            return std::count_if(vis.begin(), vis.end(), [](int v) { return v != 0; });
                        }
                        break;
                    }
                }
            }
        }
        return 0;
    }();
    res = one_side * (adj.size() - one_side);
    printf("Res = %lld\n", res);
}
//-----------------------------------------------------------------------------
const std::map<std::string, std::function<void(utils::FileReader&, int)>> functions = {
    { "1", day1 }, { "2", day2 }, { "3", day3 }, { "4", day4 }, { "5", day5 },
    { "6", day6 }, { "7", day7 }, { "8", day8 }, { "9", day9 }, { "10", day10 },
    { "11", day11 }, { "12", day12 }, { "13", day13 }, { "14", day14 },
    { "15", day15 }, { "16", day16 }, { "17", day17 }, { "18", day18 },
    { "19", day19 }, { "20", day20 }, { "21", day21 }, { "22", day22 },
    { "23", day23 }, { "24", day24 }, { "25", day25 },
};

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 4) {
        printf("Invalid arguments. Usage: %s day [part [input_file]]", argv[0]);
        return EXIT_FAILURE;
    }

    const std::string day = strcmp(argv[1], "last")? argv[1]: "25";
    const int part = argc > 2? atoi(argv[2]): 1;
    const std::string inputFile = argc > 3? argv[3]: "data/input" + day + ".txt";

    auto start = std::chrono::steady_clock::now();

    try {
        if (day == "all") {
            for (auto [day, func] : functions) {
                printf("day %s:\n", day.c_str());
                for (int part : {1, 2}) {
                    auto reader = utils::FileReader("data/input" + day + ".txt");
                    func(reader, part);
                }
            }
        } else {
            auto reader = utils::FileReader(inputFile);
            functions.at(day)(reader, part);
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
