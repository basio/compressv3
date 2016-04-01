#include"bucket.h"

bound_t bucket::get_size_in_bits() {

	return size_in_bits;
}

void bucket::update_size() {
	int bits_for_buckets = ceil(log2(end-start));

	size_in_bits = bits_for_buckets*cnt;
	size_in_bits += (32 + 5); //bits
}