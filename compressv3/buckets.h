#include "util.h"
#include "bucket.h"

#ifndef _BUCKETS_H_
#define _BUCKETS_H_
class buckets {

	vector<bucket> _buckets;
	bound_t _total_size;

public:
	int length();
	bucket& at(int i);
	void add(bucket t);
	void remove(size_t indx);
	void remove(bucket t);
	bool is_complete();
	bound_t total_size();
	void sort();
	buckets();
	vector<bucket>::const_iterator cbegin() const;
	vector<bucket>::iterator begin();
	vector<bucket>::const_iterator cend() const;
	vector<bucket>::iterator end();
};

#endif