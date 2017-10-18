#ifndef SB7_SHORT_TRAVERSAL_OPS_H_
#define SB7_SHORT_TRAVERSAL_OPS_H_

#include "../tm/tm_ptr.h"

#include "operations.h"
#include "traversal_ops.h"

#include "../struct/assembly.h"

namespace sb7 {

	class ShortTraversal1 : public Operation {
		protected:
			ShortTraversal1(enum optype type, const char *name,
					DataHolder *dh)
				: Operation(type, name, dh) {
			}

		public:
			ShortTraversal1(DataHolder *dh)
				: Operation(SHORT_TRAVERSAL_RO, "ST1", dh) {
			}

			virtual int run() const;

		protected:
			int traverse(sh_ptr<Assembly> assm) const;
			int traverse(sh_ptr<ComplexAssembly> cassm) const;
			int traverse(sh_ptr<BaseAssembly> bassm) const;
			virtual int traverse(sh_ptr<CompositePart> cpart) const;
			virtual int traverse(sh_ptr<AtomicPart> apart) const;
	};

	class ShortTraversal2 : public ShortTraversal1 {
		protected:
			ShortTraversal2(enum optype type, const char *name,
					DataHolder *dh)
				: ShortTraversal1(type, name, dh) {
			}

		public:
			ShortTraversal2(DataHolder *dh) :
				ShortTraversal1(SHORT_TRAVERSAL_RO, "ST2", dh) {
			}

		protected:
			virtual int traverse(sh_ptr<CompositePart> cpart) const;
			virtual int traverse(sh_ptr<AtomicPart> apart) const;
			virtual int traverse(sh_ptr<Document> documentation) const;
	};

	class ShortTraversal6 : public ShortTraversal1 {
		public:
			ShortTraversal6(DataHolder *dh) :
				ShortTraversal1(SHORT_TRAVERSAL, "ST6", dh) {
			}

		protected:
			virtual int traverse(sh_ptr<AtomicPart> apart) const;
	};

	class ShortTraversal7 : public ShortTraversal2 {
		public:
			ShortTraversal7(DataHolder *dh) :
				ShortTraversal2(SHORT_TRAVERSAL, "ST7", dh) {
			}

		protected:
			virtual int traverse(sh_ptr<Document> documentation) const;
	};

	class ShortTraversal8 : public Traversal7 {
		public:
			ShortTraversal8(DataHolder *dh) :
				Traversal7(SHORT_TRAVERSAL, "ST8", dh) {
			}

		protected:
			virtual void performOperationOnAssembly(
				sh_ptr<Assembly> assembly) const;
	};

	class ShortTraversal9 : public ShortTraversal1 {
		protected:
			ShortTraversal9(enum optype type, const char *name,
					DataHolder *dh)
				: ShortTraversal1(type, name, dh) {
			}

		public:
			ShortTraversal9(DataHolder *dh) :
				ShortTraversal1(SHORT_TRAVERSAL_RO, "ST9", dh) {
			}

		protected:
			virtual int traverse(sh_ptr<CompositePart> cpart) const;
			virtual int traverse(sh_ptr<AtomicPart> apart) const;
			int traverse(sh_ptr<AtomicPart> apart,
				Set<sh_ptr<AtomicPart > > &visitedParts) const;
			virtual int performOperationOnAtomicPart(
				sh_ptr<AtomicPart> apart) const;
	};

	class ShortTraversal10 : public ShortTraversal9 {
		public:
			ShortTraversal10(DataHolder *dh) :
				ShortTraversal9(SHORT_TRAVERSAL, "ST10", dh) {
			}

			virtual int performOperationOnAtomicPart(
				sh_ptr<AtomicPart> apart) const;
	};
}

#endif // SB7_SHORT_TRAVERSAL_OPS_H_
