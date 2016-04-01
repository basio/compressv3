#ifndef _BUCKET_H_
#define _BUCKET_H_


#include "compress.h"
class bucket {

	int start_indx; //start index 
	int start;

	int end_indx;
	int end; //end+1

	bound_t size_in_bits;//
	void update_size(); //compute the size of bucket
	int cnt; //count of elements in this bucket

public:
	
	bucket(int start_indx, int end_indx);
	bound_t get_size_in_bits();

};

#endif // !