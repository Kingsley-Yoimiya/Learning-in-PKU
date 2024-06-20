#include <bits/stdc++.h>


#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

using db = double;
using vec = vector < db >;
using ll = long long;
using node = pair < ll, ll >;
using tll = __int128;

const int N = 5e5 + 10;
const int mod = 1e9 + 7;
const db eps = 2e-1;
const int K = int(1 / eps) * int(1 / eps);
const int TK = 2;
const int V = 1e9;
const int BID = 30;

mt19937 rnd(19491001);
normal_distribution<> gen(0, 1);

db dis(pair < db, db > x, pair < db, db > y) {
    return sqrt((x.first - y.first) * (x.first - y.first) + (x.second - y.second) * (x.second - y.second));
}

db getD(int id) {
    return eps * pow(2., id);
}

tll getId(ll x, ll y, db d) {
    x = x / d, y = y / d;
    tll sq = V / d + 1;
    return x * sq + y;
}

node recXY(tll id, db d) {
    ll sq = V / d + 1;
    ll x = id / sq, y = id % sq;
    return node(x * d, y * d);
}

ll qp(int x, tll t) {
    ll ans = 1; t %= (mod - 1);
    for(ll v = x % mod; t; t >>= 1, v = v * v % mod) if(t & 1) ans = (ans * v) % mod;
    return ans;
}
struct sp1 {
    int c1; tll c2; int c3, r;
    void init() { c1 = c2 = c3 = 0, r = rnd() % mod; }
    void insert(tll v, int op) { c1 += op, c2 += op * v; (c3 += (qp(r, v) * op + mod) % mod) %= mod; }
    bool check() { 
        if(c1 == 0) return true;
        if(c2 % c1)  return false;
        tll p = c2 / c1;
        if(c1 * qp(r, p) % mod != c3) return false;
        return true;
    }
    bool hav() { return c1 > 0; }
    tll rec() { return c2 / c1; }
};

struct spt {
    int seed;
    sp1 s[K * 2];
    void init(int x) { seed = x; rep(i, 0, K * 2 - 1) s[i].init(); }
    int getH(tll v) { return (hash<ll>{}(v) ^ (std::hash<int>{}(seed))) % (K * 2); }
    void insert(tll v, int op, bool debug = false) {
        // cerr << (ll) v << " " << getH(v) << " " << seed << endl;
        //cerr << (v >= 0) << " " << K * 2 << " " << getH(v) << " " << seed << endl;
        if(debug) cerr << (ll)v << " " << getH(v) << " " << hash<int>{}((int)(v % mod)) << endl;
        s[getH(v)].insert(v, op);
        // cerr << "DN " << endl;
    }
    bool check() { rep(i, 0, 2 * K - 1) if(!s[i].check()) return false; return true; }
    vector < tll > recover() {
        vector < tll > ret;  
        rep(i, 0, 2 * K - 1) if(s[i].hav()) ret.emplace_back(s[i].rec());
        return ret;
    }
};

struct spr {
    int id; db d;
    spt s[TK];
    void init(int _id) {
        id = _id, d = getD(id);
        rep(i, 0, TK - 1) s[i].init(rnd() % mod); 
    }
    void insert(int x, int y, int op) { 
        tll tid = getId(x, y, d);
        // if(id == 33) cerr << id << " " << x << " " << y << " " << (ll) tid << " " << d << endl;
        //if(id == 0) cerr << (ll)tid << endl;
        rep(i, 0, TK - 1) s[i].insert(tid, op);
        // if(id == 33) cerr << s[0].check() << "!" << endl;
    }
    bool check() {
        // cerr << "CHECKING" << id << endl;
        rep(i, 0, TK - 1) if(s[i].check()) return true;
        return false;
    }
    vector < pair < db, db > > recover() {
        vector < tll > ret;
        rep(i, 0, TK - 1) if(s[i].check()) {
            vector < tll > res = s[i].recover();
            for(auto v : res) ret.emplace_back(v);
            break;
        }
        sort(ret.begin(), ret.end());
        ret.resize(unique(ret.begin(), ret.end()) - ret.begin());
        vector < pair < db, db > > ans;
        for(auto v : ret) ans.emplace_back(recXY(v, d));
        return ans;
    }
} S[BID + 1];

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
#endif
    int n; scanf("%d", &n);
    rep(i, 0, BID) S[i].init(i);
    // cerr << "DONE" << endl;
    while(n--) {
        int op, x, y; scanf("%d %d %d", &op, &x, &y);
        rep(i, 0, BID) { S[i].insert(x, y, op == 0 ? 1 : -1); }
        // if(n % 1000 == 0) cerr << n << endl;
    }
    // return 0;
    rep(i, 0, BID) if(S[i].check()) {
        vector < pair < db, db > > T = S[i].recover();
        db ans = 0;
        // cerr << i << " " << S[i].d << "!" << endl;
        // for(auto u : T) cerr << u.first << " " << u.second << endl;
        for(auto u : T) for(auto v : T) chkmax(ans, dis(u, v));
        printf("%.10lf\n", ans); return 0;
    }
    return 0;
}
