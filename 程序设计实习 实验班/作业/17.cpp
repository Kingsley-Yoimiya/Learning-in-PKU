#include <bits/stdc++.h>


#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

using db = long double;
using vec = vector < db >;

const int N = 1e5 + 10;
const int M = 1010;
const db eps = 1e-6;

mt19937 rnd(time(NULL));
normal_distribution<> gen(0, 1);

int n, m, d;
vector < pair < int, db > > A[N], AT[N];
db x[2][M], tmp[N];

void reduce(int o) {
    db res = 0;
    rep(i, 1, d) {
        res += x[o][i] * x[o][i];
    }
    res = sqrtl(res);
    rep(i, 1, d) x[o][i] /= res;
}

void mulA(db x[M], db y[N]) {
    rep(i, 1, n) y[i] = 0;
    rep(i, 1, n) {
        for(auto [pos, w] : A[i]) {
            y[i] += x[pos] * w;
        }
    }
}
void mulAT(db x[N], db y[M]) {
    rep(i, 1, d) y[i] = 0;
    rep(i, 1, d) {
        for(auto [pos, w] : AT[i]) {
            y[i] += x[pos] * w;
        }
    }
}

bool check() {
    db res = 0;
    rep(i, 1, d) {
        res += (x[0][i] - x[1][i]) * (x[0][i] - x[1][i]);
    }
    res = sqrtl(res);
    // cerr << "!" << res << endl;
    return res < eps;
}

int main() {
    #ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
    #endif
    scanf("%d %d %d", &n, &d, &m);
    rep(_, 1, m) {
        int i, j; db w;
        scanf("%d %d %Lf", &i, &j, &w);
        A[i].emplace_back(j, w);
        AT[j].emplace_back(i, w);
    }
    int o = 0;
    rep(i, 1, d) x[o][i] = gen(rnd);
    reduce(o); 
    // rep(i, 1, d) cerr << x[o][i] << " ";
    // cerr << endl;
    while(true) {
        mulA(x[o], tmp);
        mulAT(tmp, x[o ^ 1]);
        reduce(o ^ 1);
        o ^= 1;
        if(check()) break;
    }
    rep(i, 1, d) printf("%Lf\n", x[o][i]);
    return 0;
}