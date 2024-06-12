#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

const int N = 1e6 + 10;
const int K = 40;

using db = double;
using ull = unsigned long long;
using ll = unsigned long long;

mt19937_64 gener(time(NULL));

int n, m, q, b[N], tot;
int a[1010][1010];
ull ret[K][1010], seed, trans[N], ta[1010];

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    // freopen("1.out", "w", stdout);
#endif
    scanf("%d %d %d", &n, &m, &q);
    rep(i, 1, n) rep(j, 1, m) scanf("%d", &a[i][j]), b[++tot] = a[i][j];
    sort(b + 1, b + tot + 1); tot = unique(b + 1, b + tot + 1) - b - 1;
    rep(i, 1, n) rep(j, 1, m) a[i][j] = lower_bound(b + 1, b + tot + 1, a[i][j]) - b;
    rep(i, 0, K - 1) {
        seed = gener();
        mt19937_64 rnd(seed);
        rep(j, 1, tot) trans[j] = rnd();
        rep(j, 1, n) {
            rep(k, 1, m) ta[k] = trans[a[j][k]];
            ret[i][j] = *min_element(ta + 1, ta + m + 1);
        }
    }
    while(q--) {
        int x, y; scanf("%d %d", &x, &y);
        int cnt = 0;
        rep(i, 0, K - 1) cnt += ret[i][x] == ret[i][y];
        printf("%.10lf\n", 1. * cnt / K);
    }
    return 0;
}