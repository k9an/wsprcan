#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void unpack50( signed char *dat, int32_t *n1, int32_t *n2 );

void unpackcall( int32_t ncall, char *call );

void unpackgrid( int32_t ngrid, char *grid);

void unpackpfx( int32_t nprefix, char *call);

void deinterleave(unsigned char *sym);

// used by qsort
int floatcomp(const void* elem1, const void* elem2);

