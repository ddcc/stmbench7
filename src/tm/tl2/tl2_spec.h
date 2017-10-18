#include "tl2.h"
#include "../../common/tid.h"

// declaration of things relevant for tl2 only
namespace sb7 {
	static ::Thread *get_thread();

	static ::Thread *get_thread(unsigned id);

	extern ::Thread *tl2_thread_desc[];
}

// definition of functions relevant to tl2 only
static inline ::Thread *sb7::get_thread() {
	return get_thread(get_tid());
}

static inline ::Thread *sb7::get_thread(unsigned id) {
	return tl2_thread_desc[id];
}

// interface implementation
inline void sb7::global_init_tm() {
	TxOnce();
}

inline void sb7::thread_init_tm() {
	unsigned id = get_tid();
	//::Thread *th = get_thread(id);
	//::TxInitThread(th, id);
	//::TxNewThread(th);
	::Thread *th = ::TxNewThread();
	::TxInitThread(th, id);
	tl2_thread_desc[id] = th;
}

inline void sb7::global_clean_tm() {
	// empty
}

inline void sb7::thread_clean_tm() {
	::TxFreeThread(get_thread());
}

inline void *sb7::tm_read_word(void *addr) {
	return (void *)TxLoad(get_thread(), (volatile intptr_t*)addr);
}

inline void sb7::tm_write_word(void *addr, void *val) {
	TxStore(get_thread(), (volatile intptr_t*)addr, (intptr_t)val);
}

#define TX_DATA

#define TX_START ::TxStart(get_thread(), &buf, &ro_flag)

#define TX_COMMIT ::TxCommit(get_thread())

#define TX_ABORT ::TxAbort(get_thread())
