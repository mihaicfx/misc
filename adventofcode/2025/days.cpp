/*
 * adventofcode.cpp
 *
 * Author: Mihai Feier
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
DAY(12, utils::FileReader& reader, int part) {
    if (part == 2) return;
    long long res = 0;
    std::array<int, 6> shapes; // 6 shapes of size 3x3
    for (int i = 0; i < 6; ++i) {
        reader.nextLine();
        int cnt = 0;
        for (int j = 0; j < 3; ++j) {
            reader.nextLine();
            auto a = reader.getLine().get();
            cnt += std::count(a.begin(), a.end(), '#');
        }
        shapes[i] = cnt;
        reader.nextLine(); // space
    }
    for (auto line : reader) {
        int n, m, x[6];
        line.read("%dx%d: %d %d %d %d %d %d", &n, &m, x + 0, x + 1, x + 2, x + 3, x + 4, x + 5);
        // dissapointing solution: just check if area is enough
        int s = n * m;
        for (int i = 0; i < 6; ++i) {
            s -= x[i] * shapes[i];
        }
        res += s >= 0;
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(11, utils::FileReader& reader, int part) {
    long long res = 0;
    std::unordered_map<int, std::vector<int>> graph;
    utils::StringToIdMap str2id;
    for (auto line : reader) {
        const auto els = line.split(" ");
        auto& e = graph[str2id.set(std::string(els[0].substr(0, 3)))];
        for (int i = 1; i < els.size(); ++i) {
            e.push_back(str2id.set(std::string(els[i])));
        }
    }
    int end = str2id.get("out");
    if (part == 1) {
        std::vector<long long> d(str2id.size() + 1);
        d[str2id.get("you")] = 1;
        bool go = true;
        while (go) {
            go = false;
            std::vector<long long> d2(d.size());
            for (int i = 0; i < d.size(); ++i) {
                if (d[i]) {
                    for (const auto j : graph[i]) {
                        d2[j] += d[i];
                        go = true;
                    }
                }
            }
            d = d2;
            res += d[end];
        }
    } else {
        int a = str2id.get("dac");
        int b = str2id.get("fft");
        std::array<std::vector<long long>, 4> d;
        for (auto &dd : d) {
            dd.resize(str2id.size() + 1);
        }
        d[0][str2id.get("svr")] = 1;
        bool go = true;
        while (go) {
            go = false;
            std::array<std::vector<long long>, 4> d2;
            for (auto &dd : d2) {
                dd.resize(str2id.size() + 1);
            }
            for (int k = 0; k < 4; ++k) {
                for (int i = 0; i < d[k].size(); ++i) {
                    if (d[k][i]) {
                        for (const auto j : graph[i]) {
                            int kk = k;
                            if (j == a) {
                                kk += 2;
                            } else if (j == b) {
                                kk += 1;
                            }
                            d2[kk][j] += d[k][i];
                            go = true;
                        }
                    }
                }
            }
            d = d2;
            res += d[3][end];
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
int Day10_part1(std::string_view lights, const std::vector<int>& v) {
    int target = 0;
    for (int i = 0; i < lights.size() - 2; ++i) {
        target |= (lights[i + 1] == '#') << i;
    }
    int level = 0;
    int next = 1;
    int found = 0;
    std::queue<int> q;
    std::vector<bool> vis(1 << 10);
    vis[0] = true;
    q.push(0);
    while (!q.empty() && found == 0) {
        const auto e = q.front();
        q.pop();
        for (int i = 0; i < v.size(); ++i) {
            const int ne = e ^ v[i];
            if (!vis[ne]) {
                vis[ne] = true;
                q.push(ne);
                if (ne == target) {
                    found = level + 1;
                    break;
                }
            }
        }
        if (--next == 0) {
            level++;
            next = q.size();
        }
    }
    return found;
}

// runs for a very long time. there must be someting better
int Day10_p2_bt(std::vector<std::vector<int>>& m, std::vector<int> re) {
    int pi = -1;
    int pn = 99;
    int pp = 0;
    int ps = 0;
    for (int i = 0; i < m.size(); ++i) {
        int s = m[i][0];
        int p = -1;
        int n = 0;
        for (int j = 1; j < m[i].size(); ++j) {
            const int v = re[m[i][j]];
            if (v == -1) {
                p = j;
                n++;
            } else {
                s -= v;
            }
        }
        if (s < 0) return -1;
        if (n == 0 && s > 0) return -1;
        if (n > 0 && n < pn) {
            pi = i;
            pn = n;
            pp = p;
            ps = s;
        }
    }
    if (pi == -1) {
        return std::accumulate(re.begin(), re.end(), 0, [](int a, int b) {
            return b >= 0? a + b: a;
        });
    }
    int res = -1;
    if (pn == 1) {
        re[m[pi][pp]] = ps;
        res = Day10_p2_bt(m, re);
        re[m[pi][pp]] = -1;
        return res;
    }
    for (int k = 0; k <= ps; ++k) {
        re[m[pi][pp]] = k;
        const auto r = Day10_p2_bt(m, re);
        re[m[pi][pp]] = -1;
        if (r >= 0) {
            res = res < 0? r: std::min(res, r);
        }
    }
    return res;
}

int Day10_part2(std::string_view joltage, const std::vector<int>& v) {
    std::vector<int> target;
    const char *p = joltage.data() + 1;
    while (p) {
        target.push_back(std::atoi(p));
        const auto p2 = strchr(p, ',');
        p = p2? p2 + 1: nullptr;
    }
    std::vector<std::vector<int>> m;
    m.resize(target.size());
    for (int i = 0; i < target.size(); ++i) {
        m[i].push_back(target[i]);
        for (int j = 0; j < v.size(); ++j) {
            if (v[j] & (1 << i)) {
                m[i].push_back(j);
            }
        }
    }
    std::vector<int> re(v.size(), -1);
    const auto res = Day10_p2_bt(m, re);
    PRINT(res);
    return res;
}

DAY(10, utils::FileReader& reader, int part) {
    long long res = 0;
    for (auto line : reader) {
        const auto els = line.split(" ");
        std::vector<int> v;
        for (int i = 1; i < els.size() - 1; ++i) {
            int& comb = v.emplace_back(0);
            for (int j = 1; j < els[i].size(); j += 2) {
                comb |= 1 << (els[i][j] - '0');
            }
        }
        res += part == 1? Day10_part1(els.front(), v): Day10_part2(els.back(), v);
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(9, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<utils::Coord> v;
    for (auto line : reader) {
        long a = 0, b = 0;
        line.read("%d,%d", &a, &b);
        v.emplace_back(a, b);
    }
    for (int i = 0; i < v.size(); ++i) {
        for (int j = i + 1; j < v.size(); ++j) {
            const auto d = v[j] - v[i];
            long long area = static_cast<long long>(std::abs(d.i) + 1) * (std::abs(d.j) + 1);
            if (area > res) {
                bool ok = true;
                if (part == 2) {
                    if (d.i != 0 && d.j != 0) {
                        const auto a = utils::Coord(std::min(v[j].i, v[i].i), std::min(v[j].j, v[i].j));
                        const auto b = utils::Coord(std::max(v[j].i, v[i].i), std::max(v[j].j, v[i].j));
                        for (int k = 0; ok && k < v.size(); ++k) {
                            const auto k1 = (k + 1) % v.size();
                            const auto ka = utils::Coord(std::min(v[k].i, v[k1].i), std::min(v[k].j, v[k1].j));
                            const auto kb = utils::Coord(std::max(v[k].i, v[k1].i), std::max(v[k].j, v[k1].j));
                            if ((ka.i == kb.i && ka.i > a.i && kb.i < b.i && ka.j < b.j && kb.j > a.j)) {
                                ok = false;
                            }
                            if ((ka.j == kb.j && ka.j > a.j && kb.j < b.j && ka.i < b.i && kb.i > a.i)) {
                                ok = false;
                            }
                        }
                    }
                }
                if (ok) {
                    res = std::max(res, area);
                }
            }
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(8, utils::FileReader& reader, int part) {
    long long res = 0;
    using D3 = std::tuple<int, int, int>;
    std::vector<D3> v;
    for (auto line : reader) {
        long x, y, z;
        if (!line.read("%d,%d,%d", &x, &y, &z)) {
            break;
        }
        v.emplace_back(x, y, z);
    }
    std::vector<std::tuple<long long, int, int>> edges;
    edges.reserve(v.size() * (v.size() - 1) / 2);
    for (int i = 0; i < v.size(); ++i) {
        for (int j = i + 1; j < v.size(); ++j) {
            const auto [x1, y1, z1] = v[i];
            const auto [x2, y2, z2] = v[j];
            const auto d = static_cast<long long>(x1 - x2) * (x1 - x2) + static_cast<long long>(y1 - y2) * (y1 - y2) + static_cast<long long>(z1 - z2) * (z1 - z2);
            edges.emplace_back(d, i, j);
        }
    }
    // compute minimum spanning tree
    std::sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
        return std::get<0>(a) < std::get<0>(b);
    });
    std::vector<int> pars(v.size());
    std::iota(pars.begin(), pars.end(), 0);
    std::function<int(int)> find;
    find = [&pars,&find](int i) {
        if (pars[i] != i) {
            pars[i] = find(pars[i]);
        }
        return pars[i];
    };
    const auto unite = [&pars](int i, int j) {
        // todo: rank?
        pars[i] = j;
    };
    int n = 0;
    for (auto [d, i, j] : edges) {
        auto pi = find(i);
        auto pj = find(j);
        if (pi != pj) {
            unite(pi, pj);
            if (part == 2 && ++n == v.size() - 1) {
                res = std::get<0>(v[i]) * std::get<0>(v[j]);
                break;
            }
        }
        if (part == 1 && ++n == 1000) {
            break;
        }
    }
    if (part == 1) {
        // find the 3 largest components
        std::vector<int> sizes(v.size());
        for (int i = 0; i < pars.size(); ++i) {
            sizes[find(i)]++;
        }
        std::nth_element(sizes.begin(), sizes.begin() + 3, sizes.end(), std::greater<int>());
        res = sizes[0] * sizes[1] * sizes[2];
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(7, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<long long> v;
    if (reader.nextLine()) {
        auto line = reader.getLine().get();
        v.resize(line.size());
        for (int i = 0; i < line.size(); ++i) {
            v[i] = line[i] == 'S';
        }
    }
    for (auto line : reader) {
        auto l = line.get();
        for (int i = 0; i < l.size(); ++i) {
            if (v[i] && l[i] == '^') {
                v[i - 1] += v[i];
                v[i + 1] += v[i];
                v[i] = 0;
                if (part == 1) {
                    res++;
                }
            }
        }
    }
    if (part == 2) {
        res = std::accumulate(v.begin(), v.end(), 0ll);
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(6, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<std::vector<long long>> v;
    std::vector<char> ops;
    // OK, I don't like this solution, I need to refactor it at some point
    if (part == 1) {
        for (auto line : reader) {
            std::vector<long long> lineV;
            for (auto v : line.split(" ")) {
                if (isdigit(v[0])) {
                    lineV.push_back(atoll(v.data()));
                } else {
                    ops.push_back(v[0]);
                }
            }
            if (!lineV.empty()) {
                v.push_back(lineV);
            } else {
                break;
            }
        }
    } else {
        auto [text, n, m] = reader.allLines();
        v.resize(n - 1);
        for (auto& l : text) {
            m = std::max(m, static_cast<int>(l.size()));
        }
        int k = 0;
        for (int j = 0; j < m; ++j) {
            if (text[n - 1].size() > j && text[n - 1][j] != ' ') {
                for (int i = 0; i < n - 1; ++i) {
                    v[i].emplace_back();
                }
                ops.push_back(text[n - 1][j]);
                k = 0;
            } else {
                k++;
            }
            for (int i = 0; i < n - 1; ++i) {
                if (text[i].size() > j && text[i][j] != ' ') {
                    v[k].back() = v[k].back() * 10 + text[i][j] - '0';
                }
            }
        }
    }
    for (int i = 0; i < ops.size(); ++i) {
        long long x = v[0][i];
        for (int j = 1; j < v.size(); ++j) {
            if (ops[i] == '+') {
                x += v[j][i];
            } else if (ops[i] == '*') {
                x *= std::max(1LL, v[j][i]);
            }
        }
        res += x;
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(5, utils::FileReader& reader, int part) {
    long long res = 0;
    std::vector<std::pair<long long, long long>> v;
    for (auto line : reader) {
        long long a = 0, b = 0;
        if (!line.read("%lld-%lld", &a, &b)) {
            break;
        }
        v.emplace_back(a, b);
    }
    if (part == 1) {
        for (auto line : reader) {
            long long x = 0;
            if (!line.read("%lld", &x)) {
                break;
            }
            bool in = false;
            for (const auto [a, b] : v) {
                if (a <= x && x <= b) {
                    in = true;
                    break;
                }
            }
            if (in) {
                res++;
            }
        }
    } else {
        std::sort(v.begin(), v.end());
        long long s = 0, e = -1;
        for (const auto [a, b] : v) {
            if (a <= e) {
                e = std::max(e, b);
            } else {
                res += e - s + 1;
                s = a;
                e = b;
            }
        }
        res += e - s + 1;
    }

    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(4, utils::FileReader& reader, int part) {
    long long res = 0;
    auto [map, n, m] = reader.allLines();
    bool done = false;
    while (!done) {
        done = true;
        for (int i = 0; i < n; ++i) {
            const auto& line = map[i];
            for (int j = 0; j < m; ++j) {
                if (line[j] == '@') {
                    int c = 0;
                    for (auto d : utils::dir::ALL8) {
                        const auto p = utils::Coord(i, j) + d;
                        if (p.inside(n, m) && map[p.i][p.j] != '.') {
                            c++;
                        }
                    }
                    if (c < 4) {
                        res++;
                        if (part == 2) {
                            done = false;
                            map[i][j] = '.';
                        }
                    }
                }
            }
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(3, utils::FileReader& reader, int part) {
    long long res = 0;
    for (auto line : reader) {
        const auto l = line.get();
        long long v = 0;
        auto s = l.begin();
        for (int k = part == 2? 11: 1; k >= 0; --k) {
            const auto it = std::max_element(s, l.end() - k);
            v = v * 10 + *it - '0';
            s = it + 1;
        }
        res += v;
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(2, utils::FileReader& reader, int part) {
    long long res = 0;
    reader.nextLine();
    auto line = reader.getLine();
    for (auto range : line.split(",")) {
        long long a = 0, b = 0;
        sscanf(range.data(), "%lld-%lld", &a, &b);
        // try a smart scan, by computing parts of number in the range
        // and assembling them by repetition
        std::unordered_set<long long> nbs;
        int d = utils::digits(a);
        for (auto x = a; x <= b; x = std::pow(10, d), ++d) {
            for (int k = 2; k <= d; ++k) {
                if (d % k == 0) {
                    long long q = x / std::pow(10, d - d / k);
                    const long long e = std::pow(10, d / k);
                    while (q < e) {
                        auto w = q;
                        for (int i = 1; i < k; ++i) {
                            w = w * e + q;
                        }
                        if (w >= a && w <= b) {
                            nbs.insert(w);
                        }
                        q++;
                    }
                }
                if (part == 1) break;
            }
        }
        for (const auto nb : nbs) {
            res += nb;
        }
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
DAY(1, utils::FileReader& reader, int part) {
    long long res = 0;
    int dial = 50;
    for (auto line : reader) {
        char c = 0;
        int a = 0;
        if (!line.read("%c%d", &c, &a)) {
            throw utils::MyException("error reading dial");
        }
        const int d = c == 'L'? -1: 1;
        const int rot = d * a;
        if (part == 2) {
            // count all other passes through 0
            int d1 = dial + rot - d;
            if (d1 <= 0) d1 = -d1 + (dial != 0? 100: 0);
            res += d1 / 100;
        }
        dial = ((dial + rot) % 100 + 100) % 100;
        res += dial == 0;
    }
    printf("Res%d = %lld\n", part, res);
}
//-----------------------------------------------------------------------------
