#include "stm.h"

inline void sb7::global_init_tm() {
	wlpdstm_global_init();
}

inline void sb7::thread_init_tm() {
	wlpdstm_thread_init();
}

inline void sb7::global_clean_tm() {
	// empty
}

inline void sb7::thread_clean_tm() {
	// empty
}

inline void *sb7::tm_read_word(void *addr) {
  return (void *)wlpdstm_read_word((Word *)addr);
}

inline void sb7::tm_write_word(void *addr, void *val) {
  wlpdstm_write_word((Word *)addr, (Word)val);
}
