#include <iostream>
#include <cstdio>

#include <random>
using namespace std;

mt19937 rnd(time(NULL));

int main() {
    int n = 1000000;
    freopen("1.in", "w", stdout);
    cout << n << " " << 0.04 << endl;
    for(int i = 1; i <= n; i++) {
        cout << rnd() % 1000000007 << " ";
    } cout << endl;
}