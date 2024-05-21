#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

using db = double;
//using node = tuple < int, int, int >;

const db eps = 0.5;
const int N = 1e5 + 10;

struct node {
    db x, y, z;
    node(db _x = 0, db _y = 0, db _z = 0) : x(_x), y(_y), z(_z) {}
};

int n, q, rt, ndn;

db ans = 1e18;
struct tnode {
    db diam;
    int ch[8];
    node a;
} val[N << 5];

db dis(node a, node b) { return (sqrt(1. * (a.x - b.x) * (a.x - b.x) + 1. * (a.y - b.y) * (a.y - b.y) + 1. * (a.z - b.z) * (a.z - b.z))); }

int build(vector < node > pot, int dep = 0) {
    int x = ++ndn; 
    db xl = 2e9, xr = 0, yl = 2e9, yr = 0, zl = 2e9, zr = 0;
    for(auto v : pot)
        chkmin(xl, v.x), chkmax(xr, v.x),
        chkmin(yl, v.y), chkmax(yr, v.y),
        chkmin(zl, v.z), chkmax(zr, v.z);
    val[x].diam = dis(node(xl, yl, zl), node(xr, yr, zr));
    val[x].a = pot.back();// pot.pop_back();
    if(val[x].diam < 1e-7 && pot.size() > 1) chkmin(ans, val[x].diam);
    if(val[x].diam < 1e-7) return x;
    vector < node > cpot[8];
    db xmid = (xl + xr) / 2, ymid = (yl + yr) / 2, zmid = (zl + zr) / 2;
    for(auto x : pot) 
        cpot[((x.x > xmid) << 1 | (x.y > ymid)) << 1 | (x.z > zmid)].emplace_back(x);
    rep(i, 0, 7) if(cpot[i].size()) 
        val[x].ch[i] = build(cpot[i], dep + 1);
    return x;
}

vector < pair < int, int > > res;

void WSPD(int x, int y) {
    if(x == y && val[x].diam < 1e-7) return;
    if(val[x].diam < val[y].diam) swap(x, y);
    if(dis(val[x].a, val[y].a) * eps > val[x].diam) return res.emplace_back(x, y), void();
    rep(i, 0, 7) if(val[x].ch[i]) 
        WSPD(val[x].ch[i], y);
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
#endif 
    scanf("%d", &n);
    vector < node > pot; 
    rep(i, 1, n) {
        node t; scanf("%lf %lf %lf", &t.x, &t.y, &t.z); 
        pot.emplace_back(t);
    }
    rt = build(pot);
    cerr << "!" << ndn << endl;
    rep(x, 1, ndn) {
        rep(i, 0, 7) if(val[x].ch[i]) rep(j, i + 1, 7) if(val[x].ch[j]) 
            WSPD(val[x].ch[i], val[x].ch[j]);
    }
    for(auto v : res) chkmin(ans, dis(val[v.first].a, val[v.second].a));
    printf("%.2lf\n", ans);
    return 0;
}