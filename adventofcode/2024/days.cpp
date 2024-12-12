/*
 * adventofcode.cpp
 *
 * Author: Mihai Feier
 * Copyright QTronic GmbH. All rights reserved.
 */
#include "days.h"

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

//-----------------------------------------------------------------------------
DAY(14, utils::FileReader& reader, int part) {
    long long res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(13, utils::FileReader& reader, int part) {
    long long res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(12, utils::FileReader& reader, int part) {
    long long res = 0;
    auto [lines, n, m] = reader.allLines();
    std::unordered_set<utils::Coord> vis;
    const auto region = [&](const utils::Coord& pos) -> std::pair<int, int> {
        const char mark = lines[pos.i][pos.j];
        int area = 0;
        int perim = 0;
        std::queue<utils::Coord> q;
        q.push(pos);
        vis.clear();
        vis.insert(pos);
        lines[pos.i][pos.j] = '-';
        while (!q.empty()) {
            auto p = q.front(); q.pop();
            for (const auto& d : utils::dir::UDLR) {
                utils::Coord pp = p + d;
                if (pp.inside(n, m) && lines[pp.i][pp.j] == mark) {
                    q.push(pp);
                    vis.insert(pp);
                    lines[pp.i][pp.j] = '-';
                } else if (!vis.count(pp)) {
                    perim++;
                }
            }
            area++;
        }
        if (part == 2) {
            for (const auto& p : vis) {
                for (auto d : {utils::dir::D, utils::dir::R}) {
                    auto p2 = p + d;
                    if (vis.count(p2)) {
                        for (auto dd : {d.rot90(true), d.rot90(false)}) {
                            if (!vis.count(p + dd) && !vis.count(p2 + dd)) {
                                perim--;
                            }
                        }
                    }
                }
            }
        }
        return {area, perim};
    };
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] != '-') {
                auto [area, perim] = region({i, j});
                res += area * perim;
            }
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
struct Day11Node {
    long long val = 0;
    int parent = 0;
    int size_index = 0;
    long long size = 1;
    std::vector<long long> sizes;
};

DAY(11, utils::FileReader& reader, int part) {
    long long res = 0;
    reader.nextLine();
    auto line = reader.getLine();
    std::vector<Day11Node> nodes(1);
    std::unordered_map<long long, int> mn;
    int v = 0;
    while (line.read("%d", &v)) {
        nodes.push_back({v, 0});
    }
    int repeat = (part == 1)? 25: 75;
    for (int k = 0; k < repeat; ++k) {
        nodes[0].size = 0;
        nodes.reserve(nodes.size() * 3); // hack to keep a pointer in the vector even if it's resized
        for (int i = nodes.size() - 1; i; --i) {
            auto& n = nodes[i];
            if (mn.count(n.val)) {
                const int ii = mn[n.val];
                auto& p = nodes[ii];
                if (ii != i) {
                    n.size = p.sizes[n.size_index++];
                } else {
                    n.sizes.push_back(n.size);
                }
            } else {
                n.size = 1;
                if (n.val == 0) {
                    n.val = 1;
                } else if (int d = utils::digits(n.val); (d % 2) == 0) {
                    int x = 1;
                    for (d = d / 2; d; --d) x *= 10;
                    nodes.push_back({n.val / x, i});
                    nodes.push_back({n.val % x, i});
                    mn[n.val] = i;
                    n.sizes.push_back(2);
                    n.size = 2;
                } else {
                    n.val *= 2024;
                }
            }
            nodes[n.parent].size += n.size;
            n.size = 0;
        }
        //if (part == 2) PRINT(k, nodes[0].size, s.size(), s);
    }
    //res = s.size();
    printf("Res%d = %lld\n", part, nodes[0].size);
}
//-----------------------------------------------------------------------------
int day10_dfs(utils::Coord pos, const std::vector<std::string> &lines, int n, int m, std::unordered_set<utils::Coord>& vis) {
    if (lines[pos.i][pos.j] == '9') return 1;
    int res = 0;
    vis.insert(pos);
    for (const auto& d : utils::dir::UDLR) {
        utils::Coord pp = pos + d;
        if (pp.inside(n, m) && !vis.count(pp) && lines[pos.i][pos.j] + 1 == lines[pp.i][pp.j]) {
            res += day10_dfs(pp, lines, n, m, vis);
        }
    }
    vis.erase(pos);
    return res;
}

DAY(10, utils::FileReader& reader, int part) {
    int res = 0;
    auto [lines, n, m] = reader.allLines();
    std::vector<utils::Coord> heads;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == '0') {
                heads.push_back({i, j});
            }
        }
    }
    std::unordered_set<utils::Coord> vis;
    const auto reach = [&](const utils::Coord& pos) -> int {
        std::queue<utils::Coord> q;
        q.push(pos);
        int count = 0;
        while (!q.empty()) {
            auto p = q.front(); q.pop();
            if (lines[p.i][p.j] == '9') {
                count++;
                continue;
            }
            for (const auto& d : utils::dir::UDLR) {
                utils::Coord pp = p + d;
                if (pp.inside(n, m) && !vis.count(pp) && lines[p.i][p.j] + 1 == lines[pp.i][pp.j]) {
                    q.push(pp);
                    vis.insert(pp);
                }
            }
        }
        return count;
    };
    for (const auto& head : heads) {
        vis.clear();
        res += part == 1? reach(head): day10_dfs(head, lines, n, m, vis);
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
struct Day9Pos {
    int id = 0, pos = 0, cnt = 0, space = 0, size = 0;
};

DAY(9, utils::FileReader& reader, int part) {
    long long res = 0;
    reader.nextLine();
    auto line = reader.getLine().get();
    std::vector<Day9Pos> a;
    int pos = 0;
    for (int i = 0; i < line.length(); ++i) {
        const int cnt = line[i] - '0';
        if ((i % 2) == 0) a.push_back({i/2, pos, cnt, 0, cnt});
        else {
            a.back().space = cnt;
            a.back().size += cnt;
        }
        pos += cnt;
    }
    const auto checksum = [](int p, int id, int cnt){
        return (long long)(id) * (2 * p + cnt - 1) * cnt / 2;
    };
    int e = a.size() - 1;
    if (part == 2) {
        for (; e > 0; --e) {
            for (int i = 0; i < e; ++i) {
                if (a[i].space >= a[e].cnt) {
                    a[e].pos = a[i].pos + a[i].size - a[i].space;
                    a[i].space -= a[e].cnt;
                    //PRINT("move", a[e].id, a[e].pos);
                    break;
                }
            }
        }
    }
    for (int i = 0; i < a.size(); ++i) {
        res += checksum(a[i].pos, a[i].id, a[i].cnt);
        int p = a[i].pos + a[i].cnt;
        int cnt = a[i].space;
        if (part == 1) {
            while (cnt && i < e) {
                int c = std::min(a[e].cnt, cnt);
                res += checksum(p, a[e].id, c);
                p += c;
                cnt -= c;
                a[e].cnt -= c;
                if (!a[e].cnt) e--;
            }
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(8, utils::FileReader& reader, int part) {
    int res = 0;
    auto [lines, n, m] = reader.allLines();
    std::unordered_map<char, std::vector<utils::Coord>> antennas;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] != '.') {
                antennas[lines[i][j]].push_back({i, j});
            }
        }
    }
    std::unordered_set<utils::Coord> nodes;
    for (const auto& [_, v] : antennas) {
        for (int i = 0; i < v.size(); ++i) {
            for (int j = i + 1; j < v.size(); ++j) {
                const utils::Coord ab = {v[j].i - v[i].i, v[j].j - v[i].j};
                const int lim = part == 1? 1: 1000;
                const auto add_node = [&](const utils::Coord& p, const utils::Coord& d) {
                    const utils::Coord pp = p + d;
                    if (pp.inside(n, m)) {
                        nodes.insert(pp);
                        return true;
                    }
                    return false;
                };
                if (part == 1) {
                    add_node(v[i], -ab);
                    add_node(v[j], ab);
                } else {
                    for (int k = 0; add_node(v[i], -ab * k); ++k);
                    for (int k = 0; add_node(v[j], ab * k); ++k);
                }
            }
        }
    }
    res = nodes.size();
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
bool day7_try(long long target, long long acc, std::vector<int>& vals, int p, bool part2) {
    if (p == vals.size()) return target == acc;
    if (part2) {
        int dig = 1;
        for (int vp = vals[p]; vp; dig *= 10, vp /= 10);
        long long acc_conc = acc * dig + vals[p];
        if (acc_conc <= target && day7_try(target, acc_conc, vals, p + 1, part2)) return true;
    }
    long long acc_add = acc + vals[p];
    long long acc_mul = acc * vals[p];
    return (acc_add <= target && day7_try(target, acc_add, vals, p + 1, part2)) ||
           (acc_mul <= target && day7_try(target, acc_mul, vals, p + 1, part2));
}
DAY(7, utils::FileReader& reader, int part) {
    long long res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        long long target = 0;
        int v = 0;
        std::vector<int> vals;
        if (!line.read("%lld:", &target)) break;
        while (line.read("%d", &v)) {
            vals.push_back(v);
        }
        if (day7_try(target, vals[0], vals, 1, part == 2)) {
            res += target;
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(6, utils::FileReader& reader, int part) {
    int res = 0;
    auto [lines, n, m] = reader.allLines();
    utils::Coord pos;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == '^') {
                pos = {i, j};
            }
        }
    }
    lines[pos.i][pos.j] = '.';
    utils::Dir d = utils::dir::U;
    std::unordered_set<utils::Coord> vis;
    std::unordered_set<std::pair<utils::Coord, utils::Dir>> vis2;
    while (true) {
        vis.insert(pos);
        utils::Coord p2 = pos + d;
        if (!p2.inside(n, m)) break;
        if (lines[p2.i][p2.j] == '#') {
            d = d.rot90(true);
        } else {
            if (lines[pos.i][pos.j] == '.') {
                if (part == 1) {
                    res++;
                }
            }
            lines[pos.i][pos.j] = '*';
            if (part == 2 && !vis.count(p2)) {
                const char old = lines[p2.i][p2.j];
                lines[p2.i][p2.j] = '#';
                utils::Dir dd = d.rot90(true);
                utils::Coord pp = pos;
                vis2.clear();
                while (true) {
                    utils::Coord p3 = pp + dd;
                    if (!p3.inside(n, m)) break;
                    if (lines[p3.i][p3.j] == '#') {
                        dd = dd.rot90(true);
                    } else if (vis2.count({pp, dd})) {
                        res++;
                        break;
                    } else {
                        vis2.insert({pp, dd});
                        pp = p3;
                    }
                }
                lines[p2.i][p2.j] = old;
            }
            pos = p2;
        }
    }
    if (part == 1) res++; // end
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(5, utils::FileReader& reader, int part) {
    int res = 0;
    std::unordered_map<int, std::unordered_set<int>> before;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        int a = 0, b = 0;
        if (!line.read("%d|%d", &a, &b)) break;
        before[b].insert(a);
    }
    while (reader.nextLine()) {
        auto line = reader.getLine();
        std::vector<int> v;
        int a = 0;
        while (line.read("%d", &a)) {
            v.push_back(a);
            line.read(",");
        }
        bool ok = [&](){
            bool ok = true;
            for (int i = 0; i < v.size(); ++i) {
                for (int j = i + 1; j < v.size(); ++j) {
                    if (before[v[i]].count(v[j])) {
                        if (part == 1) {
                            return false;
                        } else {
                            std::swap(v[i], v[j]);
                            ok = false;
                        }
                    }
                }
            }
            return ok;
        }();
        if ((part == 1 && ok) || !ok) {
            res += v[v.size() / 2];
        }
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(4, utils::FileReader& reader, int part) {
    int res = 0;
    const char xmas[5] = "XMAS";
    const auto is_ms = [](char a, char b) { return (a == 'M' && b == 'S') || (b == 'M' && a == 'S');};
    auto [lines, n, m] = reader.allLines();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (part == 1) {
                if (lines[i][j] == xmas[0]) {
                    for (const auto d : utils::dir::ALL8) {
                        int k = 0;
                        for (k = 1; k < 4; ++k) {
                            const int ii = i + d.i * k;
                            const int jj = j + d.j * k;
                            if (!(ii >= 0 && ii < n && jj >= 0 & jj < m && lines[ii][jj] == xmas[k])) {
                                break;
                            }
                        }
                        res += (k == 4);
                    }
                }
            } else {
                if (lines[i][j] == 'A' && i && j && i < n - 1 && j < m - 1) {
                    char x[4] = {lines[i - 1][j - 1], lines[i + 1][j + 1], lines[i - 1][j + 1], lines[i + 1][j - 1]};
                    if (is_ms(x[0], x[1]) && is_ms(x[2], x[3])) {
                        res++;
                    }
                }
            }
        }
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(3, utils::FileReader& reader, int part) {
    int res = 0;
    bool en = true;
    while (reader.nextLine()) {
        const char *p = reader.getLine().get().data();
        int a = 0, b = 0;
        int z = 0;
        while (*p) {
            const char c = *p;
            const int prevz = z;
            switch (z) {
                case 0: a = b = 0; if (c == 'm') z++; else if (part == 2 && c == 'd') z = 7; else z = 0; break;
                case 1: if (c == 'u') z++; else z = 0; break;
                case 2: if (c == 'l') z++; else z = 0; break;
                case 3: if (c == '(') z++; else z = 0; break;
                case 4: if (isdigit(c)) a = a * 10 + c - '0'; else if (c == ',') z++; else z = 0; break;
                case 5: if (isdigit(c)) b = b * 10 + c - '0'; else if (c == ')') z++; else z = 0; break;
                 case 6: if (en) res += a * b; z = 0; break;
                case 7: if (c == 'o') z++; else z = 0; break;
                case 8: if (c == '(') z++; else if (c == 'n') z = 10; else z = 0; break;
                 case 9: if (c == ')') en = true; z = 0; break;
                case 10: if (c == '\'') z++; else z = 0; break;
                case 11: if (c == 't') z++; else z = 0; break;
                case 12: if (c == '(') z++; else z = 0; break;
                 case 13: if (c == ')') en = false; z = 0; break;
            }
            if (!(prevz && (!z ||  z == 6))) ++p;
        }
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(2, utils::FileReader& reader, int part) {
    int res = 0;
    const auto vec_ok = [](std::vector<int>& v) {
        int prevd = 0;
        for (int i = 1; i < v.size(); ++i) {
            int dd = v[i] - v[i - 1];
            if (!dd || prevd * dd < 0 || std::abs(dd) > 3) {
                return false;
            }
            prevd = dd;
        }
        return true;
    };
    while (reader.nextLine()) {
        auto line = reader.getLine();
        int x = 0;
        std::vector<int> v;
        while (line.read("%d", &x)) {
            v.push_back(x);
        }
        bool ok = vec_ok(v);
        if (!ok && part == 2) {
            std::vector<int> vv(v.begin() + 1, v.end());
            for (int i = 0; !ok && i < v.size(); ++i) {
                ok = vec_ok(vv);
                if (i < vv.size()) {
                    vv[i] = v[i];
                }
            }
        }
        res += ok;
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(1, utils::FileReader& reader, int part) {
    std::vector<int> aa, bb;
    std::map<int, int> ab;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        int a = 0, b = 0;
        if (!line.read("%d %d", &a, &b)) {
            throw utils::MyException("error reading game");
        }
        aa.push_back(a);
        bb.push_back(b);
        ab[b]++;
    }
    std::sort(aa.begin(), aa.end());
    std::sort(bb.begin(), bb.end());
    int res = 0;
    for (int i = 0; i < aa.size(); ++i) {
        res += part == 1? std::abs(aa[i] - bb[i]) : aa[i] * ab[aa[i]];
    }
    printf("Res%d = %d\n", part, res);
}
//-----------------------------------------------------------------------------
