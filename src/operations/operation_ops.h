#ifndef SB7_OPERATION_OPS_H_
#define SB7_OPERATION_OPS_H_

#include "../tm/tm_ptr.h"

#include "operations.h"
#include "query_ops.h"
#include "traversal_ops.h"

namespace sb7 {

	class Operation6 : public Operation {
		protected:
			Operation6(optype t, const char *n, DataHolder *dh)
				: Operation(t, n, dh) {
			}

		public:
			Operation6(DataHolder *dh) : Operation(OPERATION_RO, "OP6", dh) {
			}

			virtual int run() const;

		protected:
			virtual void performOperationOnComplexAssembly(
				sh_ptr<ComplexAssembly> cassm) const;
	};

	class Operation7 : public Operation {
		protected:
			Operation7(optype t, const char *n, DataHolder *dh)
				: Operation(t, n, dh) {
			}

		public:
			Operation7(DataHolder *dh) : Operation(OPERATION_RO, "OP7", dh) {
			}

			virtual int run() const;

		protected:
			virtual void performOperationOnBaseAssembly(
				sh_ptr<BaseAssembly> bassm) const;
	};

	class Operation8 : public Operation {
		protected:
			Operation8(optype t, const char *n, DataHolder *dh)
				: Operation(t, n, dh) {
			}

		public:
			Operation8(DataHolder *dh) : Operation(OPERATION_RO, "OP8", dh) {
			}

			virtual int run() const;

		protected:
			virtual void performOperationOnComponent(
				sh_ptr<CompositePart> comp) const;
	};

	class Operation9 : public Query1 {
		public:
			Operation9(DataHolder *dh) : Query1(OPERATION, "OP9", dh) {
			}

		protected:
			virtual void performOperationOnAtomicPart(
				sh_ptr<AtomicPart> apart) const;
	};

	class Operation10 : public Query2 {
		public:
			Operation10(DataHolder *dh) : Query2(dh, OPERATION, "OP10", 1) {
			}

		protected:
			virtual void performOperationOnAtomicPart(
				sh_ptr<AtomicPart> apart) const;
	};

	class Operation11 : public Traversal8 {
		public:
			Operation11(DataHolder *dh) : Traversal8(OPERATION, "OP11", dh) {
			}

		protected:
			virtual int traverse(sh_ptr<Manual> manual) const;
	};

	class Operation12 : public Operation6 {
		protected:
			Operation12(optype t, const char *n, DataHolder *dh)
				: Operation6(t, n, dh) {
			}

		public:
			Operation12(DataHolder *dh) : Operation6(OPERATION, "OP12", dh) {
			}

		protected:
			virtual void performOperationOnComplexAssembly(
				sh_ptr<ComplexAssembly> cassm) const;
	};

	class Operation13 : public Operation7 {
		public:
			Operation13(DataHolder *dh) : Operation7(OPERATION, "OP13", dh) {
			}

		protected:
			virtual void performOperationOnBaseAssembly(
				sh_ptr<BaseAssembly> bassm) const;
	};

	class Operation14 : public Operation8 {
		public:
			Operation14(DataHolder *dh) : Operation8(OPERATION, "OP14", dh) {
			}

		protected:
			virtual void performOperationOnComponent(
				sh_ptr<CompositePart> comp) const;
	};

	class Operation15 : public Query1 {
		public:
			Operation15(DataHolder *dh) : Query1(OPERATION, "OP15", dh) {
			}

		protected:
			virtual void performOperationOnAtomicPart(
				sh_ptr<AtomicPart> apart) const;
	};

}

#endif // SB7_OPERATION_OPS_H_
