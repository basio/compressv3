#include "util.h"
#include "bucket.h"
#include "buckets.h"
#include "numbers.h"

void bucket::init(int start_indx, int end_indx) {
	this->start_indx = start_indx;
	this->end_indx = end_indx;

	this->cnt = end_indx - start_indx + 1;

	numbers::init(start_indx, end_indx);

	this->start = numbers::get_value(start_indx);
	this->end = numbers::get_value(end_indx);

	update_size();	

	p1 = p2 = -1;

}

bucket::bucket(){
	p1= p2 = -1;
}

bucket::bucket(const bucket & b) {
	start = b.start;
	end = b.end;
	start_indx = b.start_indx;
	end_indx = b.end_indx;
	id = b.id;
	p1 = b.p1;
	p2 = b.p2;
	size_in_bits = b.size_in_bits;
	cnt = b.cnt;
}

bucket::bucket(int start_indx, int end_indx) {
	init(start_indx, end_indx);	
//	id = ++buckets::id;
}

void bucket::divide(bucket b,size_t & p_1, size_t & p_2, Strategy st){
	if ((b.p1 != -1) && (b.p2 != -1)) {
		p_1 = b.p1;
		p_2 = b.p2;
		return;
	}
	
	if (st == Strategy::divide_index) {
		int mid = (b.end_indx + b.start_indx) / 2;
		numbers::divide_index(b.start_indx, b.end_indx);
 		p_1=buckets::createbucket(b.start_indx, mid);
		p_2 = buckets::createbucket(mid+1, b.end_indx);
	}
	else if (st == Strategy::divide_value) {
		int mid_val = (b.end  + b.start) / 2;
		auto bound=numbers::divide_val(b.start_indx, b.end_indx,mid_val);
		p_1 = buckets::createbucket(b.start_indx, bound);
		p_2 = buckets::createbucket(bound + 1, b.end_indx);
	}	
	buckets::_buckets[b.id].p1 = p_1;
	buckets::_buckets[b.id].p2 = p_2;
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
	outs  << b.id<<","<<b.start_indx << "," << b.end_indx <<  ","<< b.start <<","<<b.end <<"," <<b.size_in_bits;
	return outs;
}