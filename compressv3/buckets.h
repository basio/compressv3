#include "util.h"
#include "bucket.h"

#ifndef _BUCKETS_H_
#define _BUCKETS_H_
class buckets {
	

	vector<size_t> _buckets_ids;
	bound_t _total_size;

public:

	static vector<bucket> _buckets;
	buckets();

	static size_t createbucket(size_t start_indx, size_t end_indx);
	static bucket & get(size_t indx);
	int length();
	bucket& at(int i);
	size_t get_id(size_t i);
	void add_bucket_id(size_t b_id);
	void remove_bucket_id(size_t indx);
	//void remove(bucket& t);
	
	bound_t total_size();
	//void sort();
	static size_t id;

	vector<size_t>::const_iterator cbegin() const;
	vector<size_t>::iterator begin();
	vector<size_t>::const_iterator cend() const;
	vector<size_t>::iterator end();
};

#endif