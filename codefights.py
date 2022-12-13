from fractions import gcd
from math import *
from collections import deque
import re

def subarraysearch(a, s):
  i = 0
  for c in s.lower():
    i = a.lower().find(c,i) + 1
    if i == 0: return False
  return True

def Fraction(a, b):
  s = a + b;
  while b: a, b = b, a%b
  return s/a

def pie(a):
  r = 4
  i = 3
  while a :
    r = 4./i - r
    i += 2
    a -= 1
  return abs(r)

def SC(s):
  return s.swapcase()

def PlusMinus(N, A):
  v = [0] * 3
  for x in A:
    v[0 if x > 0 else 1 if x < 0 else 2] += 1.
  return [round(x/N,3) for x in v]

def Fraction(a, b):
  x,y = a,b
  while y: x, y = y, x%y
  return str(a/x) + '/' + str(b/x)

def LastThreeDigits(k):
  def f(x,y):
    for x in range(x, 1002):
      if (x + y) ** 3 % 1000 == k:
        return x
    return -1
  a = f(0,0)
  return [a, f(1,a)]

def PascalTriangleRow(r):
  a = [1]
  for i in range(1,r+2):
    a = [sum(a[max(0,j-1):j+1]) for j in range (0,i)]
  return a

def NumberofZeros(n):
  i = 5
  t = 0
  while i <= n:
    t += n / i
    i *= 5
  return t

def findRange(a, n):
  i = s = 0
  for x in a:
    s += x
    while s > n:
      s -= a[i]
      i += 1
    if s and s == n:
      return True
  return False

def PlayWithStringsIII(S, k):
  n = len(S)
  mx = 0
  for i in range(n):
    s = set()
    for j in range(i, n):
      s.add(S[j])
      if len(s) == k:
        mx = max(mx, j-i+1)
  return mx

def rounding(n, m):
  return m * ((n + (m - 1)/2) / m)

def NumberGameII(n):
  v = [0, 1]
  for i in range (2, n):
    v.append(0)
    for x in range (1, ceil(log(i,2)) + 1):
      v[i] |= not v[i - x]
  return bool(v[n - 1])

def perm_sum(n):
  s = 0
  x = 1
  e = 0
  i = 1
  while n:
    s += n % 10
    n /= 10
    if n: x *= i
    e = e*10 + 1
    i += 1
  return s * x * e

Game = lambda a, b, c: ["First","Second"][not (a ^ b ^ c)]

def Fibonnaci(n):
    a, b = 0, 1
    while n:
      a, b = b, a + b
      n -= 1
    return a

def botWar(n):
    t = 0
    p = [1] * n
    for i in range (2, n-1):
        if p[i]:
            j = 2
            while i*j < n:
                p[i*j] = 0
                j += 1
            if i >= n/2 and p[n-i]:
                t += 1
    return t

def ShortestPathI(D, k):
    d = [-1] * len(D)
    q = deque([(0,0)])

    while any(x < 0 for x in d):
        m = 0
        for i,a in q:
            for j, b in enumerate(D[i]):
              if b:
                if not m or a + b:
                    m = a + b
                    z = j
        q.append((z, m))
        i,a = q.popleft()
        if d[i] < 0 and a % k == 0:
            d[i] = a
        print (z, m)
    return d

def Operation(a, o, b):
    if o == '/^': return a ** (1./b)
    if o == '^': o = '**'
    return eval('%d%s%d' % a, o, b)

Calculator = lambda a, o, b: int(a ** (1./b)+.1) if o == '/^' else a ** b if o == '^' else eval('%d%s%d' % a, o, b)

def Revamp(S):
    a = [sorted(s) for s in S.split()]
    q = [''.join(s) for x, s in sorted([(sum([ord(x) for x in s]), s) for s in a])]
    return ' '.join(q)

def LCM_2(N):
    n = 1
    for i in range(2, N+1):
        x = n
        y = i
        while y: x, y = y, x%y
        n = n * i / x
    return n % 10 ** 8

def KnightFight(p, d, K):
    if p == d: return 0
    l = 1
    q = [(l,p)]
    z = [[1]*9 for i in range(9)]
    while l <= K:
        l, (a,b) = q.pop(0)
        i,j = ord(a) - 96, int(b)
        for x,y in [(-2,-1),(-1,-2),(1,-2),(2,-1),(-2,1),(-1,2),(1,2),(2,1)]:
            x += i
            y += j
            if 0 < x < 9 and 0 < y < 9 and z[x][y]:
                w = chr(96 + x) + str(y)
                q.append((l+1, w))
                z[x][y] = 0
                if w == d:
                    return l
    return -1

def queens(Q):
    t = 64
    r = range(1,9)
    for x in r:
        for y in r:
            for q in Q:
                i,j = ord(q[0]) - 96, int(q[1])
                if x == i or y == j or x - i == y - j or x - i == j - y:
                    t -= 1
                    break
    return t

def digitalRoot(A, B):
    df = lambda x: 1 + ((x - 1) % 9)
    return sorted([df(int(A) + i) for i in range(df(int(B) - int(A) + 1))])

def GoodYears(a, b):
    t = 0
    a = int(a)
    b = int(b)
    for s in range(int(log(a,2)) + 1, int(log(b,2)) + 2):
        for i in range(s - 1):
            if a <= (1 << s) - (1 << i) - 1 <= b:
                t += 1
    return t

def sternBrocotTree(x, y):
    a, b = (1, 0), (0, 1)
    s = ''
    while x * b[1] != b[0] * y:
        q = a[0] + b[0], a[1] + b[1]
        if x * q[1] >= q[0] * y:
            b = q
            s += 'A'
        else:
            a = q
            s += 'B'
    return s if (x, y) == b else 'Impossible'

def ZigZag(n):
    r = []
    x = [(a-b)/abs(a-b) for a,b in zip(n[1:], n[:-1])]
    a = 1
    q = 0
    for b in x:
        if b != a:
            r += [q]
            q = 1
        else:
            q += 1
        a = b
    return r + [q]

def DigiRoot(A):
    q = 1
    t = 0
    for x in A:
        q *= x
        t = 1 + ((t+q-1) % 9)
    return t

def WiFi(s, t):
    f = factorial
    d = s.count('+') - s.count('-') - t.count('+') + t.count('-')
    n = t.count('?')
    k = (n - d) / 2
    return 0 if n < d or (n - d) % 2 else pow(2, -n) * f(n) / f(k) / f(n-k)

def findSongs(s):
    s = [p.split(':') for p in s.split(',')]
    q = {}
    m = 0
    for n, w in s:
        if sum(1 for x,y in s if y == w) > 1:
            q[n] = q.get(n,0) + 1
            m = max(m, q[n])
    r = []
    for n, w in s:
        if n not in r and n in q and m == q[n]:
            r.append(n)
    return r

def NewYearandDays(n, t):
    if t[0] == 'w':
        return 52 if 4 < n < 7 else 53
    return 12 if n < 30 else 11 if n < 31 else 7

def numberOfYear(k):
    a = [0, 1, 2]
    t = 1
    while k-1:
        b = [0] * 3
        for i in range(3):
            for x in [0,1,2,5]:
                b[(i + x) % 3] += a[i]
        a = b
        t += a[0]
        k -= 1
    return t

def Win_or_Lose(p):
    return ["Alex","Bill"][not reduce(lambda x,y: x^y, p)]

def TheNextPalindrome(K):
    n = len(K)
    m = n / 2
    r = K[:n-m] + K[m-1::-1]
    if r <= K:
        a = str(int(K[:n-m]) + 1)
        r = a + a[m-1::-1]
    return r

def MahjongPureHand(t):
    def f(s, t=0):
        if not s: return t == 1
        a = s.pop(0)
        n = s.count(a)
        if (n and f(s[1:], t+1)) or (n > 1 and f(s[2:], t)):
            return 1
        if a+1 in s and a+2 in s:
            s.remove(a+1)
            s.remove(a+2)
            return f(s, t)
        return 0
    r = ''
    for c in '123456789':
        if t.count(c) < 4:
            q = sorted(ord(x)-48 for x in t + c)
            if f(q):
                r += c
    return r

def visualSequence(s, n):
    while n:
        p = 0
        k = 0
        r = ''
        for c in str(s) + ' ':
            if c == p:
                k += 1
            else:
                if k:
                    r += str(k) + p
                p = c
                k = 1
        s = int(r)
        n -= 1
    return s

def Encryption(m, k):
    return ''.join([chr((ord(c) & 96) + ((ord(c) & 31) - 1 + k) % 26 + 1) for c in m])

def savePepe(m):
    r = ''
    p = 0
    for x in m:
        p = 129*(x^p) % 256
        r += chr(p)
    return r

def taster(n, m):
    if n > m: return taster(n - m, m)
    x = m / n
    return (x - 1) * n if x * n == m else x * n + taster(n, m - x * n)

def MaximumString(n):
    m = -1
    for j in range(1, len(n)):
        for i in range(1, j):
            a = [n[:i], n[i:j], n[j:]]
            t = -1
            for x in a:
                if (x[0] == '0' and len(x) > 1) or int(x) > 1e6:
                    t = -1
                    break
                t += int(x)
            if m < t: m = t + 1
    return m

def whatIteration(a):
    t = 1
    while 1:
        b = 1
        for i in range(len(a) - 1):
            if a[i] > a[i+1]:
                a[i], a[i+1] = a[i+1], a[i]
                b = 0
        if b: return t
        t += 1

def num_to_eng(n):
    x = 'zero one two three four five six seven eight nine ten eleven twelve'.split()
    y = 'twen thir four fif six seven eigh nine'.split()
    return x[n] if n < 13 else y[n-12] + 'teen' if n < 20 else (y[n/10-2] if n/10 != 4 else 'for') + 'ty' + ('-' + x[n%10] if n%10 else '')

def Stairway(n, d={}):
    if n not in d:
        d[n] = n if n < 4 else (Stairway(n/2-1, d)**2 + Stairway(n/2, d)**2 if n % 2 == 0 else (Stairway(n/2-1, d) + Stairway(n/2+1, d)) * Stairway(n/2, d)) % 1000000007
    return d[n]

def removeBadMembersfor(a):
    while not a[0]: a.pop(0)
    while not a[-1]: a.pop()
    return a

def good_numbers(a):
    d = {}
    def S(n):
        if n not in d:
            d[n] = n if n < 4 else (S(n/2-1)**2 + S(n/2)**2 if n % 2 == 0 else (S(n/2-1) + S(n/2+1)) * S(n/2)) % 1000000007
        return d[n]
    return [S(int(x)) for x in a]

def offer(n, p):
    return sum(p for i,p in enumerate(reversed(sorted(p))) if i%3 - 2)

def pascalMatrix(n):
    a = [0] * (n/2)
    z = a + [1] + a
    r = [z]
    for q in range(n/2):
        z = [z[i-1] + z[(i+1)%n] for i in range(n)]
        r += [z]
    return r

def HousePicture(p):
    h = ["bwwbbbbb",
         "bbbbbyyb",
         "bbrbbyyb",
         "brrrbbbb",
         "rrrrrbbb",
         "brrrbbbb",
         "brrrbbbb",
         "gggggggg"]
    r = []
    x,y = len(p), len(p[0])
    for i in range(9 - x):
        for j in range(9 - y):
            ok = 1
            for k in range(x):
                if h[i+k][j:j+y] != p[k]:
                    ok = 0
                    print i, h[i+k][j:j+y]
                    break
            if ok:
                r += [[i,j]]
    return r

def replaceWords(s):
    s = s.split()
    q = sorted(s, key=lambda w: len(w))
    q = [q[len(s) - q.index(w) - 1] for w in s]
    return ' '.join([q[i].lower() if q[i] != 'I' and i else q[i].title() for i in range(len(q))])

def CakewalkSimulator(s, d):
    x = s if 'r' in d else -s if 'l' in d else 1
    y = s if 'd' in d else 1
    r = [[0] * abs(x) for i in range(abs(y))]
    q = abs(s)
    for i in range(q):
        j = i if x > 0 else i+1
        k = i if y > 0 else i+1
        print x/q*j, y/q*k
        r[y/q*k][x/q*j] = i+1
    return r

def MatrixMultiplication(a, b):
    return [[sum([a[i][k]*b[k][j] for k in range(3)]) for j in range(3)] for i in range(3)]

def distSameLetter(s):
    q = [s.rfind(s[i]) - i for i in range(len(s))]
    m = q.index(max(q))
    return s[m] + str(q[m]+1)

def Zeros(N):
    t = 0
    d = 5
    while d <= N:
        t += N / d
        d *= 5
    return t

def formRectangle(A, B, C, D):
    print 'old=', all(len(set(x)) == 2 for x in zip(A, B, C, D))
    d = lambda (x,y), (a,b): (x-a) ** 2 + (y-b) ** 2
    return d(A,B) == d(C,D) and d(A,D) == d(B,C) and d(A,C) == d(B,D)

def GoldMineRace(c, s, g):
    d = lambda (x,y), (a,b): ((x-a) ** 2 + (y-b) ** 2) ** 0.5
    q = [d(c[i], g) / s[i] for i in range(len(c))]
    for w in sorted(q):
        if q.count(w) == 1:
            return q.index(w)
    return -1

def highestPower(N, A):
    d = 2
    p = []
    q = A
    while d*d <= q:
        n = 0
        #print q, d, q % d
        while q % d == 0:
            n += 1
            q /= d
        if n:
            p += [(d, n)]
        d += 1
    if q-1:
        p += [(q, 1)]
    #print p
    m = N
    for d, n in p:
        t = 0
        q = d
        while q <= N:
            t += N / q
            q *= d
        #print d, n, t
        m = min(m, t/n)
    return m

def Expand_It(s, k):
    for x in sorted(re.findall('[a-z]\d*', s)):
        k -= int(x[1:])
        if k <= 0:
            return x[0]
    return "-1"

def backpack(n, M, W, P):
    x = [0] * (M + 1)
    for w,p in zip(W,P):
        for i in range(M - w, -1, -1):
            if not i or x[i]:
                x[i + w] = max(x[i + w], x[i] + p)
    print [y for y in x if y]
    return x[M] or -1

#i=2, j=4, n=5 => (6-4, 2), (

def rotateImage(a):
    n = len(a)
    for i in range(n / 2 + 1):
        for j in range(i, n - i - 1):
            print i, j
            q = a[i][j]
            a[i][j] = a[n - 1 - j][i]
            a[n - 1 - j][i] = a[n - 1 - i][n - 1 - j]
            a[n - 1 - i][n - 1 - j] = a[j][n - 1 - i]
            a[j][n - 1 - i] = q
    return a

def textDisplay(k):
    d = { 186: ';', 188: ',', 190: '.', 222: "'" }
    r = []
    i = 0
    for c in k:
        if c == 8: # backspace
            if i:
                i -= 1
                del r[i]
        elif c == 35: # end
            i = len(r)
        elif c == 36: # home
            i = 0
        elif c == 37: # <-
            if i:
                i -= 1
        elif c == 39: # ->
            if i < len(r):
                i += 1
        elif c == 46: # delete
            if i < len(r):
                del r[i]
        else:
            r.insert(i, chr(c) if c < 100 else d[c])
            i += 1
    return lower(''.join(r))

def teeNine(m):
    def n(c): 
        k = ord(c)
        return (k - k/113 - k/120 - 91) / 3
    def r(x): 
        return chr(91 + int(x * 3) + x/8)
    def w(c): 
        q = chr(ord(c)+1)
        return q if n(c) == n(q) else r(n(c))
    x = [r(n(c.lower())) if c.isalnum() else c for c in m]
    y = []
    for i in range(len(x)):
        if i and x[i].isalnum() and x[i] == x[i-1]:
            y[-1] = w(y[-1])
        else:
            y += [x[i]]
    return ''.join(y)

def couldBeAnagram(s1, s2):
    for c in s1:
        if c != '?' and c in s2:
            s2 = s2.replace(c, '', 1)
    return len(s2) == s1.count('?')

def minesweeper3(g, m):
    ok = 1
    n = len(g)
    v = [[0] * n for i in range(n)]
    def click(i, j):
        if 0 <= i < n and 0 <= j < n:
            if g[i][j] == 9:
                ok = 0
            if v[i][j] == 0:
                v[i][j] = 1
                if g[i][j] == 0:
                    for x,y in [[-1,0], [0,-1], [1,0], [0,1]]:
                        if click(i+x, j+y):
                            ok = 0
    for i,j,a in m:
        if a:
            if v[i][j] % 2 == 0:
                v[i][j] = 2 - v[i][j]
        else:
            click(i,j)
    return v if ok else []

print "Test ground"
print minesweeper3([[2,9,1,0,0], [9,2,1,0,0], [1,2,1,1,0], [0,1,9,1,0], [0,1,1,1,0]],  [[1,3,0], [3,2,1], [2,2,0], [3,1,0]])
#print couldBeAnagram("n?ce", "nice")
#print teeNine("a cap of tea. Oh noooooo!!")
#print textDisplay([72, 69, 89, 32, 89, 79, 85, 32, 68, 73, 68, 32, 73, 84, 32, 87, 82, 79, 78, 71, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 188, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 46, 46, 8, 8, 46, 46, 8])
#print rotateImage([[1,2,3,4,5], [6,7,8,9,10], [11,12,13,14,15], [16,17,18,19,20], [21,22,23,24,25]])
#print backpack(7, 100, [80,10,20,10,100,30,55], [8,20,5,1,10,30,20])  #29
#print Expand_It("a2b3c12a1", 8)
#print highestPower(9, 6)
#print GoldMineRace([[-3,-4],[3,4],[-3,4],[3,-4]], [5, 5, 2, 2], [0, 0])
#print formRectangle([0,-1],[1,0],[0,1],[-1,0])
#print Zeros(7)
#print distSameLetter("fffffahhhhhhaaahhhhbhhahhhhabxx")
#print CakewalkSimulator(-3, "downright")
#print replaceWords("I am better than him")
#print HousePicture(["bby","rbb"])
#print pascalMatrix(9)
#print offer(7, [10, 3, 2, 4, 6, 4, 9])
#print good_numbers([str(i) for i in range(20)])
#print Stairway(21)  # 21=17711
#print removeBadMembersfor([0, 9, 5, 0, 0, 0, 0, 2, 0, 0]) # [9, 5, 0, 0, 0, 0, 2]
#print num_to_eng(23)
#print MaximumString("1223334455678")
#print taster(7, 2)
#print savePepe([241,134,146,23,6,141,140,156,134,31,145,146,23,2,129,15])
#print visualSequence(1, 0)
#print MahjongPureHand("1223334455678")
#print Win_or_Lose([3, 1, 1])
#print numberOfYear(30)
#print findSongs("song1:wordA,song2:wordA")
#print WiFi("+++--+-+--+--+-++-+-+-++--+", "++-+-+-+-??-++-+?-++++--?--")
#print DigiRoot([2,3,4])
#print ZigZag([1,0,4,5])
#print sternBrocotTree(8, 5)
#print GoodYears('72057594000000000', '72057595000000000')
#print digitalRoot("12", "22")
#print queens(["a8","b2"])
#print KnightFight("f5", "a2", 7)
#print LCM_2(10)
#print Revamp("batman is bruce wayne")
#print Calculator(7**6, '/^', 6)
#print ShortestPathI([[0,2,3,-1,7], [2,0,-1,9,6], [3,-1,0,8,4], [-1,9,8,0,5], [7,6,4,5,0]], 3)
#print botWar(50)
#print Fibonnaci(5)
#print Game(4,0,4)
#print perm_sum(1)
#print NumberGameII(2)
#print rounding(1,3000)
#print PlayWithStringsIII("abcbaaaxy", 3);
#print findRange([1,2,3,4,5], 12)
#print NumberofZeros(25)
#print PascalTriangleRow(7)
#print LastThreeDigits(0)
#print Fraction(2,4)
#print PlusMinus(6, [-4,3,-9,0,4,1])
#print SC("abc123")
#print pie(4)
#print Fraction(90,120)
#print subarraysearch("ABCdefgh","bCf")
