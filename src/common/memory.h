// this is just a convinient way of switching memory management
// implementations

#ifdef MM_GC_BOEHM
#include "memory/memory_gc_boehm.h"
#else
#include "memory/memory_explicit.h"
#endif
