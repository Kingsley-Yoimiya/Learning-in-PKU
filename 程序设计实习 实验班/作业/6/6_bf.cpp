#include <iostream>
#include <cassert>
#include <random>
#include <algorithm>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

using db = double;

const double eps = 1e-3;
const int N = 1e5 + 10;
const int M = 50;

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
    freopen("6.in", "r", stdin);
    freopen("bf.out", "w", stdout);
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
    while(Q--) {
        node ask; scanf("%d %d %d", &ask.x, &ask.y, &ask.z);
        db ans = 0;
        rep(i, 1, n) {
            ans += dis(ask, a[i]);
        } printf("%.12lf\n", ans);
    }
}