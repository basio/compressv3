#include "compress.h"

int main(int argc, char *argv[]) {
	
	for (int b = 1; b < 100; b = b * 2) {
		int l = 1000 / b;
		for (int i = 0; i < b; i++) {
			int m = (2 * i + 1)*l / 2;
		//	std::nth_element(data + i*l, data + m, data + (i + 1)*l);
			//bounds.push_back(data[m]);
			cout << m << endl;
		}
	}
	return 0;

	clock_t begin = clock();

	
	numbers::read(argv[1]);
	clock_t end1 = clock();
	numbers::compute_unique_values(false);

	clock_t end2 = clock();

	double elapsed_secs = double(end1 - begin) / CLOCKS_PER_SEC;
	printf("elapsed_time for read (s) %f\n", elapsed_secs);

	 elapsed_secs = double(end2 - end1) / CLOCKS_PER_SEC;
	printf("elapsed_time for sorting (s) %f\n", elapsed_secs);
}