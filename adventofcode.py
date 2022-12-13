import argparse, heapq, re


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


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Solve problems on https://adventofcode.com/2022/')
    parser.add_argument('day', metavar='d', type=str, help='problem to solve')

    args = parser.parse_args()
    locals()[str(args.day)]()