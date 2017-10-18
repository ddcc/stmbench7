#ifndef SB7_TM_TX_H_
#define SB7_TM_TX_H_

#include <cstdlib>

namespace sb7 {
	typedef void *tx_param;
	typedef void *tx_abort_param;

	typedef void *(*tx_fun)(tx_param);
	typedef void (*tx_abort_fun)(tx_abort_param);

	void *run_tx(
		tx_fun fun,
		int ro_flag = 0,
		tx_param param = NULL,
		tx_abort_fun abort_fun = NULL,
		tx_abort_param abort_param = NULL);
}

#endif
