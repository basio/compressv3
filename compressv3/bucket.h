#include"util.h"
#ifndef _BUCKET_H_
#define _BUCKET_H_

class bucket {
private:
	
	

public:
	int start_indx; //start index 
	int start;
	size_t id;

	int end_indx;
	int end; //end+1

	bound_t size_in_bits;//
	void update_size(); //compute the size of bucket
	int cnt; //count of elements in this bucket

	void init(int start_indx, int end_indx);
	int p1, p2;

public:

	bucket();
	bucket(const bucket &b);
	bucket(int start_indx, int end_indx);

	bound_t get_size_in_bits();
	int bit();
	bound_t delta();

	//divide bucket b into b and b2
	static void divide(bucket b,size_t &b1, size_t &b2,Strategy st);
	bool operator<(const bucket & rhs) const;
	int operator()(const bucket & rhs) const;
	bool operator==(const bucket & rhs) const;
};
 ostream & operator << (ostream & outs, const bucket &b);

#endif // !