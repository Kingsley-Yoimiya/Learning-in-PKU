#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <random>
#include <cstring>
#include <algorithm>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)

using namespace std;

using ll = int;

const int N = 3010;
int n, seed;
int a[N][N], b[N][N], c[N][N];

mt19937 rnd(time(NULL));

int randint(const unsigned int &seed) {
    static unsigned int x = seed;
    x = (x * 23333LL + 23327) % 1000000007;
    return x % 20 - 10;
}

int v[N], l[N], u[N], t[N];

int main() {
    double st = clock();
    scanf("%d %d", &n, &seed);
    //n = 3000, seed = 114514;
    for(int i = 1; i <= n; i ++)
        for(int j = 1; j <= n;j++) {
            a[i][j] = randint(seed);
            b[i][j] = randint(seed);
        }
    rep(i, 1, n) rep(j, 1, n) scanf("%d", &c[i][j]);
    //rep(i, 1, n) rep(j, 1, n) c[i][j] = randint(seed);

    rep(test, 0, 2000) {
        rep(i, 1, n) v[i] = rnd() & 1, u[i] = l[i] = t[i] = 0;
        rep(j, 1, n) rep(k, 1, n) u[k] += v[j] * a[j][k];
        rep(j, 1, n) rep(k, 1, n) l[k] += u[j] * b[j][k];
        rep(j, 1, n) rep(k, 1, n) t[k] += v[j] * c[j][k];
        rep(i, 1, n) if(l[i] != t[i]) return puts("NO"), 0;
        if((clock() - st) / CLOCKS_PER_SEC > 1.0) break;
        //cerr << "Passed test" << test << endl;
    } puts("YES"); return 0;
}