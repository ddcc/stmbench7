// File contains general services expected from tm.
#ifndef SB7_TM_SPEC_H_
#define SB7_TM_SPEC_H_

#ifndef ORIGINAL
# include <ffi.h>

enum stm_merge : unsigned int;
enum stm_merge_policy : unsigned int;
struct stm_merge_context;
struct stm_op_id;
#endif /* ORIGINAL */

namespace sb7 {
	void global_init_tm();

	void thread_init_tm();

	void global_clean_tm();

	void thread_clean_tm();

	void *tm_read_word(void *addr);

	void tm_write_word(void *addr, void *val);

#ifndef ORIGINAL
    void *tm_read_word_tag(void *addr, size_t tag);

    const struct stm_op_id tm_new_op(const char *s, ffi_type *rtype, ffi_type **atypes, unsigned int nargs, void (*f)(void), enum stm_merge (*delayed)(struct stm_merge_context *), const enum stm_merge_policy policy[2]);

    template<typename... Args>
    void tm_begin_op(const struct stm_op_id op, enum stm_merge (*jit)(struct stm_merge_context *), Args... args);

    void tm_end_op(const struct stm_op_id op, const void *rval);
#endif /* ORIGINAL */

    void tm_mem_init();

    void *tm_malloc(size_t size);

    void tm_free(void *addr);
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
