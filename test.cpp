/*
 * test.cpp
 *
 * Author: Mihai Feier, Jan 9, 2015
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
#include <iomanip>
#include <sstream>
#include <new>

using namespace std;

class Counter
{
    long nb_op;
    long nb_asg;
public:
    Counter() { reset(); }
    void op() { nb_op++; }
    void asg() { nb_asg++; }
    void reset() { nb_op = nb_asg = 0; }
    void print() { printf("Nasg=%d\nNop=%d\n", nb_asg, nb_op); }

    friend class Number;
};

Counter ctr;

class Number
{
private:

    int x;
public:
    Number(int y=0) : x(y) { /*printf("[%d]", y);*/ }
    Number(const Number &y) : x(y.x) { /*printf("{%d}", y.x);*/ }

    int val() const { return x; }
    //const Number operator+(const Number& y) const { return Number(x + y.x); }
    Number& operator=(const Number& y) { ctr.asg(); x = y.x; return *this; }
    int operator==(const Number& y) const { ctr.op(); return x == y.x; }
    int operator!=(const Number& y) const { ctr.op(); return x != y.x; }
    int operator>(const Number& y)  const { ctr.op(); return x > y.x; }
    int operator>=(const Number& y) const { ctr.op(); return x >= y.x; }
    int operator<(const Number& y)  const { ctr.op(); return x < y.x; }
    int operator<=(const Number& y) const { ctr.op(); return x <= y.x; }
};
//const Number operator+(int x, const Number& y) { return y + x; }

static void testNumber() {
    Number array[100], swap;
    int n, c, d;

      printf("Enter number of elements\n");
      scanf("%d", &n);

      printf("Enter %d integers\n", n);

      for (c = 0; c < n; c++) {
        scanf("%d", &array[c]);
      }

      for (c = 0 ; c < ( n - 1 ); c++)
      {
        for (d = 0 ; d < n - c - 1; d++)
        {
          if (array[d] > array[d+1]) // For decreasing order use <
          {
            swap       = array[d];
            array[d]   = array[d+1];
            array[d+1] = swap;
          }
        }
      }

      printf("Sorted list in ascending order:\n");

      for (c = 0 ; c < n ; c++)
         printf("%d\n", array[c]);

    ctr.print();
}

/*#define Q std::vector <
#define Y int
Q Q Y> > Simple_Task(Y N, Y M, Q Q std::string> > m) {
  Q Q Y> > r(N, Q Y>(M));
  for (Y i=0; i<N*M; i++)
      r[i/M][i%M] = m[i/M][i%M] == "X";
  return r;
}*/

/*
    cout << ((1<<0)|(1<<3)|(1<<6)) << endl;
    cout << ((1<<1)|(1<<4)|(1<<7)) << endl;
    cout << ((1<<2)|(1<<5)|(1<<8)) << endl;
    cout << ((1<<0)|(1<<1)|(1<<2)) << endl;
    cout << ((1<<3)|(1<<4)|(1<<5)) << endl;
    cout << ((1<<6)|(1<<7)|(1<<8)) << endl;
    cout << ((1<<0)|(1<<4)|(1<<8)) << endl;
    cout << ((1<<2)|(1<<4)|(1<<6)) << endl;
*/
#define F for (auto
#define V std::vector<
bool Tic_Tac_Toe(V V std::string > > a) {
    int x=0,u=0;
    F i:a)
        F j:i)
            x += x + (j == "X");
    F v : V int>{73,146,292,7,56,448,273,84})
        u |= (x & v) % v == 0;
    return u;
}
#undef F
#undef V

int missingNumber(std::vector< int > A) {
    int i=2, s=1;
    for(int a : A) s += i++ - a;
    return s;
}

int nodes_outside_cycle(int N, std::vector< int > p) {
    std::vector<int> v(N);
    int i,x=1,t=0;
    while (N--) {
        for (i=N; v[i] == 0; i=p[i])
            v[i] = x++;
        t += (v[i] >= v[N]? v[i]: x) - v[N];
    }
    return t;
}

#define V std::vector<
int max_treasure(int N, V V int > > C, int P) {
    V int> v(P+1);
    for(auto c: C)
        for(int i=P-c[1]; i>=0; i--)
            v[i + c[1]] = max(v[i + c[1]], v[i] + c[0]);
    return v[P];
}
#undef V

double SqrRotBab(int n, double e) {
    double r = n;
    while (abs(n - r * r) > e) {
        r = (r + n/r)/2;
    }
    return int(r*100000)/100000.0;
}

std::vector< int > LineLandFuel(int i, std::vector< int > c) {
  int d,m=0,M=0;
  for (auto p: c) {
    d = abs(p - c[i]);
    if (!m || d && d < m) m = d;
    if (d > M) M = d;
  }
  return std::vector<int>{M, m};
}

bool isFibonacci(int N) {
    int i,f[50] = {0,1};
    for (i=1; f[i] < N; i++)
        f[i+1] = f[i] + f[i-1];
    int x, a = 0, b = 1;
    while (b != N)
        x = a + b, a = b, b = x;
    return b == N;
}

bool Check1(std::string N) {
   int x = 0;
   for (auto i = N.rbegin(); i != N.rend(); i++) {
       x = *i - '0' - (x < 0? x+11 :x);
   }
   return !x;
}

bool Check(std::string N) {
    int i=1, x=0;
    for (char c : N)
      x = c - 4 - x;
    return !(x%11);
 }

std::vector< int > multby2(std::vector< int > a) {
  for (int &i: a) i *= 2;
  return a;
}

// Spreadsheet : A1 to CRXO65535
// Row-Column: R1C1 to R65535C65535
std::string spreadsheet(std::string s) {
    int r=0,c;
    std::string o;
    if (sscanf(s.c_str(), "R%dC%d", &r, &c) == 2) {
        while (c)
            o = char(64 + c % 26) + o,
            c /= 26;
    } else {
        for (int c : s)
            if (c > 60) r = r*26 + c - 64;
            else o += c;
        o = "R" + o + "C";
    }
    return o + std::to_string(r);
}

// For circle1 = [2, 2, 3] and circle2 = [0, -1, 3] the output should be 8
int overlapPoint(std::vector< int > a, std::vector< int > b) {
    int i,j, q,w, t=0;
    for (i=a[0]-a[2]; i <= a[0]+a[2]; i++) {
        q = sqrt(a[2]*a[2] - pow(i-a[0], 2));
        w = sqrt(b[2]*b[2] - pow(i-b[0], 2));
        t += std::max(0, std::min(q+a[1], w+b[1]) - std::max(-q+a[1],-w+b[1]) + 1);
    }
    return t;
}

// For a = [555,123,334,233,123,555,444,567,334] the output should be [555,123,334].
std::vector< int > mode(std::vector< int > a) {
    int t,m = 0;
    std::vector<int> r;
    for (int& x: a) {
        t = 0;
        for (int& y: a)
            if (&x < &y && x == y) {
                if (++t > m) r.clear(), m = t;
                if (t == m) r.push_back(x);
            }
    }
    return r;
}

#define S std::string
S Sum(int N) {
  S r = --N? --N? "494" + S(N-1,57) + "55" + S(N, 48) : "4905" : "45";
  return r;
}
#undef S

int first_again(int N) {
    int t = 0, p = 2;
    do {
        p = p <= N? 2*p-1: 2*(2*N-p+1);
        t++;
    } while (p != 2);
    return t;
}

bool is_cyclic_sorted(std::vector< int > a) {
    int n = a.size(), q=0, p = n? a[n-1]: 0;
    for (int x : a)
        x < p? q++: x > p? q--: n++,
        p = x;
    return std::abs(q) >= n-2;
}

int Digital_sum(int a) {
    while (a > 9) a = a/10 + a%10;
    return a;
  }

int compareSumOfDigits(std::string N) {
    int s = 0;
    for (int c: N)
        s += c % 2? c-48: 48-c;
    return s;
}

std::string hexa(int h) {
    char s[9];
    sprintf(s, "%#X", h);
    return s;
}

string HackIt(string s) {
  string r;
  for (int c: s)
    r += c < 65? c: 2*(c&32) + 155 - c;
  return r;
}

int water_area(std::vector< int > a) {
    int p = a[0], t = 0, c = 0;
    for (auto i=a.begin(); i!=a.end(); i++) {
        if (*i < p)  c += p - *i;
        if (*i >= p)
            t += c,
            c = 0,
            p = std::min(*i, *std::max_element(i+1, a.end()));
    }
    return t;
}

std::vector< std::string > filterBible(std::vector< std::string > s, std::string b, std::string c) {
    std::vector< std::string > r;
    for (auto x: s) {
        if (x.substr(0,5) == b+c)
            r.push_back(x);
    }
    return r;
}

// [2, 1, 2, 2, 6, 5, 0, 2, 0, 5, 5, 7, 7, 4, 3, 3, 9]  -->
// [2, 1, 10, 5, 2, 24, 4, 15] -->
// [2, 1, 10, 5, 30, 15]  - array of length 6
int SumGroups(std::vector< int > a) {
    int i,p, b = 1;
    while (b)
        for (b=0,i=1,p=a[0]; i < a.size(); i++)
             p%2 == a[i]%2?
                a[i-1] += a[i],
                a.erase(a.begin()+i--),
                b = 1
             : p = a[i];
    return i;
}

// 4 -> 1,2,3,41,42,431,4321
int pressButton(int n) {
    return n*(n*n+5.)/6;
}

int Count_SubArrays(std::vector< int > A, int D) {
    int t=0, x=0,i,s, n = A.size();
    while (!(++x >> n)) {
        for (i=s=0; i<n; i++)
            if (x & (1 << i))
                s += A[i];
        t += s % D == 0;
    }
    return t;
}

int Chocolates(int B, int w) {
    int t;
    for (t=B; B >= w; B = B/w + B%w) t += B/w;
    return t;
  //return B + B/(w - 1 + 1./B);
}

int Count_SubArray(std::vector< int > a, int n, int d) {
    int m=0, x=0,i,s,q, N = a.size();
    while (!(++x >> N)) {
        for (i=s=q=0; i<N; i++)
            if (x & (1 << i))
                s += a[i], q++;
        if (q == n && s % d == 0 && (!m || s < m)) m = s;
    }
    return m;
}

#define V std::vector<double>
// n = 2, a = [1.5, 3.2], b = [1, 23.1], the answer will be 2310
double CashExchange(int n, V a, V b) {
    /*for (int i=0; i<n; i++) {
        v[0][i+1] = max(v[0][i],      max(v[1][i]*a[i],      v[2][i]*b[i]));
        v[1][i+1] = max(v[0][i]/a[i], max(v[1][i],           v[2][i]/a[i]*b[i]));
        v[2][i+1] = max(v[0][i]/b[i], max(v[1][i]/b[i]*a[i], v[2][i]));
    }*/
    //double x = v[k][i] / (1) * (k && k-j? k>1? a[i]: b[i]: 1);
    V p(3), q(3);
    p[0] = 100;
    for (int i=0; i<n; i++,p=q)
        for (int j=0; j<9; j++) {
            double y = p[j%3], g=a[i], h=b[i],
                   x[9] = {y, y*g, y*h, y/g, y, y*h/g, y/h, y*g/h, y};
            if (q[j/3] < x[j]) q[j/3] = x[j];
        }
    return int(p[0]*100)/100.;
}
#undef V

std::vector<int> DX_is_so_handsome(int N) {
    int i,x;
    std::vector<int> r;
    for (i=4; i<N; i++) {
        for(x=i; x%10 == 4 || x%10 == 7; x/=10);
        if (!x) r.push_back(i);
    }
    return r;
}

int bitwiseandzero(int n, std::vector< int > a) {
    int t = 0;
    for (int &x: a)
      for (int &y: a)
         if (&x != &y && !(x & y)) t++;
    return t;
}

int Sum_prime_numbers(int N) {
  int i,j, s = 0, v[100001] = {0};
  for (i=2; i<=N; i++)
    if (!v[i]) {
      for (j=2; i*j <= N; j++)
        v[i*j] = 1;
      s += i;
    }
  return s % 256;
}

int charactersMatrix(std::string m, int r, int c) {
    int i,j, t=0;

    for (i=0; i<r; i++, m+='\n')
        for (j=0; j<c; j++) m += m[i*c+j] | 32;
    for (j=0; j<c; j++, m+='\n')
        for (i=0; i<r; i++) m += m[i*c+j] | 32;
    for (j=1-r; j<c; j++, m+='\n')
        for (i=0; i<r; i++)
            if (i+j >= 0 && i+j < c)
                m += m[i*c+i+j] | 32;

    cout << m;
    for (i=r*c-1; (i = m.find("phuong", i+1)) > 0; t++);
    return t;
}

int Luckynumber(int N) {
    int i=0,t=0;
    for ( ; N; N /= 10) t += N % 10 / 3 << i++;
    return t;
}

std::string blackjack(std::vector< int > p) {
    int i=0,q[2] = {0,0}, z=0;
    for (int x: p) q[i] += x, i=z?i:1-i, z |= !x;
    return q[0] > 21 || q[0] < q[1]? "second": "first";
}

bool correct_parentheses(std::string s) {
    int q[99],i=0;
    for (int c: s) {
        if (c - 1 & 3) q[i++] = c + 1 + (c&1);
        else if (!i || c != q[--i]) return 0;
    }
    return !i;
}

int Majority_Element(int n, std::vector<int> a) {
    int s[500] = {0}, m = -1;
    for (int x: a) m = ++s[x] > n/2? x: m;
    return m;
}

int sumofareas(int l, int w, int a) {
    return l*w + l*a + w*a << 1;
}

int snail_trip(int H, int d, int n) {
    return H < d? !!H: (H - n - 1) / (d - n) + 1;
}

// For a = 3006 and b = 4090 the output should be 1923016
int sumofoddnumbers(int a, int b) {
    a = (a-1)|1;
    b = (b-2)|1;
    return (long long)(a + b + 2) * (b - a) / 4 % 10000007;
}

int someFunction(int x) {
    int i;
    for (i=2; x/i/i; )
      x % i++? 0:x=1;
    return x-1;
}

int trivia_game(std::vector<int> s, int a, int b) {
    int i=0,j,m=0, p[1000] = {0};
    for (; i <= s[0]; i++) {
        for (j=0; j<8; j++)
            p[i] += std::max(s[1] + a*(j&1?-1:1), s[2] + b*(j&2?-1:1)) < s[0] + i*(j&4?-1:1);
        m = p[m] < p[i]? i: m;
    }
    return m;
}

int digitalroot(std::string n) {
    int t=0;
    for (int c: n) t += c - 3;
    return t % 9? t%9: 9;
}

int strings(string s) {
    int i,j,n,m=0,r,t,b;
    for (n=b=1; s[n-1]; n++)
        for (i=b=0; s[i]; i++) {
            for (t=0,j=i; (j=s.find(s.substr(i,n),j+1)) + 1; t++) b=1;
            if (t >= m) m = t, r = n;
        }
    return r;
}

std::vector<int> intersection(std::vector<std::vector<int>> a) {
    std::map<int, std::array<int, 10>> m;
    std::vector<int> r;
    int b, j, i = 0;
    for (auto v: a) {
        for (int x: v) m[x][i]++;
        i++;
    }
    for (auto x: m) {
        for (j=b=1; j<i; j++) b &= x.second[j] == x.second[j-1];
        if (b) r.push_back(x.first);
    }
    return r;
}

int sumofprimenumbers(int n, std::vector<int> a) {
    int t = 0;
    for (int x: a) {
      for (n=2; n < x && x % n; n++);
      if (n == x) while(x) t += x % 10, x /= 10;
    }
    return t;
  }

int palindrome_prime(int N) {
  int x,y=0;
  for (x=N; x; x /= 10) y = y*10 + x % 10;
  for (x=2; x < N && N % x; x++);
  return x == N & y == N;
}

int wellOrderedNumber(int n) {
    int p;
    while (p = n % 10, n /= 10, p > n % 10);
    return !n;
}

std::string Palindromic_Number(char N) {
  std::string r;
  r = N += 48;
  while(N - 49) r = --N + r + N;
  return r;
}

std::string stringRotation(std::vector<std::string> A) {
  std::string a=A[0], b=A[1];
  return ((a+a).find(b)+1) && ((b+b).find(a)+1) ? "YES": "NO";
}

template <class V>
V shuffleState(V d, int n) {
    V a(d);
    int i, s = d.size();
    for ( ; i=0,n--; d = a)
        for (int &x: a)
            x = d[i/2 + s/2*(i++%2)];
    return d;
}

int OnesandZeros(int n, int s=1) {
    return n? OnesandZeros(n-1) + (s? OnesandZeros(n-1, 0): 0): 1;
}

int RankOfElement(std::vector<int> A, int i) {
  int t=0,j=0;
  for (int a: A) t += a < A[i] + (j++ < i);
  return t;
}

std::vector<std::vector<int>> pyramidMatrix(int N) {
    std::vector<std::vector<int>> r;
    for(int j,i=0; i<N; i++) {
        r.push_back(std::vector<int>(N));
        for (j=0; j<N; j++) {
            r[i][j] = std::min(std::min(i+1,j+1),std::min(N-i, N-j));
        }
    }
    return r;
}

int FaultyOdometer(int n) {
    int x=0,d,q;
    for (q=1;  n; n /= 10, x += (d-d/5)*q, q *= 9) d = n%10;
    return x;
}

std::vector<int> HappyNumbers(int R) {
    std::vector<int> r;
    int i,x,a, h[2001];
    for (i=1; i<=R; i++) {
        x = i;
        for (x=i; h[x] != i; x = a)
            for (h[x]=i,a=0; x; x /= 10) a += (x%10)*(x%10);
        if (x == 1) r.push_back(i);
    }
    return r;
}

int main(int argc, char **argv)
{
    for (int x: HappyNumbers(2000)) cout << x << ", ";
    //cout << FaultyOdometer(2003);
    //for (auto r: pyramidMatrix(3)) { for (int x: r) cout << x << ","; cout << endl; }
    //cout << RankOfElement(vector<int>{1,2,3,4,5,6}, 3);
    //cout << OnesandZeros(7);
    //for (int x: shuffleState(vector<int>{3,4,7,3,4,6,4,2,2,3,4,5,7,8,9,5,4,3,3,5,6,7,8,8,6,5,4,3,5,6,7,8,5,6,3,5,7,8}, 17)) cout << x << ", ";
    //cout << stringRotation(vector<string>{"codefights", "fightscode"});
    //cout << Palindromic_Number(5);
    //cout << wellOrderedNumber(356);
    //cout << sumofprimenumbers(5, vector<int> { 1, 2, 3, 4, 5, } );
    // for (auto x: intersection(vector<vector<int>>{{2, 1, 5, 3, 5}, {3, 5, 5}})) cout << x << ", ";
    //cout << strings("abcd");
    //cout << digitalroot("12345");
    //cout << trivia_game(vector<int>{150,100,110}, 99, 80);
    //cout << sumofoddnumbers(3006, 4090);
    //cout << snail_trip(1, 12, 10);
    //cout << Majority_Element(5, vector<int>{ 4,3,4,4,2 });
    //cout << correct_parentheses("((([])))()[]");
    //cout << blackjack(vector<int>{7,9,5,3,0,4,2,2});
    //cout << Luckynumber(7477);
    //cout << charactersMatrix("pHuOnG", 1, 6);
    //cout << Sum_prime_numbers(50);
    //cout << bitwiseandzero(5, vector<int>{3,6,0});
    //for (int x: DX_is_so_handsome(1000)) cout << x << ", ";
    //cout << CashExchange(2, {1.5, 3.2}, {1, 23.1});
    //cout << Count_SubArray(vector<int>{1,2,3,4,5,6}, 3, 5);
    //cout << Chocolates(1000, 20);
    //cout << Count_SubArrays(vector<int>{2,3,5,5}, 5);
    //cout << pressButton(4);
    //cout << SumGroups(vector<int>{2, 1, 2, 2, 6, 5, 0, 2, 0, 5, 5, 7, 7, 4, 3, 3, 9});
    //for (auto s: filterBible(std::vector<std::string>{"01001001", "01001002", "01002001", "01002002", "01002003", "02001001", "02001002", "02001003"}, "02", "001")) cout << s << endl;
    //cout << water_area(std::vector< int > {2,0,3,0,4,0,1});
    //cout << HackIt(string("Xlwv 98Urtsgh hello"));
    //cout << hexa(27);
    //cout << compareSumOfDigits("74");
    //cout << Digital_sum(49484);
    //cout << is_cyclic_sorted(vector<int> { 1 } );
    //cout << first_again(100000000);

    char x[3][3][2] = {
            {"X","O","O"},
            {"X","O","X"},
            {"O","X","O"} };
    vector<vector<string> > v;
    v.push_back(vector<string>(x[0],x[1]));
    v.push_back(vector<string>(x[1],x[2]));
    v.push_back(vector<string>(x[2],x[3]));
    //cout << Tic_Tac_Toe(v);
    //cout << missingNumber(vector<int>{ 1,2,3,4,5 });
    //cout << nodes_outside_cycle(3, vector<int>{ 1,2,0,4,5,3,7,8,4 });
    //int mt[] = { [[4,1], [9,3], [2,1], [3,1]] };
    vector<vector<int> > mt({{4,2},{9,4},{2,2},{3,2}});
    //cout << max_treasure(mt.size(), mt, 1);
    //cout << std::setprecision(15);
    //cout << SqrRotBab(140,0.89); // 11.83262
    //auto LLF = LineLandFuel(2, vector<int>{ -5,-2,2,7 });
    //cout << LLF[0] << ", " << LLF[1];
    //cout << Check1("68162732");
    //cout << multby2(vector<int> { 1,2,3,4 })[1];
    //cout << spreadsheet("R5C4");
    //cout << spreadsheet("CRXO65535");
    //cout << overlapPoint(vector<int>{2,2,3}, vector<int>{0,-1,3});
    //for (int x: mode(vector<int> {555,123,334,233,123,555,444,567,334})) cout << x << ", ";
    //cout << Sum(2);

    return 0;
}
