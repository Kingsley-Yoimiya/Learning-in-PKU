// 2300012929 尹锦润
#include <cstdio>

unsigned int Bin2BCD(short BIN_in) {
	unsigned int result = 0;
	for(int i = 0; i < 16; i++) {
		result = result << 1 | (BIN_in >> 16 & 1);
		BIN_in <<= 1;
		// printf("--%x\n", result);
		if(i == 15) break;
		for(int j = 7; j >= 0; j--) {
// for every slot, if (>=5) -> += 3
			unsigned int t = result >> (j * 4) & 0xF;
			if(t >= 5) result += 3 << (j * 4);
		} 
		// printf("%x\n", result);
	}
	return result;
}

int main() {
	short x; 
	scanf("%hd", &x);
	printf("%x\n", Bin2BCD(x));
}
