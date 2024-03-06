#include "streaming.h"
#include <bits/stdc++.h>

using namespace std;

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

int n, k; 
#define TN (256)
#define TP (255)
#define T (10)
#define ST (4900)

#define TID(id, res) (id * TN + (fasthash64(res, id << 10) & TP))

#define query(i, x) Get(TID(i, x))


void init(int _k) {
    n = 0, k = _k;
}

void add(int x) {
	auto tquery = [&](int x) { 
		int mn = n;
		for(int i = 0; i < T; i++) {
			mn = min(mn, query(i, x));
		} return mn;
	};
	++n;
	for(int i = 0; i < T; i++) 
		Set(TID(i, x), Get(TID(i, x)) + 1);
	
	int fst = 0;
	set < int > all;
	for(int i = ST; i < 5000; i++) {
		if(Get(i) > 0 && tquery(Get(i)) < 1. * n / k) {
			Set(i, 0);
			fst = i;
		} else if(Get(i) > 0) all.insert(Get(i));
		else fst = i;
	}
	//cerr << ans_cnt << endl;
	if(tquery(x) >= 1. * n / k && all.find(x) == all.end()) {
		Set(fst, x);
	}
    return;
}

vector <int> report() {
	vector < int > ans;
	for(int i = ST; i < 5000; i++) if(Get(i) > 0) ans.emplace_back(Get(i));
    return ans;
}