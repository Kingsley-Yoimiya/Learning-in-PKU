#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

const int M = 25;
const int N = 1e5 + 10;

typedef unsigned long long u64;

int n, q, m, d;
u64 a[N];
pair < u64, int > b[M][N];
int tr[M][64];
u64 x;

u64 trans(u64 x, int i) {
    u64 res = 0;
    rep(j, 0, 63) res |= (x >> tr[i][j] & 1ull) << j;
    return res;
}

mt19937_64 rnd(time(NULL));
normal_distribution<> gen(0, 1);

const int V = 1e6;
int val[N];

int main() {
    freopen("1.in", "w", stdout);
    n = 1e5, d = 1000, m = 1e5; //q = 50000;
    cout << n << " " << d << " " << m << endl;
    rep(_, 1, m) {
        int i = rnd() % n + 1, j = rnd() % d + 1; double w = gen(rnd);
        cout << i << " " << j << " " << w << endl;
    }
    return 0;
}