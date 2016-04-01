#ifndef _NUMBERS_H_
#define _NUMBERS_H_
#include "compress.h"
class numbers {

	static int* data; //refered to it as numbers
	static int* unique_num;
	static int cnt; //count of the numbers of the numer
	static int unique_cnt;
	static int *unique_cnts;
	static bool is_sorted;
	static bool is_modified; //

	static int compute_unique_values_hash(bool fill_cnts);

	static int compute_unique_values_sort(bool fill_cnts);
	static void bucketing(int b, vector<double>& bounds);
public:
	
	static int read(char * filename);
	static int compute_unique_values(bool fill_cnts);

	
};
#endif // DEBUG

