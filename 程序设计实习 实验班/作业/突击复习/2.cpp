#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

const int N = 3010;

using db = double;
using uint = unsigned int;
using ll = unsigned long long;

int n, a[N][N], b[N][N], c[N][N], d[N];
ll e[N], f[N], g[N];
uint seed;
int randint(const unsigned int &seed) {
    static unsigned int x = seed;
    x = (x * 23333LL + 23327) % 1000000007;
    return x % 20 - 10;
}

mt19937 rnd(19491001);

#define GG return puts("NO"), void();

void solve() {
    scanf("%d %u", &n, &seed);
    for(int i = 1; i <= n; i ++)
    for(int j = 1; j <= n;j++) {
        a[i][j] = randint(seed);
        b[i][j] = randint(seed);
    }
    rep(i, 1, n) rep(j, 1, n) scanf("%d", &c[i][j]);
    rep(_, 1, 8) {
        rep(i, 1, n) d[i] = rnd() % 2, e[i] = f[i] = g[i] = 0;
        rep(i, 1, n) rep(j, 1, n) e[i] += b[i][j] * d[j];
        rep(i, 1, n) rep(j, 1, n) f[i] += a[i][j] * e[j];
        rep(i, 1, n) rep(j, 1, n) g[i] += c[i][j] * d[j];
        rep(i, 1, n) if(f[i] != g[i]) GG;
    } puts("YES");
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    // freopen("1.out", "w", stdout);
#endif
    int T = 1;
    while(T--) solve();
    return 0;
}