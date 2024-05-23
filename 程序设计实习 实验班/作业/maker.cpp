#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

const int M = 25;
const int N = 1e5 + 10;

typedef unsigned long long u64;

int n, q, m;
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

const int V = 1e6;
int val[N];

int main() {
    freopen("1.in", "w", stdout);
    n = 6e4, m = 300; //q = 50000;
    cout << n << " " << m << endl;
    rep(i, 1, m) val[i] = int(rnd() % 100) - 50;
    double res = 0;
    rep(i, 1, n) {
        int t = 20; printf("%d ", t);
        int ret = 0;
        rep(j, 1, t) {
            int pos = rnd() % m + 1, v = int(rnd() % V) - V / 2;
            ret += val[pos] * v;
            printf("%d %d ", pos, v);
        } 
        int val = rnd() % 2; val -= 1; 
        ret += val; res += val * val;
        printf("%d\n", ret + rnd() % 2);
    }
    cerr << res << endl;
    return 0;
}