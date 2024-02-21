#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <random>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)

using namespace std;

const int N = 3e6 + 10;

int n, m;
int stu[N], cur;
int u[N], v[N];

mt19937 rnd(time(NULL));

void solve() {
    scanf("%d %d", &n, &m);
    rep(i, 1, m) scanf("%d %d", &u[i], &v[i]);
    rep(i, 1, m) stu[i] = 0;
    double tot = 0;
    int ans = 0;
    rep(i, 1, 5000) {
        cur = 0;
        rep(i, 1, n) stu[i] = rnd() % 2;
        rep(i, 1, m) if(stu[u[i]] ^ stu[v[i]]) cur++;
        tot = tot * (i - 1) + cur;
        tot /= i;
        printf("%.2lf\n", tot);
        if(i % 100 == 0) cerr << "Cur: " << i << endl;
        ans = max(ans, cur);
    }
    cout << ans << endl;
}

int main() {
    freopen("graph.txt", "r", stdin);
    freopen("res.txt", "w", stdout);
    solve();
    return 0;
}