#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

const int V = 1e9;
using db = double;

mt19937 rnd(time(NULL));

const int N = 1e5 + 10;
const db eps = 0.2;

db t1[N], t2[N];
int n;

int main() {
    freopen("6.in", "r", stdin);
    scanf("%d", &n);
    freopen("ac.out", "r", stdin);
    rep(i, 1, n) scanf("%lf", &t1[i]);
    freopen("bf.out", "r", stdin);
    rep(i, 1, n) scanf("%lf", &t2[i]);
    rep(i, 1, n) {
        db t = t2[i];
        db l = t * (1 - eps), r = t * (1 + eps);
        if(t1[i] >= l && t1[i] <= r) continue;
        printf("WA on %d %.12lf %.12lf\n", i, t1[i], t2[i]);
        return -1;
    } return 0;
}