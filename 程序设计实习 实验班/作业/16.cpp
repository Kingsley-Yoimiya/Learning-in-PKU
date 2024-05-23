#include <bits/stdc++.h>


#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

using db = double;
using vec = vector < db >;

const int N = 6e4 + 10;
const int M = 2010;
const db eps = 1e-8;

mt19937 rnd(time(NULL));
normal_distribution<> gen(0, 1);

int n, d, m = M - 10;
vector < pair < int, int > > pos[N];
db x[M][M], y[M][2], xt[M][M], xtx[M][M], invxtx[M][M], ty[M][2], py[N];

void mul(db a[M][M], db b[M][M], db c[M][M], int n, int m, int d) {
    rep(i, 1, n) rep(j, 1, d) c[i][j] = 0;
    // rep(i, 1, n) rep(j, 1, d) rep(k, 1, m) c[i][j] += a[i][k] * b[k][j];
    rep(i, 1, n) rep(k, 1, m) rep(j, 1, d) c[i][j] += a[i][k] * b[k][j];
}

void mul(db a[M][M], db b[M][2], db c[M][2], int n, int m, int d) {
    rep(i, 1, n) rep(j, 1, d) c[i][j] = 0;
    rep(i, 1, n) rep(j, 1, d) rep(k, 1, m) c[i][j] += a[i][k] * b[k][j];
}
void mul(db a[M][M * 2], db b[M][2], db c[M][2], int n, int m, int d) {
    rep(i, 1, n) rep(j, 1, d) c[i][j] = 0;
    rep(i, 1, n) rep(j, 1, d) rep(k, 1, m) c[i][j] += a[i][k] * b[k][j];
}

void getInv() {
    rep(i, 1, d) rep(j, 1, d) invxtx[i][j] = xtx[i][j];
    rep(i, 1, d) invxtx[i][d + i] = 1;
    rep(i, 1, d) {
        //cerr << "!" << i << endl;
        int pos = i; rep(j, i, d) if(fabs(invxtx[j][i]) > fabs(invxtx[pos][i])) pos = j;
        if(pos != i) rep(j, 1, d * 2) swap(invxtx[i][j], invxtx[pos][j]);
        db rate = invxtx[i][i];
        rep(k, i, d * 2) invxtx[i][k] /= rate;
        rep(j, 1, d) if(j != i) {
            db rate = invxtx[j][i] / invxtx[i][i];
            rep(k, i, d * 2) invxtx[j][k] -= rate * invxtx[i][k];
        }
    }
    rep(i, 1, d) rep(j, 1, d) invxtx[i][j] = invxtx[i][j + d];
}

int main() {
    #ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
    #endif
    scanf("%d %d", &n, &d);
    rep(i, 1, n) {
        int l; scanf("%d", &l);
        while(l--) {
            int p, v; scanf("%d %d", &p, &v);
            pos[i].emplace_back(p, v);
        }
        scanf("%lf", &py[i]);
    }
    rep(j, 1, n) { // A 每一列仅一个非零（1, -1），生成矩阵 X'(m * d) = A(m * n)X(n * d)
        int p = rnd() % m + 1, v = rnd() % 2 ? 1 : -1; // p, j, k
        for(auto [po, va] : pos[j])
            x[p][po] += va * v;
        y[p][1] += py[j] * v; // p, j, 1
    }
    rep(i, 1, m) rep(j, 1, d) xt[j][i] = x[i][j];
    mul(xt, x, xtx, d, m, d);
    mul(xt, y, ty, d, m, 1);

    //rep(i, 1, d) rep(j, 1, d) xtx[i][j] += gen(rnd); // 扰动

    getInv();
    mul(invxtx, ty, y, d, d, 1);
    
    // mul(invxtx, xtx, x, d, d, d);
    // rep(i, 1, d) { rep(j, 1, d) cerr << x[i][j] << " "; cerr << endl; }
    // rep(i, 1, d) rep(j, 1, d) assert((x[i][j] - (i == j)) < eps);

    rep(i, 1, d) printf("%.10lf%c", y[i][1], " \n"[i == d]);
    db ct = 0;
    rep(i, 1, n) {
        db res = 0;
        for(auto [po, va] : pos[i]) res += y[po][1] * va;
        ct += (res - py[i]) * (res - py[i]);
    }
    cerr << ct << endl;
    return 0;
}