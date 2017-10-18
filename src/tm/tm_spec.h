// File contains general services expected from tm.
#ifndef SB7_TM_SPEC_H_
#define SB7_TM_SPEC_H_

namespace sb7 {
	void global_init_tm();

	void thread_init_tm();

	void global_clean_tm();

	void thread_clean_tm();

	void *tm_read_word(void *addr);

	void tm_write_word(void *addr, void *val);
}

// in addition to defining these functions, tm specific functions
// must also define macros: TX_DATA, TX_START, TX_COMMIT and TX_ABORT

// now include tm specific files
#ifdef STM_TL2
#include "tl2/tl2_spec.h"
#elif defined STM_TINY_STM
#include "ts/ts_spec.h"
#elif defined STM_WLPDSTM
#include "wlpdstm/wlpdstm_spec.h"
#endif

#endif
