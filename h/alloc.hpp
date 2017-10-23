#ifndef H_ALLOC
#define H_ALLOC

#include <cstdlib>

#define alloc(p,a,t) do \
if (!(p = (t *) malloc ((size_t) ((a) * sizeof (t))))) { \
fprintf(stderr,"run out of memory [alloc(%s,%d,%s)]\n",#p,a,#t); \
exit(1); \
} while(0);

#endif // H_ALLOC
