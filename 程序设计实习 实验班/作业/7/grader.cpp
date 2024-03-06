#include "streaming.h"

using namespace std;

const int N = 5000; 

int arr[N];

void Set(int a, int b) {
    if(a < 0 || a >= N) assert(0);
    arr[a] = b; 
}
int Get(int a) {
    if(a < 0 || a >= N) assert(0);
    return arr[a];
}

int main() {
    int n, k; 
    scanf("%d%d", &n, &k);
    init(k);
    // cout << n << ' ' << k << endl;
    for(int i = 1; i <= n; i++) {
        int x;
        scanf("%d", &x);
        // cout << "add " << x << endl;
        add(x);
    }
    vector <int> ans = report();
    sort(ans.begin(), ans.end());
    cout << ans.size() << '\n';
    for(int x : ans) cout << x << ' ';
    //cerr << ans[0] << endl;
}