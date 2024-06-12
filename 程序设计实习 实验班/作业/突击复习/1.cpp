#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

const int N = 2010;

using db = double;

int n, m;
int u[N], v[N];
vector < int > ans;
int ans_res;
int col[N];
mt19937 rnd(19491001);

void solve() {
    scanf("%d %d", &n, &m);
    // cerr << n << " " << m << endl;
    rep(i, 1, m) scanf("%d %d", &u[i], &v[i]);
    ans_res = -1; ans.clear();
    // db st = clock();
    rep(_, 1, 10) {
        rep(i, 1, n) col[i] = rnd() % 2;
        int cnt = 0;
        rep(i, 1, m) if(col[u[i]] ^ col[v[i]]) cnt++;
        // cerr << cnt << endl;
        if(cnt > ans_res) {
            ans_res = cnt; ans.clear();
            rep(i, 1, n) if(col[i] == 1) ans.emplace_back(i);
        }
    } printf("%d\n", ans.size()); for(auto v : ans) printf("%d ", v); puts("");
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    // freopen("1.out", "w", stdout);
#endif
    int T; scanf("%d", &T);
    while(T--) solve();
    return 0;
}