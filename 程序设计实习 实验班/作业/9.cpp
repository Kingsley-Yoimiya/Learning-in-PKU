#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

const int M = 15;
const int N = 1e5 + 10;

typedef unsigned long long u64;

int n, q;
u64 a[N];
pair < u64, int > b[M][N];
int tr[M][64];
u64 x;

u64 trans(u64 x, int i) {
    u64 res = 0;
    rep(j, 0, 63) res |= (x >> tr[i][j] & 1ull) << j;
    return res;
}

mt19937 rnd(time(NULL));

int main() {
#ifndef ONLINE_JUDGE
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
#endif
    scanf("%d %d", &n, &q); 
    rep(i, 1, n) scanf("%llu", &a[i]);
    rep(i, 0, M - 1) {
        iota(tr[i] + 0, tr[i] + 64, 0);
        shuffle(tr[i] + 0, tr[i] + 64, rnd);
        rep(j, 1, n) b[i][j] = { trans(a[j], i), j };
        sort(b[i] + 1, b[i] + n + 1);
    }
    while(q--) {
        scanf("%llu", &x);
        vector < pair < int, int > > ret;
        rep(i, 0, M - 1) {
            u64 trx = trans(x, i);
            int p = upper_bound(b[i] + 1, b[i] + n + 1, pair < u64, int >(trx, 0)) - b[i] - 1;
            int p1 = p, p2 = p + 1;
            int tot = 0;
            while(tot < 2 * M) {
                int v1 = -1, v2 = -1;
                if(p1 >= 1) v1 = __builtin_clzll(b[i][p1].first ^ trx);
                if(p2 <= n) v2 = __builtin_clzll(b[i][p2].first ^ trx);
                if(v1 == -1 && v2 == -1) break;
                if(v1 > v2) ret.emplace_back(__builtin_popcountll(b[i][p1].first ^ trx), b[i][p1].second), p1--;
                else ret.emplace_back(__builtin_popcountll(b[i][p2].first ^ trx), b[i][p2].second), p2++;
                tot++;
            }
        }
        //rep(i, 1, n) ret.emplace_back(__builtin_popcountll(a[i] & x), i);
        printf("%d\n", min_element(ret.begin(), ret.end()) -> second); 
    }
    cerr << __builtin_clzll(0) << endl;
    // rep(i, 0, 63) cerr << tr[0][i] << " "; cerr << endl;
    // cerr << a[1] << " " << b[0][1].first << " " << b[0][n].first << " " << b[0][1].first << endl;
    return 0;
}