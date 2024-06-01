#include <bits/stdc++.h>


#define rep(i, x, y) for(int i = (x); i <= (y); i++)
#define per(i, x, y) for(int i = (x); i >= (y); i--)

using namespace std;

template < typename T > void chkmax(T &x, const T &y) { x = x < y ? y : x; }
template < typename T > void chkmin(T &x, const T &y) { x = x < y ? x : y; }

using db = double;
using vec = vector < db >;

const int N = 1200 + 10;
const db eps = 1e-3;

mt19937 rnd(time(NULL));
normal_distribution<> gen(0, 1);

db A[N][N], x[N];
db B[N];
int n = 1200;
char s[N];
db y[N];

db calc(int r) {
    rep(i, 1, n) B[i] = 0;
    rep(i, 1, r) rep(j, 1, n) B[i] += A[i][j] * x[j];
    FILE *f = fopen("temp.lp", "w");
    fprintf(f, "min: ");
    rep(i, 1, n) fprintf(f, "y%d%c", i, "+;"[i==n]);
    fprintf(f, "\n\n");

    rep(i, 1, r) {
        rep(j, 1, n) {
            fprintf(f, "%+.10f x%d%c", A[i][j], j, " ="[j==n]);
        }
        fprintf(f, "%.10f ;\n", B[i]);
    }

    rep(i, 1, n) {
        fprintf(f, "y%d >= x%d;\n", i, i);
        fprintf(f, "y%d >= -x%d;\n", i, i);
    }
    fflush(f);
    system("lp_solve temp.lp  > temp.res");

    FILE *g = fopen("temp.res", "r");
    char s[200], line[200], label[200];
    db value;

    db ret = 0;
    int id = 1;
    while(fgets(line, sizeof(line), g)) {
        if (sscanf(line, "%s %lf", label, &value) == 2 && label[0] == 'y') {
            y[id++] = value;
        }
    }
    // rep(i, 1, n) if(fabs(x[i] - y[i]) > 1e-1) cerr << x[i] << " "<< y[i] << endl;
    rep(i, 1, n) ret += fabs(x[i] - y[i]);
    cerr << r <<  " " << ret << endl;
    //cerr << id << endl;
    return ret;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("tree.txt", "r", stdin);
    freopen("res.out", "w", stdout);
#endif
    int tot = 0,cnt = 0,  _n = 40, _m = 30;
    rep(i, 1, _n) {
        scanf("%s", s + 1);
        rep(j, 1, _m) x[++tot] = s[j] - '0', cnt += s[j] != '0';
    }
    cerr << tot << endl;
    cout << "CNT: " << 1. *  cnt / n << endl;
    rep(i, 1, n) {
        rep(j, 1, n) {
            A[i][j] = gen(rnd);
        }
    }
    cout << "600 RESULT: " << calc(600) << endl;
    // cout << calc(1) << endl;
//    return 0;

    int l = 1, r = n;
    while(l <= r) {
        int mid = l + r >> 1;
        if(calc(mid) < eps) r = mid - 1;
        else l = mid + 1;
    }
    cerr << "R: " << l << endl;
    rep(i, l - 10, l + 2) {
        cout << i << " " << calc(i) << endl;
    }
    return 0;
}