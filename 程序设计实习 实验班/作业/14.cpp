#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

using db = double;
//using node = tuple < int, int, int >;
using vec = vector < db >;

//const db eps = 0.5; //0.3 / 4;
const int N = 5e4 + 10;
const int V = 1e8;

mt19937 rnd(time(NULL));

int n, D;

struct node {
    vec t; int id;
    node(vec _t = vec(), int _id = 0) : t(_t), id(_id) {}
};

node operator + (node a, node b) {
    node c = a;
    rep(i, 0, D - 1) c.t[i] += b.t[i];
    return c;
}

node operator / (node a, db t) {
    node c = a;
    rep(i, 0, D - 1) c.t[i] /= t;
    return c;
}

bool operator < (node a, node b) {
    rep(i, 0, D - 1) if(a.t[i] != b.t[i]) return a.t[i] < b.t[i];
    return 0;
}


int get_pos(node a, node mid) {
    int pos = 0;
    rep(i, 0, D - 1) if(a.t[i] > mid.t[i]) pos |= 1 << i;
    return pos;
}

db dis(node x, node y) {
    db t = 0; 
    rep(i, 0, D - 1) t += (x.t[i] - y.t[i]) * (x.t[i] - y.t[i]);
    return sqrt(t);
}

vector < pair < int, int > > ans;

node build(vector < node > pot, int dep, node left, node right) {
    //cerr << pot.size() << " " << dep << endl;
    swap(left, right);
    for(auto v : pot) rep(j, 0, D - 1) left.t[j] = min(left.t[j], v.t[j]), right.t[j] = max(right.t[j], v.t[j]);

    if(pot.size() <= 1 or dis(left, right) <= 1e-1) {
        for(auto x : pot) if(x.id != pot[0].id) ans.emplace_back(x.id, pot[0].id);
        return pot[0];
    }
    node mid = left / 2 + right / 2;
    vector < pair < int, node > > cpot;
    db diam = dis(left, right), edlen = diam * sqrt(D);
    
    for(auto x : pot) cpot.emplace_back(get_pos(x, mid), x);
    sort(cpot.begin(), cpot.end());
    vector < node > tpot;
    rep(l, 0, (int)pot.size() - 1) {
        int r = l; while(r < pot.size() and cpot[r].first == cpot[l].first) r++;
        vector < node > tmp; rep(i, l, r - 1) tmp.emplace_back(cpot[i].second);
        node lleft = left, lright = right;
        rep(i, 0, D - 1) if(cpot[l].first & (1 << i)) lleft.t[i] = mid.t[i]; else lright.t[i] = mid.t[i];
        tpot.emplace_back(build(tmp, dep + 1, lleft, lright));
        l = r - 1;
    }
    sort(tpot.begin(), tpot.end(), [&](node a, node b) { return dis(a, mid) < dis(b, mid); });
    for(auto v : tpot) if(v.id != tpot[0].id) ans.emplace_back(v.id, tpot[0].id);
    return tpot[0];
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
#endif 
    scanf("%d %d", &n, &D);
    vector < node > pot;
    rep(i, 1, n) { 
        node v; v.id = i;
        rep(j, 0, D - 1) { int x; scanf("%d", &x); v.t.push_back(x); }
        pot.emplace_back(v);
    }
    node shift; rep(i, 0, D - 1) shift.t.push_back(rnd() % V);
    for(auto &v : pot) v = v + shift;
    node left, right;
    rep(i, 0, D - 1) left.t.emplace_back(0), right.t.emplace_back(V * 2);
    build(pot, 0, left, right);
    assert(ans.size() == n - 1);
    for(auto v : ans) printf("%d %d\n", v.first, v.second);
    return 0;
}