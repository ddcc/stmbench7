#include "../thread/pthread_wrap.h"
#include "tm_ptr.h"

::pthread_key_t sb7::obj_log_key;

void sb7::global_init_obj_log() {
	::pthread_key_create(&obj_log_key, NULL);
}

void sb7::thread_init_obj_log() {
	::pthread_setspecific(obj_log_key,
		(const void *)(new ObjectLog()));
}
