#include "util.h"
#include "bucket.h"
#include "buckets.h"
#include "compress.h"
#include "numbers.h"

#define BL 8


void bitpack(char *target, size_t * target_offset, int encoding_length, int source) {
	int j;
	for (j = encoding_length - 1; j >= 0; j--, (*target_offset)++) {
		target[(*target_offset) / BL] |= ((source >> j) & 1) << ((BL - (*target_offset) % BL) - 1);
	}
}

inline void adjust_to_byte(size_t &r) {
	if (r % 8)
		r = r + (8 - r % 8);
}


int main(int argc, char *argv[]) {
	std::ofstream out("c:/data/out.txt");

	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());
	clock_t begin = clock();

	buckets _buckets;
	bound_t sz = compress::incremental_divide(argv[1], _buckets);
	_buckets.sort();
	bound_t ss = compress::WriteCompressedFile("c:/data/o", _buckets);
	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "elapsed_time for sorting (s) " << elapsed_secs << endl;

	cout << "size " << sz / 1024.0 / 1024 << endl;
	cout << "size " << ss / 1024.0 / 1024 << endl;

	cout << _buckets.length() << endl;
	for (auto it = _buckets.cbegin(); it != _buckets.cend(); it++) {
		cout << *it << endl;
	}
	std::cout.rdbuf(coutbuf);
}

int compress::incremental_divide(char* filename, buckets &bounds) {
	int number_of_buckets = 1;
	int increasing = 0;
	int min_b = -1;
	bound_t min = INT32_MAX;

	buckets buckets_bits[32];
	bound_t sz_s[32];

	numbers::read(filename);

	for (int i = 0; i < 32; i++)
		sz_s[i] = i*numbers::cnt();

	bucket b(0, numbers::cnt() - 1);
	buckets_bits[0].add(b);
	number_of_buckets = 1;
	for (int i = 0; i < 32; i++) {

		buckets bs = buckets_bits[i];

		//now iterator over (can have a priority quue)
		for (int j = 0; j < bs.length(); j++) {
			bound_t sz = 0;
			bucket b1, b2;
			bucket &b = bs.at(j);
			b.divide(b1, b2, Strategy::divide_index);
			sz = b1.get_size_in_bits() + b2.get_size_in_bits();

			int size_2 = ceil(log2(bs.length() + 1));
			if (size_2 == i) {
				if (b.get_size_in_bits() > sz) {
					bs.remove(j);
					bs.add(b1);
					bs.add(b2);
				}
			}
			else {
				//copy to bs[i]
				if (buckets_bits[size_2].length() == 0) {
					for (int z = 0; z < bs.length(); z++) {
						buckets_bits[size_2].add(bs.at(z));
					}
				}
				if (b.get_size_in_bits() > sz) {
					buckets_bits[size_2].remove(b);
					buckets_bits[size_2].add(b1);
					buckets_bits[size_2].add(b2);
				}
			}
		}
		//update the sum
		sz_s[i] += bs.total_size();
		if (i > 0) {

			if (sz_s[i] > sz_s[i - 1])
				increasing++;
			else if (sz_s[i] < sz_s[i - 1])
				increasing = 0;
			if (sz_s[i] < min) {
				min = sz_s[i];
				min_b = i;
			}

			if (increasing > 2) {
				bounds = buckets_bits[min_b];
				return min_b; 
			}
		}
	}

	return min_b;
}

long compress::writeHeader(FILE * f, buckets &_buckets) {
	//calculate the bits needed
	int size_bytes = 0;

	double  max_delta = LONG_MIN;
	int  bucket_count = _buckets.length();		//no of buckets
	int tmp = numbers::cnt();
	fwrite(&tmp, sizeof(int), 1, f); size_bytes += 4;
	fwrite(&bucket_count, sizeof(int), 1, f); size_bytes += 4;

	//write the first point
	tmp = _buckets.at(0).start;// sorted_unique_numbers[buckets_starts[0]];
	fwrite(&tmp, sizeof(int), 1, f); size_bytes += 4;

	auto m = std::max_element(_buckets.begin(), _buckets.end(), [](bucket &r, bucket &l) {return l.bit() < r.bit(); });
	char max_b = m->bit();
	//write bits for delta
	char bit_4_delta = ceil(log2(max_delta + 1));
	fwrite(&bit_4_delta, sizeof(char), 1, f); size_bytes += 1;
	fwrite(&max_b, sizeof(char), 1, f); size_bytes += 1;

	//writing the buckets_starts delta
	long sz = (bit_4_delta)* (bucket_count);
	char * bf = (char *)malloc((sizeof(char) * sz / 8) + 1);
	memset(bf, 0, sz / 8 + 1);
	size_t bf_off = 0;
	for (int i = 0; i < bucket_count; i++) {
		bound_t delta = _buckets.at(i).delta();
		bitpack(bf, &bf_off, bit_4_delta, delta);
	}
	adjust_to_byte(bf_off);


	fwrite(bf, sizeof(char), bf_off / 8, f); size_bytes += (bf_off / 8);
	free(bf);
	//Writing the bits for each buckets
	sz = (max_b)* (bucket_count);
	bf = (char *)malloc((sizeof(char) * sz / 8) + 1);
	memset(bf, 0, sz / 8 + 1);

	bf_off = 0;
	for (int i = 0; i < bucket_count; i++) {
		int v = _buckets.at(i).bit();
		bitpack(bf, &bf_off, max_b, v);
	}
	adjust_to_byte(bf_off);

	fwrite(bf, sizeof(char), bf_off / 8, f); size_bytes += (bf_off / 8);
	free(bf);

	return size_bytes;
}

vector<bucket>::const_iterator find(vector<bucket>::const_iterator first, vector<bucket>::const_iterator last, const int val)
{
	vector<bucket>::const_iterator it;
	iterator_traits<vector<bucket>::const_iterator>::difference_type count, step;
	count = std::distance(first, last);
	while (count > 0)
	{
		it = first; step = count / 2; std::advance(it, step);
		if (!(val < it->start))                 // or: if (!comp(val,*it)), for version (2)
		{
			first = ++it; count -= step + 1;
		}
		else count = step;
	}
	return first;
}

bound_t compress::WriteCompressedFile(char * outfile,buckets& _buckets) {

	long sz = 0;
	FILE * out = fopen(outfile, "wb");
	int bucket_bits = ceil(log2(_buckets.length()));
	sz += 8 * writeHeader(out, _buckets);

	//allocate a big buffer (assume no compression)
	long s = numbers::cnt()*(32 + 8) / 8;
	char * bf = (char *)malloc(sizeof(char) * (s + 1));
	memset(bf, 0, s + 1);
	size_t bf_off = 0;
	//compute offset_bits
	vector<int> bits;
	bits.resize(_buckets.length());
	for (int i = 0; i < _buckets.length(); i++) {
		bits[i] = _buckets.at(i).bit();
	}

	for (int i = 0; i < numbers::cnt(); i++) {
		//determine bucket
		int val = numbers::get_value(i);
		vector<bucket>::const_iterator up = find(_buckets.cbegin(), _buckets.cend(), val);
		int index = up - _buckets.begin() - 1;
		bitpack(bf, &bf_off, bucket_bits, index);
		int	off = val - _buckets.at(index).start;
		bitpack(bf, &bf_off, bits[index], off);
	}

	//get the upper bound in bits
	adjust_to_byte(bf_off);
	sz += bf_off;

	fwrite(bf, sizeof(char), bf_off / 8, out);
	free(bf);
	fclose(out);
	return sz;
}



#if 0


int amain(int argc, char *argv[]) {

	int  a[]{ 1,2,3,4,6 };
	auto r = minmax_element(a + 0, a + 4);
	vector<bucket> buckets;
	numbers::read();
	//	numbers::bucketing(0, buckets);
	bucket b(0, 10);
	bucket b1, b2;

	b.divide(b1, b2, Strategy::divide_index);
	cout << "oo" << endl;
	b.divide(b1, b2, Strategy::divide_value);
	return 0;
}
int Mmain(int argc, char *argv[]) {

	std::ofstream out("c://data//out.txt");
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());
	clock_t begin = clock();


	numbers::read(argv[1]);

	clock_t end1 = clock();
	vector<bucket> bounds;
	numbers::bucketing(128, bounds);

	clock_t end2 = clock();

	double elapsed_secs = double(end1 - begin) / CLOCKS_PER_SEC;
	cout << "elapsed_time for read (s) " << elapsed_secs << endl;

	elapsed_secs = double(end2 - end1) / CLOCKS_PER_SEC;
	cout << "elapsed_time for sorting (s) " << elapsed_secs << endl;

	for (auto it = bounds.cbegin(); it != bounds.cend(); it++)
		cout << *it << endl;
	bucket b1;
	bucket b2;
	bounds[10].divide(b1, b2, Strategy::divide_index);
	cout << "divide" << bounds[10] << endl;
	cout << b1 << endl;

	cout << b2 << endl;
	bucket b3;
	bucket b4;

	bounds[10].divide(b3, b4, Strategy::divide_value);

	cout << b3 << endl;

	cout << b4 << endl;
	return 0;

}

#endif


#if 0
//OLD code
bound_t compress::incremental_divide(char* filename, int n, vector<bucket> &bounds) {
	int number_of_buckets = 1;
	bound_t sz = 0;

	queue<bucket> to_process;
	numbers::read(filename);

	if (n > 0) {
		numbers::bucketing(n, bounds);
	}
	else {
		bounds.push_back(bucket(0, numbers::cnt() - 1));
	}

	for (auto it = bounds.begin(); it != bounds.end(); it++)
		to_process.push(*it);
	number_of_buckets = bounds.size();
	while (!to_process.empty()) {
		bucket b1;
		bucket b2;
		double bits;
		double bit_org;
		bucket b = to_process.front();
		to_process.pop();
		b.divide(b1, b2, Strategy::divide_index);
		sz = b1.get_size_in_bits() + b2.get_size_in_bits();
		bits = (log2(number_of_buckets + 1))*numbers::cnt();
		sz += bits;
		bound_t sz_org = b.get_size_in_bits();
		bit_org = (log2(number_of_buckets))*numbers::cnt();
		sz_org += bit_org;

		if (sz_org > sz) {
			vector<bucket>::iterator a = std::find(bounds.begin(), bounds.end(), b);
			*a = b1;
			bounds.push_back(b2);
			to_process.emplace(b1);
			to_process.emplace(b2);
			number_of_buckets++;
		}
	}
	return sz;
}


#endif