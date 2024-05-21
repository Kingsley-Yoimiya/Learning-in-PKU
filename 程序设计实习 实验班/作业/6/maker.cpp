#include <bits/stdc++.h>

#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

const int V = 1e9;

mt19937 rnd(time(NULL));

int main(int argc, char **argv) {
    freopen("6.in", "w", stdout);
    int n = atoi(argv[1]), q = atoi(argv[2]);
    printf("%d %d\n", n, q);
    rep(i, 1, n)
        printf("%d %d %d\n", rnd() % (2 * V) - V, rnd() % (2 * V) - V, rnd() % (2 * V) - V);
    rep(i, 1, q)
        printf("%d %d %d\n", rnd() % (2 * V) - V, rnd() % (2 * V) - V, rnd() % (2 * V) - V);
}