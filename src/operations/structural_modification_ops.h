#ifndef SB7_STRUCTURAL_MODIFICATION_H_
#define SB7_STRUCTURAL_MODIFICATION_H_

#include "../tm/tm_ptr.h"

#include "operations.h"

namespace sb7 {

	class StructuralModification1 : public Operation {
		public:
			StructuralModification1(DataHolder *dh)
				: Operation(STRUCTURAL_MODIFICATION, "SM1", dh) {
			}

			virtual int run() const;
	};

	class StructuralModification2 : public Operation {
		public:
			StructuralModification2(DataHolder *dh)
				: Operation(STRUCTURAL_MODIFICATION, "SM2", dh) {
			}

			virtual int run() const;
	};

	class StructuralModification3 : public Operation {
		public:
			StructuralModification3(DataHolder *dh)
				: Operation(STRUCTURAL_MODIFICATION, "SM3", dh) {
			}

			virtual int run() const;
	};

	class StructuralModification4 : public Operation {
		public:
			StructuralModification4(DataHolder *dh)
				: Operation(STRUCTURAL_MODIFICATION, "SM4", dh) {
			}

			virtual int run() const;
	};

	class StructuralModification5 : public Operation {
		public:
			StructuralModification5(DataHolder *dh)
				: Operation(STRUCTURAL_MODIFICATION, "SM5", dh) {
			}

			virtual int run() const;
	};

	class StructuralModification6 : public Operation {
		public:
			StructuralModification6(DataHolder *dh)
				: Operation(STRUCTURAL_MODIFICATION, "SM6", dh) {
			}

			virtual int run() const;
	};

	class StructuralModification7 : public Operation {
		public:
			StructuralModification7(DataHolder *dh)
				: Operation(STRUCTURAL_MODIFICATION, "SM7", dh) {
			}

			virtual int run() const;
	};

	class StructuralModification8 : public Operation {
		public:
			StructuralModification8(DataHolder *dh)
				: Operation(STRUCTURAL_MODIFICATION, "SM8", dh) {
			}

			virtual int run() const;
	};

}

#endif // SB7_STRUCTURAL_MODIFICATION_H_
