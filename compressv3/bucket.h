#include"util.h"
#ifndef _BUCKET_H_
#define _BUCKET_H_

class bucket {
public:
	int start_indx; //start index 
	int start;

	int end_indx;
	int end; //end+1

	bound_t size_in_bits;//
	void update_size(); //compute the size of bucket
	int cnt; //count of elements in this bucket

	void init(int start_indx, int end_indx);

public:
	bucket();
	int bit();
	bound_t delta();
	bucket(int start_indx, int end_indx);	
	bound_t get_size_in_bits();

	//divide bucket b into b and b2
	void divide(bucket &b, bucket &b2,Strategy st);
	bool operator<(const bucket & rhs) const;
	int operator()(const bucket & rhs) const;
	bool operator==(const bucket & rhs) const;
};
 ostream & operator << (ostream & outs, const bucket &b);

#endif // !