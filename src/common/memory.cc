// this is just a convinient way of switching memory management
// implementations

#ifdef MM_GC_BOEHM

#else

#include "memory/memory_explicit.cc"

#endif
