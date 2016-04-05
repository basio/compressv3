#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include "util.h"


class compress {
	static long writeHeader(FILE * f, buckets& buckets);
public:
	static 	int incremental_divide(char * filename, buckets& bounds);
	static bound_t WriteCompressedFile(char *outfile, buckets &bounds);
};

#endif

int main(int argc, char * argv[]);
