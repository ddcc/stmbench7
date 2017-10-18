#include <iostream>

#include "../tm/tm_ptr.h"

static unsigned test_no;
static unsigned test_succ_no;

void global_init();

void thread_init();
void thread_cleanup();

void run_test1();

int main(int argc, char **argv) {
	std::cout << "********************************" << std::endl;
	std::cout << "***   ObjectLog test start   ***" << std::endl;
	std::cout << "********************************" << std::endl;
	std::cout << std::endl;

	global_init();

	run_test1();

	std::cout << std::endl;
	std::cout << "******************************" << std::endl;
	std::cout << "***   ObjectLog test end   ***" << std::endl;
	std::cout << "***                        ***" << std::endl;
	std::cout << "***   Test success: "
			<< test_succ_no << " / " << test_no
			<< "  ***" << std::endl;
	std::cout << "******************************" << std::endl;
	
}

#define FAIL(num) std::cout << "test" << num << " failed" << std::endl; break
#define SUCC(num) std::cout << "test" << num << " success" << std::endl; test_succ_no++

void run_test1() {
	test_no++;

	thread_init();

	do {
		sb7::ObjectLog * obj_log = sb7::getObjectLog();

		obj_log->put((void *) 1, (void *) 101);
		obj_log->put((void *) 2, (void *) 102);
		obj_log->put((void *) 3, (void *) 103);

		if(obj_log->get((void *) 4) != NULL) {
			FAIL(1);
		}

		if(obj_log->get((void *) 2) != (void *)102) {
			FAIL(1);
		}

		obj_log->put((void *) 2, (void *) 202);

		if(obj_log->get((void *) 2) != (void *)202) {
			FAIL(1);
		}

		obj_log->put((void *) 4, (void *) 104);

		if(obj_log->get((void *) 4) != (void *)104) {
			FAIL(1);
		}

		sb7::obj_log_tx_commit();

		if(obj_log->get((void *) 4) != NULL) {
			FAIL(1);
		}

		SUCC(1);
	} while (false);

	thread_cleanup();
}

void global_init() {
	sb7::global_init_obj_log();

	test_no = 0;
	test_succ_no = 0;
}

void thread_init() {
	sb7::thread_init_obj_log();
}

void thread_cleanup() {
	sb7::obj_log_tx_commit();
}
