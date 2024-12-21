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
DAY(22, utils::FileReader& reader, int part) {
    long long res = 0;
    while (reader.nextLine()) {
        auto line = reader.getLine();
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
void Day17_comb(std::vector<std::string> &res, std::string curr, utils::Coord pos, int vi, int vj, utils::Coord end) {
    if (pos == utils::Coord{3, 0}) return; // space
    if (pos == end) {
        res.push_back(curr + 'A');
        return;
    }
    if (pos.i != end.i) Day17_comb(res, curr + (vi < 0? '^': 'v'), {pos.i + vi, pos.j}, vi, vj, end);
    if (pos.j != end.j) Day17_comb(res, curr + (vj < 0? '<': '>'), {pos.i, pos.j + vj}, vi, vj, end);
}

long long Day17_butpress(std::unordered_map<char, utils::Coord>& kbm, const std::string& in,
    int level, std::unordered_map<int, std::unordered_map<std::pair<utils::Coord, utils::Coord>, long long>>& cache
) {
    long long res = 0;
    utils::Coord pos = kbm['A'];
    for (char c : in) {
        const utils::Coord& to = kbm[c];
        long long& best_size = cache[level][{pos, to}];
        if (best_size == 0) {
            std::vector<std::string> comb;
            const utils::Coord dist = to - pos;
            Day17_comb(comb, "", pos, (dist.i < 0? -1: 1), (dist.j < 0? -1: 1), to);
            for (auto& move : comb) {
                long long move_size = move.length();
                if (level) {
                    move_size = Day17_butpress(kbm, move, level - 1, cache);
                }
                if (best_size == 0 || move_size < best_size) {
                    best_size = move_size;
                }
            }
        }
        res += best_size;
        pos = to;
    }
    return res;
}

DAY(21, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<std::string> input;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        input.emplace_back(line.get());
    }
    std::unordered_map<char, utils::Coord> kbm;
    const auto add_to_kbm = [&kbm](const std::vector<std::string>& kb) {
        for (int i = 0; i < kb.size(); ++i) {
            for (int j = 0; j < kb[i].size(); ++j) {
                kbm[kb[i][j]] = utils::Coord{i, j};
            }
        }
    };
    const std::vector<std::string> kb1 = {"789","456","123"," 0A"};
    const std::vector<std::string> kb2 = {"","","", " ^A","<v>"};
    add_to_kbm(kb1);
    add_to_kbm(kb2);
    const int levels = part == 1? 2: 25;
    std::unordered_map<int, std::unordered_map<std::pair<utils::Coord, utils::Coord>, long long>> cache;
    for (std::string in : input) {
        int num_part = 0;
        for (char c : in) {
            if (std::isdigit(c)) {
                num_part = num_part * 10 + (c - '0');
            }
        }
        long long total_size = Day17_butpress(kbm, in, levels, cache);
        PRINT(in, total_size, num_part);
        res += total_size * num_part;
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(20, utils::FileReader& reader, int part) {
    long long res = 0;
    auto [maze, n, m] = reader.allLines();
    utils::Coord start, end;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (maze[i][j] == 'S') start = {i, j};
            else if (maze[i][j] == 'E') end = {i, j};
        }
    }
    std::vector<std::vector<int>> dist{static_cast<std::size_t>(n), std::vector<int>(m, -1)};
    std::queue<std::pair<int, utils::Coord>> q;
    q.push({0, end});
    dist[end.i][end.j] = 0;
    while (!q.empty()) {
        const auto [d,p] = q.front(); q.pop();
        for (const auto& dir : utils::dir::UDLR) {
            const auto pp = p + dir;
            if (maze[pp.i][pp.j] != '#') {
                if (dist[pp.i][pp.j] == -1) {
                    dist[pp.i][pp.j] = d + 1;
                    q.push({d + 1, pp});
                }
            }
        }
    }
    const int normal_dist = dist[start.i][start.j];
    std::vector<std::vector<char>> vis{static_cast<std::size_t>(n), std::vector<char>(m)};
    q.push({0, start});
    vis[start.i][start.j] = 1;
    const int kk = part == 1? 2: 20;
    while (!q.empty()) {
        const auto [d,p] = q.front(); q.pop();
        for (int i = -kk; i <= kk; ++i) {
            const int jkk = kk - std::abs(i);
            for (int j = -jkk; j <= jkk; ++j) {
                const auto pp2 = p + utils::Dir{i, j};
                if (pp2.inside(n, m) && dist[pp2.i][pp2.j] != -1) {
                    const int dnew = d + dist[pp2.i][pp2.j] + std::abs(i) + std::abs(j);
                    const int save = normal_dist - dnew;
                    if (save >= 100) {
                        res++;
                    }
                }
            }
        }
        for (const auto& dir : utils::dir::UDLR) {
            const auto pp = p + dir;
            if (maze[pp.i][pp.j] != '#') {
                if (!vis[pp.i][pp.j]) {
                    vis[pp.i][pp.j] = 1;
                    q.push({d + 1, pp});
                }
            }
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(19, utils::FileReader& reader, int part) {
    long long res = 0;
    reader.nextLine();
    std::vector<std::string> pat;
    for (auto s : reader.getLine().split(",")) {
        pat.push_back(std::string(utils::trim(s)));
    }
    reader.nextLine();
    while (reader.nextLine()) {
        const auto des = reader.getLine().get();
        std::vector<long long> ways(des.size() + 1);
        ways[0] = 1;
        for (int i = 0; i < des.size(); ++i) {
            for (const auto& p : pat) {
                if (des.substr(i, p.size()) == p) {
                    ways[i + p.size()] += ways[i];
                }
            }
        }
        res += part == 1? ways.back() != 0: ways.back();
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(18, utils::FileReader& reader, int part) {
    long long res = 0;
    const int n = 71, m = n;
    int bytes_set = 1024;
    std::vector<std::pair<int, int>> bytes;
    std::vector<std::string> maze{n, std::string(m, '.')};
    while (reader.nextLine()) {
        auto line = reader.getLine();
        int x, y;
        if (line.read("%d,%d", &x, &y)) {
            bytes.push_back({x, y});
        }
    }
    const utils::Coord dest = {n - 1, m - 1};
    const auto bfs = [&](int nbytes) {
        auto maze2 = maze;
        for (int i = 0; i < nbytes; ++i) {
            const auto [x, y] = bytes[i];
            maze2[y][x] = '#';
        }
        utils::Coord pos = {0, 0};
        std::queue<std::pair<utils::Coord, int>> q;
        q.push({pos, 0});
        maze2[pos.i][pos.j] = 'O';
        while (!q.empty()) {
            auto [p, d] = q.front(); q.pop();
            if (p == dest) {
                return d;
            }
            for (const auto& dd : utils::dir::UDLR) {
                utils::Coord pp = p + dd;
                if (pp.inside(n, m) && maze2[pp.i][pp.j] == '.') {
                    q.push({pp, d + 1});
                    maze2[pp.i][pp.j] = 'O';
                }
            }
        }
        return -1;
    };
    if (part == 1) {
        res = bfs(bytes_set);
        printf("Res%d = %lld\n", part, res);
    } else {
        int s = bytes_set, e = bytes.size();
        while (s < e) {
            const int mid = (s + e) / 2;
            if (bfs(mid) == -1) {
                e = mid;
            } else {
                s = mid + 1;
            }
        }
        const auto [x,y] = bytes[s - 1];
        printf("Res%d = %d,%d\n", part, x, y);
    }
}
//-----------------------------------------------------------------------------
DAY(17, utils::FileReader& reader, int part) {
    std::array<long long, 3> reg;
    std::vector<int> prog;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        char c;
        int v;
        if (line.read("Register %c: %d", &c, &v)) {
            reg[c - 'A'] = v;
        } else if (line.read("Program: ")) {
            while (line.read("%d", &v)) {
                prog.push_back(v);
                line.read(",");
            }
        }
    }
    std::vector<int> out;
    const auto combo = [&reg](int x) -> long long { return x < 4? x: x < 7? reg[x - 4]: -1; };
    const auto step = [&](int& p) {
        switch (prog[p]) {
            case 0 :
                reg[0] /= (1 << combo(prog[p + 1]));
                break;
            case 1 :
                reg[1] ^= prog[p + 1];
                break;
            case 2 :
                reg[1] = combo(prog[p + 1]) % 8;
                break;
            case 3 :
                if (reg[0]) {
                    p = prog[p + 1];
                    return;
                }
                break;
            case 4 :
                reg[1] ^= reg[2];
                break;
            case 5 :
                out.push_back(combo(prog[p + 1]) % 8);
                break;
            case 6 :
                reg[1] = reg[0] / (1 << combo(prog[p + 1]));
                break;
            case 7 :
                reg[2] = reg[0] / (1 << combo(prog[p + 1]));
                break;
        }
        p += 2;
    };
    if (part == 1) {
        for (int p = 0; p < prog.size(); ) {
            step(p);
        }
        printf("Res%d = ", part);
        for (int v : out) printf("%d,", v);
        printf("\n");
    } else {
        // based on observation, each program run takes between 3-10 bits of A
        // to compute the next output value. so we use this to iteratively
        // find possible input combinations that produce desired output.
        std::vector<std::unordered_set<long long>> ss;
        ss.resize(prog.size());
        const auto run = [&](long long a) {
            reg[0] = a;
            reg[1] = reg[2] = 0;
            out.clear();
            for (int p = 0; p < prog.size(); ) {
                step(p);
            }
        };
        for (int i = 0; i < (1 << 10); ++i) {
            run(i);
            if (out[0] == prog[0]) {
                ss[0].insert(i);
            }
        }
        for (int k = 1; k < prog.size(); ++k) {
            for (auto val : ss[k - 1]) {
                for (long long q = 0; q < 8; ++q) {
                    const auto val2 = val | (q << (7 + 3 * k));
                    run(val2);
                    if (out[k] == prog[k] && out.size() <= prog.size()) {
                        ss[k].insert(val2);
                    }
                }
            }
        }
        long long res = *std::min_element(ss.back().begin(), ss.back().end());
        printf("Res%d = %lld\n", part, res);
    }
}
//-----------------------------------------------------------------------------
DAY(16, utils::FileReader& reader, int part) {
    long long res = 0;
    auto [maze, n, m] = reader.allLines();
    utils::Coord start, dest;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (maze[i][j] == 'S') start = {i, j};
            else if (maze[i][j] == 'E') dest = {i, j};
        }
    }
    using Par = std::pair<utils::Coord, utils::Dir>;
    using T4 = std::tuple<int, utils::Coord, utils::Dir, Par>;
    std::priority_queue<T4, std::vector<T4>, std::greater<T4>> q;
    std::unordered_map<std::pair<utils::Coord, utils::Dir>, int> vis;
    std::unordered_map<Par, std::set<Par>> parents;
    q.push({0, start, utils::dir::R, {}});
    q.push({1000, start, utils::dir::U, {}});
    while (!q.empty()) { // dijkstra
        auto [dist, pos, dir, par] = q.top();
        q.pop();
        const auto vit = vis.find({pos,dir});
        if (vit != vis.end() && dist > vit->second) continue;
        if (res && res < dist) break;
        if (part == 2) {
            parents[{pos,dir}].insert(par);
        }
        if (pos == dest) {
            res = dist;
            continue;
        }
        auto np = pos + dir;
        if (maze[np.i][np.j] == '#') {
            continue;
        }
        for (int k = -1; k < 2; ++k) {
            auto nd = k == 0? dir: dir.rot90(k > 0);
            int cd = dist + 1 + std::abs(k) * 1000;
            if (auto it = vis.find({np,nd}); it == vis.end() || cd <= it->second) {
                q.push({cd, np, nd, {pos,dir}});
                if (it == vis.end()) {
                    vis.insert({{np,nd}, cd});
                } else {
                    it->second = cd;
                }
            }
        }
    }
    if (part == 2) {
        // trace back all paths that lead to dest
        std::queue<Par> qp;
        std::unordered_set<utils::Coord> path;
        for (auto d : utils::dir::UDLR) {
            qp.push({dest, d});
        }
        while (!qp.empty()) {
            auto [p,d] = qp.front(); qp.pop();
            if (p == start) break;
            path.insert(p);
            //maze[p.i][p.j] = 'O';
            for (auto pp : parents[{p,d}]) {
                qp.push(pp);
            }
        }
        //PRINT(maze);
        res = path.size() + 1;
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(15, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<std::string> maze;
    std::string moves;
    utils::Coord pos;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        if (line.get().empty()) break;
        std::string row = std::string(line.get());
        if (part == 2) {
            std::string row2;
            for (char c : row) {
                if (c == 'O') row2.append("[]");
                else {
                    row2.push_back(c);
                    row2.push_back(c == '#'? '#': '.');
                }
            }
            row = row2;
        }
        if (const char *p = strchr(row.c_str(), '@'); p) {
            pos = {static_cast<int>(maze.size()), static_cast<int>(p - row.c_str())};
        }
        maze.push_back(std::move(row));
    }
    while (reader.nextLine()) {
        auto line = reader.getLine();
        moves.append(line.get());
    }
    const int n = maze.size();
    const int m = maze[0].size();
    const std::map<char, utils::Dir> dirs = {
        {'^', utils::dir::U}, {'<', utils::dir::L}, {'>', utils::dir::R}, {'v', utils::dir::D},
    };
    const auto move_in_maze = [&](const std::unordered_set<utils::Coord>& pos,
                               const utils::Dir& d, std::unordered_set<utils::Coord>& out) -> int {
        bool next = false;
        for (const auto& pp : pos) {
            utils::Coord p = pp + d;
            const char c = maze[p.i][p.j];
            if (c == '#') return -1;
            if (c != '.') {
                next = true;
                out.insert(p);
                if (d.i && (c == '[' || c == ']')) {
                    utils::Coord p2 = p + (c == '['? utils::dir::R: utils::dir::L);
                    out.insert(p2);
                }
            }
        }
        return next;
    };

    auto prevmaze = maze;
    for (char c : moves) {
        utils::Dir d = dirs.at(c);
        std::unordered_set<utils::Coord> out{};
        std::vector<std::unordered_set<utils::Coord>> mm = {{pos}};
        int status = 0;
        while ((status = move_in_maze(mm.back(), d, out)) == 1) {
            mm.push_back(std::move(out));
            out = {};
        }
        if (status == 0) {
            for (int i = mm.size() - 1; i >= 0; --i) {
                for (const auto& p : mm[i]) {
                    const utils::Coord e = p + d;
                    maze[e.i][e.j] = maze[p.i][p.j];
                    maze[p.i][p.j] = '.';
                }
            }
            pos = pos + d;
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (maze[i][j] == 'O' || maze[i][j] == '[') {
                res += 100 * i + j;
            }
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(14, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<std::pair<utils::Coord, utils::Dir>> robots;
    while (reader.nextLine()) {
        auto line = reader.getLine();
        int px,py, vx,vy;
        if (line.read("p=%d,%d v=%d,%d", &py, &px, &vy, &vx)) {
            robots.push_back({{px, py}, {vx, vy}});
        }
    }
    const int W = 101, H = 103;
    const auto mod = [](int x, int m) { return ((x % m) + m) % m; };
    std::array<int, 4> quad{};
    std::unordered_set<utils::Coord> pos;
    const auto move = [&](int n) {
        for (const auto& [v, d] : robots) {
            const utils::Coord p = v + d * n;
            const utils::Coord q = {mod(p.i, H), mod(p.j, W)};
            if (q.i != H / 2 && q.j != W / 2) {
                quad[(q.i / (H/2 + 1)) * 2 + (q.j / (W/2 + 1))]++;
            }
            if (part == 2) {
                pos.insert(q);
            }
        }
    };
    if (part == 1) {
        move(100);
        res = 1;
        for (int q : quad) res *= q;
    } else {
        for (int i = 100; !res; ++i) {
            pos.clear();
            move(i);
            // guess that we need to stop when no points overlap,
            // because that is the way these puzzles are built.
            if (pos.size() == robots.size()) {
                res = i;
            }
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(13, utils::FileReader& reader, int part) {
    long long res = 0;
    int ax,ay, bx,by;
    long long tx,ty;
    do {
        reader.nextLine(); reader.getLine().read("Button A: X+%d, Y+%d", &ax, &ay);
        reader.nextLine(); reader.getLine().read("Button B: X+%d, Y+%d", &bx, &by);
        reader.nextLine(); reader.getLine().read("Prize: X=%lld, Y=%lld", &tx, &ty);
        if (part == 2) {
            tx += 10000000000000;
            ty += 10000000000000;
        }
        // line1: p=0,0 m=ay/ax,  line2: ptx,ty, m=by/bx
        // x = (y1 - m2 * x1) / (m1 - m2)
        const double px = (ty - by * tx / (double) bx) / (ay / (double) ax - by / (double) bx);
        // y = m1 * x
        const double py = ay * px / (double) ax;
        const long long x = std::round(px);
        const long long y = std::round(py);
        if ((x % ax) == 0 && ((tx - x) % bx) == 0 && (y % ay) == 0 && ((ty - y) % by) == 0) {
            res += 3 * (x / ax) + (tx - x) / bx;
        }
    } while (reader.nextLine());
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
            // merge each two adjacent blocks that form a longer side
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
        nodes.reserve(nodes.size() * 3); // hack to keep a pointer in the vector even if we add stuff
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
    // ok, part 2 could have been done with dfs, but too late now
    const auto bfs_reach = [&](const utils::Coord& pos) -> int {
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
        res += part == 1? bfs_reach(head): day10_dfs(head, lines, n, m, vis);
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
        // backtracking to the target
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
        bool ok = [&]() {
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
                            if (!(ii >= 0 && ii < n && jj >= 0 && jj < m && lines[ii][jj] == xmas[k])) {
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
            switch (z) { // instead of regex, have a state machine
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
