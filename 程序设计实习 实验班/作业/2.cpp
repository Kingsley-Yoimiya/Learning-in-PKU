#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <random>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)

using namespace std;

const int N = 2010;

int n, m;
int stu[N], cur;
int u[N], v[N];

mt19937 rnd(time(NULL));

void solve() {
    cin >> n >> m;
    rep(i, 1, m) cin >> u[i] >> v[i];
    rep(i, 1, m) stu[i] = 0;
    while(true) {
        cur = 0;
        rep(i, 1, n) stu[i] = rnd() % 2;
        rep(i, 1, m) if(stu[u[i]] ^ stu[v[i]]) cur++;
        if(cur >= m * 0.45) {
            vector < int > p;
            rep(i, 1, n) if(stu[i]) p.emplace_back(i);
            printf("%d\n", p.size());
            for(auto v : p) printf("%d ", v);
            puts("");
            break;
        }
    }
}

int main() {
    int T; cin >> T;
    while(T--) solve();
    return 0;
}