import argparse, heapq, re
from functools import reduce, cmp_to_key


def day1():
    with open('input1.txt') as f:
        c = 0
        h = []
        for line in f:
            if line and not line.isspace():
                c += int(line)
            else:
                heapq.heappush(h, c)
                c = 0
        print(sum(heapq.nlargest(3, h)))


def day2():
    with open('input2.txt') as f:
        s = 0
        # part 1: X,Y,Z mean rock, paper, scissors
        r1 = {"AX": 1+3, "AY": 2+6, "AZ": 3+0,
             "BX": 1+0, "BY": 2+3, "BZ": 3+6,
             "CX": 1+6, "CY": 2+0, "CZ": 3+3}
        # part 2: X,Y,Z mean lose, draw, win
        r2 = {"AX": 3+0, "AY": 1+3, "AZ": 2+6,
             "BX": 1+0, "BY": 2+3, "BZ": 3+6,
             "CX": 2+0, "CY": 3+3, "CZ": 1+6}
        for line in f:
            a, b = line.split()
            s += r2[a + b]
        print(s)


def day3():
    prio = lambda x: ord(x) - (ord('a') - 1 if x >= 'a' else ord('A') - 27)
    with open('input3.txt') as f:
        p = 0
        for line in f:
            m = len(line) // 2
            a = set(line[:m]) & set(line[m:])
            p += prio(a.pop())
        print(p)
    with open('input3.txt') as f:
        p = 0
        for g in zip(*[iter(list(f))] * 3):
            gs = [s.strip() for s in g]
            a = set(gs[0]) & set(gs[1]) & set(gs[2])
            p += prio(a.pop())
        print(p)


def day4():
    with open('input4.txt') as f:
        p = 0
        o = 0
        for line in f:
            a,b = [[int(x) for x in i.split('-')] for i in line.split(',')]
            cont = lambda a,b, x,y: a <= x and y <= b
            over = lambda a,b, x,y: a <= y and b >= x
            if cont(*a, *b) or cont(*b, *a):
                p += 1
            if over(*a, *b):
                o += 1
        print(p, o)


def day5():
    with open('input5.txt') as f:
        readingStack = True
        ss = [''] * 9  # hardcoded no of stacks, each being a string
        for line in f:
            if readingStack:
                if line[1] == '1':
                    readingStack = False
                    continue
                for i in range(len(ss)):
                    if line[1 + 4 * i] != ' ':
                        ss[i] = line[1 + 4 * i] + ss[i]
            else:
                m = re.match('move (\d+) from (\d+) to (\d+)', line)
                if m:
                    n, f, t = [int(x) for x in m.group(1,2,3)]
                    w = ss[f - 1][-n:]
                    ss[f - 1] = ss[f - 1][:-n]
                    ss[t - 1] += w # [::-1]
        print(''.join([s[-1] for s in ss]))


def day6():
    with open('input6.txt') as f:
        s = f.read()
        p = 0
        L = 14
        while p < len(s) - L:
            if len(set(s[p: p + L])) == L:
                break
            p += 1
        print(p + L)


def day7():
    class Node:
        def __init__(self, name, parent):
            self.name = name
            self.parent = parent
            self.items = {}

        def add(self, name, size=0):
            if name not in self.items:
                self.items[name] = (Node(name, self), size)
            return self.items[name][0]

        def dirSizes(self):
            res = []
            cs = 0
            for _,el in self.items.items():
                d,size = el
                if size == 0:
                    ds = d.dirSizes()
                    cs += ds[0]
                    res += ds
                else:
                    cs += size
            return [cs] + res

    with open('input7.txt') as f:
        cd = None
        root = None
        for line in f:
            line = line.strip()
            if line.startswith("$ cd "):
                d = line[5:]
                if d == '/':
                    root = cd = root if root else Node(d, None)
                elif d == '..':
                    cd = cd.parent
                else:
                    cd = cd.add(d)
            elif line.startswith("$ ls"):
                pass
            elif line.startswith("dir "):
                cd.add(line[4:])
            else:
                size, name = line.split()
                cd.add(name, int(size))
        sizes = root.dirSizes()
        print(sum(s for s in sizes if s <= 100000))

        needed = 30000000 - (70000000 - sizes[0])
        print(min(x for x in sorted(sizes) if x >= needed))


def day8():
    with open('input8.txt') as f:
        a = [[int(x) for x in line.strip()] for line in f]
        v = [[False] * len(r) for r in a]

        def mark(n, m, idx):
            for i in range(n):
                t = -1
                for j in range(m):
                    ii, jj = idx(i, j)
                    if a[ii][jj] > t:
                        v[ii][jj] = True
                        t = a[ii][jj]

        n, m = len(v), len(v[0])
        mark(n, m, lambda i, j: (i, j))
        mark(n, m, lambda i, j: (i, m - j - 1))
        mark(m, n, lambda i, j: (j, i))
        mark(m, n, lambda i, j: (n - j - 1, i))
        print(sum(sum(r) for r in v))

        z = [[0] * m for i in range(n)]
        for i in range(1, n - 1):
            for j in range(1, m - 1):
                k = []
                for di,dj in [[0, -1], [-1, 0], [0, 1], [1, 0]]:
                    ii = i + di
                    jj = j + dj
                    while ii in range(1, n - 1) and jj in range(1, m - 1) and a[i][j] > a[ii][jj]:
                        ii += di
                        jj += dj
                    k.append(abs(i - ii + j - jj))
                z[i][j] = reduce(lambda x, y: x * y, k)
        print(max(max(x) for x in z))


def day9():
    with open('input9.txt') as f:
        dirs = {'R': (0, 1), 'U': (1, 0), 'L': (0, -1), 'D': (-1, 0)}
        ph = (0, 0)
        pt = [(0, 0)] * 9
        visited = set()
        norm = lambda x: x // 2 if abs(x) > 1 else x
        for line in f:
            dir, n = line.split()
            for _ in range(int(n)):
                ph = tuple(map(sum, zip(ph, dirs[dir])))
                p = ph
                for i in range(len(pt)):
                    d = (p[0] - pt[i][0], p[1] - pt[i][1])
                    if any(abs(x) > 1 for x in d):
                        pt[i] = (pt[i][0] + norm(d[0]), pt[i][1] + norm(d[1]))
                    p = pt[i]
                visited.add(pt[-1])
        print(len(visited))


def day10():
    with open('input10.txt') as f:
        rx = 1
        cycle = 0
        th = 20
        total = 0

        screen = ''
        N = 40

        for line in f:
            isAdd = line.startswith("addx")
            for i in range(1 + isAdd):
                screen += '#' if rx - 1 <= cycle % N <= rx + 1 else '.'
                cycle += 1
            if cycle >= th:
                total += th * rx
                th += 40
            rx += int(line[5:]) if isAdd else 0
        print(total)
        for i in range(0, len(screen), N):
            print(screen[i:i + N])


def day11():
    class Monkey:
        def __init__(self):
            self.items = []
            self.oper = None
            self.divBy = 1
            self.dest = [0] * 2
            self.inspected = 0

    def readFrom(line, regex):
        m = re.match(regex, line)
        return m.group(1) if m else None

    with open('input11.txt') as f:
        lines = f.readlines()
        monkeys = []
        # opers = {'^': lambda x,v: x * x, '*': lambda x,v: x * v, '+': lambda x,v: x + v}
        for i in range(0, len(lines), 7):
            m = Monkey()
            items = readFrom(lines[i + 1], '  Starting items: ((\d+, )*\d+)')
            m.items = [int(x) for x in items.split(", ")]
            m.oper = readFrom(lines[i + 2], '  Operation: ([^\n]+)')
            # m.oper, m.operVal = (opers['^'], 0) if oper == '* old' else (opers[oper[0]], int(oper[1:]))
            m.divBy = int(readFrom(lines[i + 3], '  Test: divisible by (\d+)'))
            m.dest[0] = int(readFrom(lines[i + 4], '    If true: throw to monkey (\d+)'))
            m.dest[1] = int(readFrom(lines[i + 5], '    If false: throw to monkey (\d+)'))
            monkeys.append(m)

        MOD = 9699690
        for _ in range(10000):
            for m in monkeys:
                for item in m.items:
                    env = {'old': item, '__builtins__': {}}
                    exec(m.oper, env)
                    val = env['new'] # // 3
                    # val = m.oper(item, m.operVal) # // 3
                    dest = m.dest[val % m.divBy != 0]
                    monkeys[dest].items.append(val % MOD)
                m.inspected += len(m.items)
                m.items.clear()

        top = sorted([x.inspected for x in monkeys], reverse=True)[:2]
        print(top[0] * top[1])


def day12():
    def traverse(S, e, m, stepCond):
        dirs = [(0,1), (1,0), (0,-1), (-1,0)]
        s = [(S, 1)]
        visited = {} # can be used to trace back too
        dist = 0
        while s and not dist:
            p, cd = s.pop(0)
            a = m[p[0]][p[1]]
            for d in dirs:
                ii, jj = (p[0] + d[0], p[1] + d[1])
                if 0 <= ii < len(m) and 0 <= jj < len(m[0]) and (ii, jj) not in visited:
                    if stepCond(a, m[ii][jj]):
                        if m[ii][jj] == e:
                            dist = cd
                            break
                        s.append(((ii, jj), cd + 1))
                        visited[(ii, jj)] = p
        return dist

    with open('input12_test.txt') as f:
        m = []
        S = None
        E = None
        for i, line in enumerate(f):
            if 'S' in line:
                S = (i, line.index('S'))
                line = line[:S[1]] + chr(ord('a') - 1) + line[S[1] + 1:]
            if 'E' in line:
                E = (i, line.index('E'))
                line = line[:E[1]] + chr(ord('z') + 1) + line[E[1] + 1:]
            m.append(line.strip())

        print(traverse(S, m[E[0]][E[1]], m, lambda a,b: chr(ord(a) + 1) >= b))
        print(traverse(E, 'a', m, lambda a,b: b >= chr(ord(a) - 1)))


def day13():
    def compare(a, b):
        if type(a) == type(b):
            if type(a) is int:
                return a - b
            for i in range(min(len(a), len(b))):
                r = compare(a[i], b[i])
                if r:
                    return r
            return len(a) - len(b)
        elif type(a) is int:
            return compare([a], b)
        else:
            return compare(a, [b])

    with open('input13.txt') as f:
        lines = f.readlines()
        total = 0
        d1, d2 = [[2]], [[6]]
        all = [d1, d2]
        for i in range((len(lines) // 3) + 1):
            a, b = eval(lines[3 * i]), eval(lines[3 * i + 1])
            r = compare(a, b)
            if r <= 0:
                total += i + 1
            all += [a, b]
        print(total)
        all.sort(key=cmp_to_key(compare))
        print((all.index(d1) + 1) * (all.index(d2) + 1))


def day14():
    with open('input14.txt') as f:
        partTwo = True
        xmin, xmax = 500, 500
        ymax = 0
        lines = []
        for line in f:
            a = [[int(x) for x in p.split(',')] for p in line.split(' -> ')]
            xmin = min(xmin, min(x[0] for x in a) - 1)
            xmax = max(xmax, max(x[0] for x in a) + 1)
            ymax = max(ymax, max(x[1] for x in a) + 1)
            lines.append(a)
        if partTwo:
            xmin, xmax = min(xmin, 500 - ymax), max(xmax, 500 + ymax)

        m = [['.'] * (xmax - xmin + 1) for _ in range(ymax + 1)]
        for a in lines:
            for i in range(len(a) - 1):
                p = a[i]
                d = [(x - y) // (abs(x - y) or 1) for x,y in zip(a[i + 1], a[i])]
                while True:
                    m[p[1]][p[0] - xmin] = '#'
                    if p == a[i + 1]:
                        break
                    p = [p[0] + d[0], p[1] + d[1]]
        # for a in m: print(''.join(a))
        n = 0
        start = [500 - xmin, 0]
        while m[start[1]][start[0]] == '.':
            p = start
            while m[p[1]][p[0]] != 'o':
                if p[1] == ymax:
                    if not partTwo:
                        m[start[1]][start[0]] = '~' # falling out, mark as done
                    else:
                        m[p[1]][p[0]] = 'o' # bottom, stop here
                        n += 1
                    break
                for dx,dy in [[0, 1], [-1, 1], [1, 1]]:
                    pp = p[0] + dx, p[1] + dy
                    if m[pp[1]][pp[0]] == '.':
                        p = pp
                        break
                else:
                    m[p[1]][p[0]] = 'o'
                    n += 1
        print(n)


def day15():
    with open('input15.txt') as f:
        data = []
        for line in f:
            m = re.match("Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+)", line)
            data.append([int(x) for x in m.group(1,2,3,4)])

        def intervals(Y):
            res = []
            for sx,sy,bx,by in data:
                md = abs(bx - sx) + abs(sy - by)
                d = md - abs(sy - Y)
                if d >= 0:
                    res.append((sx - d, sx + d))
            return sorted(res)

        Y = 2000000
        total = -len(set(bx for sx,sy,bx,by in data if by == Y))
        le = -1e10
        for s,e in intervals(Y):
            s = max(s, le)
            if s < e:
                total += e - s + 1
                le = e + 1
        print(total)

        for y in range(0, 4000001):
            le = 0
            for s,e in intervals(y):
                if s > le:
                    print(y, le, s, y + 4000000 * (s - 1))
                le = max(e, le)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Solve problems on https://adventofcode.com/2022/')
    parser.add_argument('day', metavar='d', type=str, help='problem to solve')

    args = parser.parse_args()
    locals()[str(args.day)]()