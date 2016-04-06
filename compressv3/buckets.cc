#include "util.h"
#include "buckets.h"
#include "numbers.h"

size_t buckets::id = -1;
vector<bucket> buckets::_buckets;

size_t buckets::createbucket(size_t start_indx, size_t end_indx) {
	bucket t(start_indx, end_indx);
	t.id= ++buckets::id;
	_buckets.emplace_back( t);	
	return t.id;
}

bucket& buckets::get(size_t indx) {
	return _buckets[indx];
}

int buckets::length() {
	return _buckets_ids.size();
}


bucket & buckets::at(int i) {
	int indx= _buckets_ids[i];
	return _buckets[indx];
}
size_t buckets::get_id(size_t i) {
	return _buckets_ids[i];
}

void buckets::add_bucket_id(size_t b_id) {
	_buckets_ids.push_back(b_id);
	bucket &t = _buckets[b_id];
	_total_size += t.size_in_bits;
}

void buckets::remove_bucket_id(size_t  b_id) {
	bucket &t = _buckets[b_id];
	_buckets_ids.erase(std::remove(_buckets_ids.begin(), _buckets_ids.end(), b_id), _buckets_ids.end());
	_total_size -= t.get_size_in_bits();
}
/*void buckets::remove(bucket &t) {
	_buckets_ids.erase(find(_buckets_ids.begin(), _buckets_ids.end(), t.id));
	_total_size -= t.size_in_bits;
}*/

bound_t buckets::total_size()
{
	return _total_size;
}


buckets::buckets()
{
	_total_size = 0;
}

vector<size_t>::const_iterator buckets::cbegin() const
{
	return _buckets_ids.cbegin();
}

vector<size_t>::iterator buckets::begin()
{
	return _buckets_ids.begin();
}

vector<size_t>::const_iterator buckets::cend() const
{
	return _buckets_ids.cend();
}

vector<size_t>::iterator buckets::end()
{
	return _buckets_ids.end();
}


/*
bool buckets::is_complete()
{
bound_t count = 0;
for (auto it = _buckets.begin(); it != _buckets.end(); it++) {
count += it->cnt;
}
return count == numbers::cnt();
}*/


//void buckets::sort(){
//std::sort(_buckets_ids.begin(), _buckets_ids.end());
//}
