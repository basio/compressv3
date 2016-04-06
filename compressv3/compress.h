#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include "util.h"


class compress {
	static long writeHeader(FILE * f, buckets& buckets);
public:
	static 	int incremental_divide(char * filename, buckets& bounds);
	static bound_t WriteCompressedFile(char *outfile, buckets &bounds);
};

class PairCmp
{
public:
	bool operator()(pair<size_t, size_t> n1, pair<size_t, size_t> n2) {
		return n1.second>n2.second;
	}
};
#endif

int main(int argc, char * argv[]);
