#include <iostream>
#include <cassert>
#include <random>
#include <algorithm>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

using db = double;

const double eps = 0.1;
const int N = 1e5 + 10;
const int M = 30;

struct node {
    int x, y, z;
    node() { x = y = z = 0; }
    friend db dis(node a, node b) {
        return sqrt(1. * (a.x - b.x) * (a.x - b.x) + 1. * (a.y - b.y) * (a.y - b.y) + 1. * (a.z - b.z) * (a.z - b.z));
    }
} a[N];

int n, Q;
mt19937 rnd(time(NULL));
vector < pair < db, node > > S;

int main() {
#ifndef ONLINE_JUDGE
    freopen("6.in", "r", stdin);
    freopen("ac.out", "w", stdout);
#endif
    scanf("%d %d", &n, &Q);
    rep(i, 1, n) scanf("%d %d %d", &a[i].x, &a[i].y, &a[i].z);
    shuffle(a + 1, a + n + 1, rnd);
    node cent = a[1];
    db tot = 0;
    rep(i, 1, n) tot += dis(cent, a[i]);
    db r = tot * eps / n;
    sort(a + 1, a + n + 1, [&](node a, node b) {
        return dis(cent, a) < dis(cent, b);
    });
    int cur = 0;
    while(cur < n) {
        int lcur = cur + 1;
        while(cur < n && dis(cent, a[cur + 1]) <= 2 * r) cur++;
        auto gener = [&](int l, int r) {
            int t = r - l + 1;
            vector < int > p;
            rep(i, 1, M) {
                int t = 1. * rnd() / rnd.max() * (r - l + 1) + l;
                p.emplace_back(t);
            } 
            sort(p.begin(), p.end());
            p.resize(unique(p.begin(), p.end()) - p.begin());
            return p;
        };
        if(lcur <= cur) {
            vector < int > id = gener(lcur, cur);
            int lsz = id.size();
            for(auto v : id)
                S.emplace_back(1. * (cur - lcur + 1) / lsz, a[v]);
        }
        r *= 2;
    }
    //for(auto v : S) cerr << v.first << " " << v.second.x << endl;
    while(Q--) {
        node ask; scanf("%d %d %d", &ask.x, &ask.y, &ask.z);
        db ans = 0;
        for(auto v : S) {
            ans += v.first * dis(v.second, ask);
        } printf("%.12lf\n", ans);
    }
}