#include <iostream>
#include <cassert>
#include <random>
using namespace std;
const int T = 1e6;
const double eps = 1e-2;
random_device rd;
class Generator{
    private:
        minstd_rand gen;
    public:
        int rand(){ return gen() % 2; }
};

int bern(double p, Generator *G){
    if(abs(p - 1) < eps) return 1;
    if(abs(p - 0) < eps) return 0;
    int t = G->rand();
    if(abs(p - 0.5) < eps) return t;
    if(p > 0.5) {
        if(t == 1) return 1;
        else return bern((p - 0.5) * 2, G);
    } else {
        if(t == 0) return 0;
        return bern(p * 2, G);
    }
}
int main(){
    double p; int T;
    while (cin >> p >> T){
        Generator RAND, CHECK;
        int tmp = bern(p, &CHECK);
        for (int i = 1; i <= T; ++ i){
            Generator CHECK;
            if (tmp != bern(p, &CHECK)){
               cout << "using external randomness!" << endl;
                cout << "WA" << endl;
                return 0;
            }
        }
        int c = 0;
        for (int i = 1; i <= T; ++ i)
            c += bern(p, &RAND);
        cout << "the frequncy is : " << (double)c / T << endl;
        if (c < (p - eps) * T || c > (p + eps) * T){
            cout << "WA" << endl;
            return 0;
        }
    }
    cout << "AC" << endl;
    return 0;
}