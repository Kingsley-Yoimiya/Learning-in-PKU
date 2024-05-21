#include "rank.h"
#include <algorithm>
#include <iostream>
#include <cassert>
using namespace std;
const int N = 1e6 + 6, LIM = 5e3;
const double eps = 0.03;
int n, cnt, a[N]; double p;
int query(int i){
    if (i < 1 || i > n){
        cout << "invalid query!" << endl;
        assert(false);
    }
    ++ cnt;
    if (cnt > LIM){
        cout << "too many queries!" << endl;
        assert(false);
    }
    return a[i];
}
int main(){
    double p;
    scanf("%d %lf", &n, &p);
    for (int i = 1; i <= n; ++ i){
        scanf("%d", &a[i]);
    }
    int ans = rnk(n, p);
    sort(a + 1, a + 1 + n);
    int t = lower_bound(a + 1, a + 1 + n, ans) - a;
    cout << "your answer is : " << ans << endl;
    cout << "the times you sample is : " << cnt << endl;
    if (a[t] != ans){
        cout << "your answer isn't from the array!" << endl;
        assert(false);
    }
    cout << "the rank of your answer is : " << t << endl;
    if (t < (p - eps) * n || t > (p + eps) * n){
        cout << "the range is [" << (p - eps) * n << "," << (p + eps) * n << "]" << endl;
        cout << "out of range!" << endl;
        assert(false);
    }
    cout << "correct!" << endl;
    return 0;
}