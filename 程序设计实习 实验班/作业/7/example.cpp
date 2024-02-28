#include "streaming.h"
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
void init(int _n, int _k) {
    n = _n, k = _k; 
}
void add(int x) {
    return;
}
vector <int> report() {
    return {};
}