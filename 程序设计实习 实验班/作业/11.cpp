#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

using db = double;
//using node = tuple < int, int, int >;

const db eps = 1.15;
const int N = 1e5 + 10;

struct node {
    db first, second; 
    int id;
};

int n, q, rt, ndn;
struct tnode {
    db xl, xr, yl, yr;
    int ch[2][2];
    node a;
} val[N << 5];
vector < int > npot[50], chk_pot[50];

mt19937 rnd(time(NULL));

db dis(node a, node b ) { return (sqrt(1. * (a.first - b.first) * (a.first - b.first) + 1. * (a.second - b.second) * (a.second - b.second))); }
db dis(pair <db, db> a, pair < db, db> b ) { return (sqrt(1. * (a.first - b.first) * (a.first - b.first) + 1. * (a.second - b.second) * (a.second - b.second))); }

int build(vector < node > pot, int dep = 0) {
    int x = ++ndn; 
    db xl = 1e9, xr = 0, yl = 1e9, yr = 0;
    for(auto v : pot)
        chkmin(xl, v.first), chkmax(xr, v.first),
        chkmin(yl, v.second), chkmax(yr, v.second);
    val[x].xl = xl, val[x].xr = xr, val[x].yl = yl, val[x].yr = yr;
    val[x].a = pot.back(); pot.pop_back(); npot[dep].emplace_back(x); 
    if(!pot.size()) return x;
    vector < node > cpot[2][2];
    db xmid = (xl + xr) / 2, ymid = (yl + yr) / 2;
    for(auto x : pot) 
        cpot[x.first > xmid][x.second > ymid].emplace_back(x);
    rep(i, 0, 1) rep(j, 0, 1) if(cpot[i][j].size()) 
        val[x].ch[i][j] = build(cpot[i][j], dep + 1);
    return x;
}

db ans; int ansid;

void expand(int dep, int x, node t) {
    //cerr << dis(val[x].a, t) << " " << t.first << " " << t.second << " " << dep << " " << val[x].a.first << " " << val[x].a.second << endl;
    if(dis(val[x].a, t) < ans) ans = dis(val[x].a, t), ansid = val[x].a.id;
    pair < db, db > x1 = { val[x].xl, val[x].yl }, x2 = { val[x].xr, val[x].yr };
    if((dis(val[x].a, t) - dis(x1, x2)) * eps > ans) return;
    rep(i, 0, 1) rep(j, 0, 1) if(val[x].ch[i][j]) chk_pot[dep + 1].emplace_back(val[x].ch[i][j]);
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
#endif 
    scanf("%d %d", &n, &q);
    vector < node > pot; 
    rep(i, 1, n) {
        node t; scanf("%lf %lf", &t.first, &t.second); t.id = i;
        pot.emplace_back(t);
    }
    rt = build(pot);
    //cerr << "!" << ndn << endl;
    while(q--) {
        node t; scanf("%lf %lf", &t.first, &t.second); t.id = -1;
        //cerr << t.first << " " << t.second << endl;
        ans = 1e18, ansid = -1;
        chk_pot[0].clear(); chk_pot[0].emplace_back(rt);
        rep(i, 0, 35) {
            if(!chk_pot[i].size()) break;
            chk_pot[i + 1].clear();
            for(auto nd : chk_pot[i]) expand(i, nd, t);
        } //cerr << ans << " " << ansid << endl; 
        printf("%d\n", ansid);
    }
    return 0;
}