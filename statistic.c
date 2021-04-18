#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

int const REPEATS = 1000;
int const MBL_10PW = 6; // 10^MBL_10PW is max buffer length

size_t const spow(size_t v, size_t p) {
	size_t ans = 1;
	
	while(p) {
		if(p & 1)
			--p, ans *= v;
		else
			p >>= 1, v *= v;
	}
	return ans;
}

void fill_rnd(int* buffer, size_t size) {
	srand((unsigned int)clock());
	for(size_t i = 0; i < size; ++i)
		buffer[i] = (int)rand();
}

size_t const cntnewmax(int* buffer, size_t size) {
	int max = buffer[0];
	size_t cnt = 0;

	for(size_t i = 1; i < size; ++i)
		if(buffer[i] > max)
			max = buffer[i], ++cnt;

	return cnt;
}

int main(int argc, char* argv[]) {
	int buffer[1000000];

	FILE* csvf = fopen("test.dat", "w");

	for(size_t pw = 1; pw <= MBL_10PW; ++pw) {
		size_t size = spow(10, pw);
		size_t cnt = 0;

		for(size_t r = 0; r < REPEATS; ++r) {
			fill_rnd(buffer, size);
			cnt += cntnewmax(buffer, size);
		}

		double mcnt = (double)cnt / (double)REPEATS;
		
		fprintf(csvf, "%zu %lf %lf\n", size, mcnt, log(size));
	}

	fclose(csvf);
}
