#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

const int N = 1e5 + 10;
const int K = 40;

using db = long double;
using ull = unsigned long long;
using ll = unsigned long long;

const db eps = 1e-10;

mt19937_64 rnd(time(NULL));

int n, m, d;
vector < pair < int, db > > pot[N], rpot[N];
db ret[2][N], tmp[N];
normal_distribution <> gen(0, 1);

void reduce(db *ret) { 
    db tot = 0;
    rep(i, 1, n) tot += ret[i] * ret[i];
    tot = sqrtl(tot);
    rep(i, 1, n) ret[i] /= tot;
}
void mul(db *a, db *b) {
    rep(i, 1, n) tmp[i] = b[i] = 0;
    rep(i, 1, n) for(auto [x, y] : pot[i]) tmp[i] += a[x] * y;
    rep(i, 1, d) for(auto [x, y] : rpot[i]) b[i] += y * tmp[x];
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    // freopen("1.out", "w", stdout);
#endif
    scanf("%d %d %d", &n, &d, &m);
    while(m--) {
        int x, y; db w;
        scanf("%d %d %Lf", &x, &y, &w);
        pot[x].emplace_back(y, w); rpot[y].emplace_back(x, w);
    }
    db *a, *b; a = ret[0], b = ret[1];
    rep(i, 1, d) a[i] = gen(rnd);
    reduce(a); 
    while(true) {
        mul(a, b); reduce(b);
        db tot = 0;
        rep(i, 1, d) tot += (a[i] - b[i]) * (a[i] - b[i]);
        if(tot < eps) break;
        swap(a, b);
    } rep(i, 1, d) printf("%.10Lf\n", a[i]);
    return 0;
}