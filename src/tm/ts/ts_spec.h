#include "stm.h"
#include "wrappers.h"

#include "../../sb7_exception.h"

inline void sb7::global_init_tm() {
	::stm_init(NULL);
}

inline void sb7::thread_init_tm() {
	::stm_init_thread();
}

inline void sb7::global_clean_tm() {
	::stm_exit();
}

inline void sb7::thread_clean_tm() {
	::stm_exit_thread();
}

inline void *sb7::tm_read_word(void *addr) {
	return (void *)stm_load((const stm_word_t *)addr);
}

inline void sb7::tm_write_word(void *addr, void *val) {
	stm_store((stm_word_t *)addr, (stm_word_t)val);
}

#ifndef ORIGINAL
inline void *sb7::tm_read_word_tag(void *addr, size_t tag) {
    return (void *)stm_load_tag((const stm_word_t *)addr, tag);
}

inline const struct stm_op_id sb7::tm_new_op(const char *s, ffi_type *rtype, ffi_type **atypes, unsigned int nargs, void (*f)(void), enum stm_merge (*delayed)(struct stm_merge_context *), const enum stm_merge_policy policy[2]) {
    stm_op_id_t ret = STM_INVALID_OPID;

    ffi_cif cif;
    const ffi_status ffi = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, nargs, rtype, atypes);
    if (ffi == FFI_OK)
        ret = stm_new_opcode(s, &cif, FFI_FN(f), delayed, policy);
    return ret;
}

template<typename... Args>
inline void sb7::tm_begin_op(struct stm_op_id op, enum stm_merge (*jit)(struct stm_merge_context *), Args... args) {
    int ret = stm_begin_op(op, jit, args...);
    if (!ret)
        throw Sb7Exception("Error starting operation!");
}

inline void sb7::tm_end_op(struct stm_op_id op, const void *rval) {
    int ret = stm_end_op(op, rval);
    if (!ret)
        throw Sb7Exception("Error ending operation!");
}
#endif /* ORIGINAL */
