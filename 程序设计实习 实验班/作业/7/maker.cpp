#include <bits/stdc++.h>

using namespace std;

int n, k;

const int V = 51;

mt19937 rnd(time(NULL));

int cnt[V + 100];

int main() {
    freopen("1.in", "w", stdout);
    n = 1000000, k = 50;
    
    printf("%d %d\n", n, k);
    for(int i = 1; i <= n; i++) {
        int t = rnd() % V + 1;
        cnt[t]++;
        printf("%d ", t);
    }
    for(int i = 1; i <= V; i++)
        if(cnt[i] >= 1. * n / k)
            cerr << i << " " ;
    cerr << endl;
}