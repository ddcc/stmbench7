#ifndef SB7_EXCEPTION_H_
#define SB7_EXCEPTION_H_

namespace sb7 {

	class Sb7Exception {
		public:
			Sb7Exception() {
			}

			Sb7Exception(const char *m) : msg(m) {
			}

			const char *getMsg() {
				return msg;
			}

		protected:
			const char *msg;
	};

}

#endif /*SB7_EXCEPTION_H_*/
