
#include"util.h"
#include "bucket.h"
#include "numbers.h"

 int* numbers::data = nullptr; //refered to it as numbers
 int* numbers::unique_num = nullptr;
 int numbers::cnt_; //count of the numbers of the numer
 int numbers::unique_cnt;
 int *numbers::unique_cnts=nullptr;
 bool numbers::is_sorted;
 bool numbers::is_modified; 


static long GetFileSize(char *filename) {
	struct stat stat_buf;
	int rc = stat(filename, &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

int compare(const void *a, const  void *b)
{
	//long aa = *(int*)a;
	//long bb = *(int*)b;
	//if (aa < bb) return -1;
	//if (aa == bb) return 0;
	//return 1;
	//return *(int*)a - *(int*)b;
	/*__asm__ __volatile__(
	"sub %1, %0 \n\t"
	"jno 1f \n\t"
	"cmc \n\t"
	"rcr %0 \n\t"
	"1: "
	: "+r"(a)
	: "r"(b)
	: "cc");*/
	//return a;
	int aa = (*(int*)a);// &0xFFFFfff0;
	int bb = (*(int*)b);// &0xFFFFfff0;
	return (aa < bb) ? -1 : (aa>bb);
	//return (aa < bb) ? -1 : 1;
}

void numbers::bucketing(int max_b, vector<bucket> &buckets) {
#if 0
	vector<int> bounds;
	
	bounds.reserve(1<<(int) log2(max_b));
	auto result = std::minmax_element(data, data + cnt_);
	swap(data[0], data[result.first - data]);
	swap(data[cnt_ - 1], data[result.second - data]);

	bounds.push_back(0);
	bounds.push_back(cnt_-1);

	for (int b = 1; b < max_b; b = b * 2) {
		int l = cnt_ / b;
		for (int i = 0; i < b; i++) {
			int m = (2 * i + 1)*l / 2;
			
			nth_element(data + i*l, data + m, data + (i + 1)*l);
			bounds.push_back(m - 1);
		}
	}
		//compute the buckets
		std::sort(bounds.begin(), bounds.end());
		buckets.reserve(bounds.size());
		buckets.push_back(bucket(bounds[0], bounds[1]));

		for (int i = 1; i < bounds.size() - 1; i += 1) {
			buckets.push_back(bucket(bounds[i] + 1, bounds[i + 1]));
		}
#endif
	}

void numbers::divide_index(int st, int end) {
	std::nth_element(data + st,data+(st+end)/2, data + end);
}

void numbers::init(int st, int end) {
	auto result = minmax_element(data + st , data + end);
	swap(data[st], data[result.first - data]);
	swap(data[end], data[result.second - data]);
}

int numbers::cnt() {
	return cnt_;
}

int numbers::divide_val(int st, int end,int val) {
	int * b= partition(data + st, data + end, [val](int d) { return d < val; });
	int b_indx = b - numbers::data;
	return b_indx;
}

//read a binary file
 int numbers::read(char *filename) {
	long sz = GetFileSize(filename);
	data = (int *)malloc(sz);
	FILE *f = fopen(filename, "rb");
	if (f == nullptr)
		return 1;
	fread(data, sizeof(int), sz / sizeof(int), f);
	fclose(f);
	cnt_ = sz / sizeof(int);

	is_sorted = false;
	is_modified = false;
	return 0;
}

 int numbers::read() {
 
	 data = (int *)malloc(11*4);
	 data[0] = 1;
	 data[1] = 10;
	 data[2] = 8;
	 data[3] = 5;
	 data[4] = 4;
	 data[5] = 3;
	 data[6] = 7;
	 data[7] = 8;
	 data[8] = 6;
	 data[9] = 11;
	 data[10] = 4;

	 cnt_ = 11;

	 is_sorted = false;
	 is_modified = false;
	 return 0;
 }

 int numbers::compute_unique_values(bool fill_cnts) {
	 return compute_unique_values_sort(fill_cnts);
 }

 int numbers::get_value(int indx)  {
	 return data[indx];	 
 }

 int numbers::compute_unique_values_hash(bool fill_cnts) {
	unordered_set<int> unique_numbers_set;

	 for (int i = 0; i < cnt_; i++) {
		 int num = data[i];
		 if (unique_numbers_set.find(num) == unique_numbers_set.end())
			 unique_numbers_set.insert(num);
	 }
	 unique_cnt = unique_numbers_set.size();
	 unique_num = (int*)malloc(unique_numbers_set.size()*sizeof(int));
	 int i=0;
	 for (unordered_set<int>::const_iterator it = unique_numbers_set.cbegin(); it != unique_numbers_set.cend(); it++, i++)
		 	 unique_num[i] = *it;

	 qsort(unique_num, unique_cnt, sizeof(int), compare); //much faster than std::sort
	 
	 if (fill_cnts) {
		 //perpare a map 
		 unique_cnts = (int*)malloc(sizeof(int)*unique_cnt);
		 map<int, int> map_;
		 map<int, int>::iterator it = map_.begin();
		 for (int i = 0; i < unique_cnt; i++) {
			 auto p = pair<int, int>(unique_num[i], i);
			 it = map_.insert(it, p);
		 }
		 for (int i = 0; i < cnt_; i++) {
			 int num = data[i];
			 unique_cnts[map_[num]]++;
		 }
	 }
	 return 0;
 }

 int numbers::compute_unique_values_sort(bool fill_cnts) {
	 
	 is_sorted = true;
	 is_modified = true;

	 qsort(data, cnt_, sizeof(int), compare); //much faster than std::sort
	 vector<int> sorted_unique_numbers;
	 vector<int> u_cnts;
	 unique_cnt = 0;
	 int  prev = data[0] - 1;
	 for (int i = 0; i < cnt_; i++) {
		 if (data[i] == prev) {
			 u_cnts[unique_cnt - 1]++;
		 }
		 else {
			 unique_cnt++;
			 u_cnts.push_back(1);
			 sorted_unique_numbers.push_back(data[i]);
		 }
		 prev = data[i];
	 }
	 
	 unique_cnts = (int*)malloc(unique_cnt*sizeof(int));
	 for (int i = 0; i < unique_cnt; i++) {
		 unique_cnts[i] = u_cnts[i];
	 }
	 return 0;
 }
