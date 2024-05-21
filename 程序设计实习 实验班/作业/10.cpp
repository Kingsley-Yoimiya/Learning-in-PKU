#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

using db = double;
using node = pair < db, db >;

const db eps = 1e-1;
const int N = 1e5 + 10;

int n;
db T;
db ans = 0;
node a[N];
pair < int, int > b[N];

mt19937 rnd(time(NULL));

template < typename T > db dis(pair < T, T > a, pair < T, T > b) {
    return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
#endif 
    scanf("%d", &n);
    rep(i, 1, n) scanf("%lf %lf", &a[i].first, &a[i].second);
    int p = rnd() % n + 1;
    rep(i, 1, n) chkmax(ans, dis(a[i], a[p]));
    T = ans / sqrt(2) * eps;
    rep(i, 1, n) b[i] = make_pair((int)ceil(a[i].first / T), (int)ceil(a[i].second / T));
    sort(b + 1, b + n + 1);
    int tot = unique(b + 1, b + n + 1) - b - 1;
    rep(i, 1, tot) rep(j, i + 1, tot) chkmax(ans, dis(b[i], b[j]) * T);
    printf("%.8lf\n", ans);
    return 0;
}