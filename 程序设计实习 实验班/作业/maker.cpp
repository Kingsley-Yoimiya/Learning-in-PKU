#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

const int M = 25;
const int N = 1e5 + 10;

typedef unsigned long long u64;

int n, q;
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

int main() {
    freopen("1.in", "w", stdout);
    n = q = 100000;
    cout << n << " " << q << endl;
    rep(i, 1, n) cout << rnd() << endl;
    rep(i, 1, q) cout << rnd() << endl;
    return 0;
}