#include "buckets.h"
#include "numbers.h"
int buckets::length()
{
	return _buckets.size();
}
bucket & buckets::at(int i) {
	return _buckets[i];
}
void buckets::add(bucket t) {
	_buckets.push_back(t);
	_total_size += t.size_in_bits;
}
void buckets::remove(size_t  indx) {
	bound_t size_ = _buckets[indx].get_size_in_bits();
	_buckets.erase(_buckets.begin()+indx);
	_total_size -= size_;
}
void buckets::remove(bucket t) {
	_buckets.erase(find(_buckets.begin(), _buckets.end(), t));
	_total_size -= t.size_in_bits;
}

bool buckets::is_complete()
{
	bound_t count = 0;
	for (auto it = _buckets.begin(); it != _buckets.end(); it++) {
		count += it->cnt;
	}
	return count == numbers::cnt();
}

bound_t buckets::total_size()
{
	return _total_size;
}

void buckets::sort()
{
	std::sort(_buckets.begin(),_buckets.end());
}

buckets::buckets()
{
	_total_size = 0;
}

vector<bucket>::const_iterator buckets::cbegin() const
{
	return _buckets.cbegin();
}

vector<bucket>::iterator buckets::begin()
{
	return _buckets.begin();
}

vector<bucket>::const_iterator buckets::cend() const
{
	return _buckets.cend();
}

vector<bucket>::iterator buckets::end()
{
	return _buckets.end();
}
