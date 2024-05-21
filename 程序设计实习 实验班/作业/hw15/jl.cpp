#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

using db = double;
using vec = vector < db >;

const int N = 1010;


mt19937 rnd(time(NULL));
normal_distribution<> gen(0, 1);

int d = 28 * 28, n = 1000;
int m;
vec a[N], b[N];
db G[N][N];

db err[N][N], tdis[N][N];
vector < db > res1, res2; // res1 : max, res2 : average

db dist(vec x, vec y, int d = ::d) {
    db res = 0; rep(i, 0, d - 1) res += (x[i] - y[i]) * (x[i] - y[i]);
    return sqrt(res);
}

pair < db, db > genert() {
    rep(i, 0, d - 1) rep(j, 0, m - 1) G[i][j] = gen(rnd);
    rep(i, 1, n) b[i].clear(), b[i].resize(m);
    rep(i, 1, n) rep(j, 0, d - 1) rep(k, 0, m - 1) b[i][k] += a[i][j] * G[j][k];
    rep(i, 1, n) rep(j, 0, m - 1) b[i][j] /= sqrt(m);
    db ret1 = 0, ret2 = 0;
    rep(i, 1, n) rep(j, i + 1, n) {
        db ndis = dist(b[i], b[j], m);
        ret1 = max(ret1, abs(ndis - tdis[i][j]) / tdis[i][j]),
        ret2 += abs(ndis - tdis[i][j]) / tdis[i][j];
    }
    ret2 /= n * (n - 1) / 2;
    return (pair<db, db>){ret1, ret2};
}

const int T = 5;

void gener() {
    db r1 = 0, r2 = 0;
    rep(t, 1, T) {
        db ret1, ret2; 
        tie(ret1, ret2) = genert();
        r1 += ret1, r2 += ret2;
    }
    r1 /= T, r2 /= T;
    cerr << r1 << " " << r2 << endl;
    res1.emplace_back(r1), res2.emplace_back(r2);
}

int main() {
    freopen("data.txt", "r", stdin);
    freopen("result.txt", "w", stdout);
    rep(i, 1, n) {
        rep(j, 1, d) {
            int x; scanf("%d", &x);
            a[i].emplace_back(x);
        }
    }
    rep(i, 1, n) rep(j, 1, n) tdis[i][j] = dist(a[i], a[j]);
    for(int i = 5; i <= 300; i += 5) {
        m = i; gener();
        cerr << "GENERI: " << i << endl;
    }
    for(auto v : res1) printf("%.10lf,", v); puts("");
    for(auto v : res2) printf("%.10lf,", v); puts("");
    return 0;
}