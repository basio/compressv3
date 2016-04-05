#include "util.h"
#include "bucket.h"
#include "numbers.h"

void bucket::init(int start_indx, int end_indx) {
	this->start_indx = start_indx;
	this->end_indx = end_indx;

	this->cnt = end_indx - start_indx + 1;

	numbers::init(start_indx, end_indx);

	this->start = numbers::get_value(start_indx);
	this->end = numbers::get_value(end_indx);

	update_size();	
}

bucket::bucket(){
}

bucket::bucket(int start_indx, int end_indx) {
	init(start_indx, end_indx);
}

void bucket::divide(bucket & b1, bucket & b2, Strategy st){
	if (st == Strategy::divide_index) {
		int mid = (end_indx + start_indx) / 2;
		numbers::divide_index(start_indx, end_indx);
		b1.init(start_indx, mid);
		b2.init(mid+1, end_indx);			
	}
	else if (st == Strategy::divide_value) {
		int mid_val = (end  + start) / 2;
		auto bound=numbers::divide_val(start_indx, end_indx,mid_val);
		b1.init(start_indx, bound);
		b2.init(bound+1, end_indx);
	}	
}

bool bucket::operator<(const bucket &rhs) const {
	return start_indx < rhs.start_indx;
}
int bucket::operator()(const bucket & rhs) const {
	return size_in_bits < rhs.size_in_bits;
}
bool bucket::operator==(const bucket & rhs) const
{
	return rhs.start_indx==start_indx;
}
bound_t bucket::get_size_in_bits() {
	return size_in_bits;
}

int bucket::bit() {
	return ceil(log2(end - start + 1));
}

bound_t bucket::delta() {
	return (bound_t)end -(bound_t) start + 1; //check this one
}

void bucket::update_size() {
	int bits_for_buckets = bit();

	size_in_bits = bits_for_buckets*cnt;
	size_in_bits += (32 + 5); //bits
}

 ostream & operator << (ostream & outs, const bucket &b)  {
	outs  << b.start_indx << "," << b.end_indx <<  ","<< b.start <<","<<b.end <<"," <<b.size_in_bits;
	return outs;
}