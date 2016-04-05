#ifndef _NUMBERS_H_
#define _NUMBERS_H_

#include "util.h"

class numbers {

	static int* data; //refered to it as numbers
	static int* unique_num;
	static int cnt_; //count of the numbers of the numer
	static int unique_cnt;
	static int *unique_cnts;
	static bool is_sorted;
	static bool is_modified; //

	static int compute_unique_values_hash(bool fill_cnts);
	static int compute_unique_values_sort(bool fill_cnts);
public:
	static void bucketing(int b, vector<bucket>& bounds);
	static void divide_index(int st, int end);
	static int divide_val(int st, int end,int val);
	static int read(char * filename);
	static int read();
	static int compute_unique_values(bool fill_cnts);	
	static int get_value(int indx);
	static void init(int st, int end);
	static int cnt();
};
#endif // DEBUG

