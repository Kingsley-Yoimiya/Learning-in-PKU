#include <bits/stdc++.h>

using namespace std;

const int M = 50;
const int N = 1010;
const int V = (1 << 30) - 1;

mt19937_64 rnd(time(NULL));

const int TP = 5;

int a[N][N], n, m, q;
// struct pit {
//     int v[TP];
//     pit() { for(int i = 0; i < TP; i++) v[i] = rnd() & V; }
//     bool operator < (const pit &p) const {
//         for(int i = 0; i < TP; i++) if(v[i] != p.v[i]) return v[i] < p.v[i];
//         return 0;
//     }
//     bool operator == (const pit &p) const{
//         for(int i = 0; i < TP; i++) if(v[i] != p.v[i]) return 0;
//         return 1;
//     }
// };

typedef unsigned long long u64;

#define mix(h) ({					\
			(h) ^= (h) >> 23;		\
			(h) *= 0x2127599bf4325c37ULL;	\
			(h) ^= (h) >> 47; })

u64 fasthash64(u64 v, u64 seed) {
	const uint64_t    m = 0x880355f21e6d1965ULL;
	u64 h = seed;
	h ^= mix(v);
	h *= m;
	return mix(h);
}


using pit = u64;
pit res[M][N];
pit v[N];

int b[N * N], cnt;
u64 c[N * N];

int main() {
    u64 seed = 114514;
    scanf("%d %d %d", &n, &m, &q);
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++) scanf("%d", &a[i][j]);
    }
    cnt = 0;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++) b[++cnt] = a[i][j];
    }
    sort(b + 1, b + cnt + 1);
    cnt = unique(b + 1, b + cnt + 1) - b - 1;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++) a[i][j] = lower_bound(b + 1, b + cnt + 1, a[i][j]) - b;
    }
    for(int t = 0; t < M; t++) {
        for(int i = 1; i <= cnt; i++) c[i] = rnd();//fasthash64(1ull * b[i] * t, seed);
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= m; j++) {
                v[j] = c[a[i][j]];
            }
            res[t][i] = *min_element(v + 1, v + m + 1);
        }
    }
    while(q--) {
        int x, y; scanf("%d %d", &x, &y);
        int cnt = 0;
        for(int t = 0; t < M; t++) {
            cnt += res[t][x] == res[t][y];
            //cerr << res[t][x] << " " << res[t][y] << endl;
        }
        printf("%.8lf\n", 1. * cnt / M);
    }
    return 0;
}