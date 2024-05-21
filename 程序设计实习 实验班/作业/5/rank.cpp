#include <iostream>
#include <random>
#include <algorithm>

#include "rank.h"
using namespace std;

#define rep(i, x, y) for(int i = (x); i <= (y); i++)

int t[5005];
mt19937 rnd(time(NULL));

const int LIM = 5000;

int rnk(int n, double p){
    rep(i, 1, LIM) {
        int pos = rnd() % n + 1;
        t[i] = query(pos);
    }
    sort(t + 1, t + LIM + 1);
    int v = round(LIM * p);
    v = max(v, 1), v = min(v, LIM);
    return t[v];
}