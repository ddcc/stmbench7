#ifndef SB7_TRAVERSAL_OPS_
#define SB7_TRAVERSAL_OPS_

#include "../tm/tm_ptr.h"

#include "operations.h"
#include "../containers.h"
#include "../struct/assembly.h"

namespace sb7 {

	class Traversal1 : public Operation {
		protected:
			Traversal1(optype t, const char *n, DataHolder *dh)
				: Operation(t, n, dh) {
			}

		public:
			Traversal1(DataHolder *dh) : Operation(TRAVERSAL_RO, "T1", dh) {
			}

			virtual int run() const;

		protected:
			int traverse(sh_ptr<ComplexAssembly> complexAssembly) const;
			int traverse(sh_ptr<BaseAssembly> baseAssembly) const;
			virtual int traverse(sh_ptr<CompositePart> component) const;
			virtual int traverse(sh_ptr<AtomicPart> part,
				Set<sh_ptr<AtomicPart> > &setOfVisitedParts) const;
			virtual int performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
				Set<sh_ptr<AtomicPart> > &setOfVisitedPartIds) const;
	};

	class Traversal2a : public Traversal1 {
		public:
			Traversal2a(DataHolder *dh) : Traversal1(TRAVERSAL, "T2a", dh) {
			}

		protected:
			virtual int performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
				Set<sh_ptr<AtomicPart> > &setOfVisitedPartIds) const;			
	};

	class Traversal2b : public Traversal1 {
		public:
			Traversal2b(DataHolder *dh) : Traversal1(TRAVERSAL, "T2b", dh) {
			}

		protected:
			virtual int performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
				Set<sh_ptr<AtomicPart> > &setOfVisitedPartIds) const;			
	};

	class Traversal2c : public Traversal1 {
		public:
			Traversal2c(DataHolder *dh) : Traversal1(TRAVERSAL, "T2c", dh) {
			}

		protected:
			virtual int performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
				Set<sh_ptr<AtomicPart> > &setOfVisitedPartIds) const;			
	};

	class Traversal3a : public Traversal1 {
		protected:
			Traversal3a(optype t, const char *n, DataHolder *dh)
				: Traversal1(t, n, dh) {
			}

		public:
			Traversal3a(DataHolder *dh) : Traversal1(TRAVERSAL, "T3a", dh) {
			}

		protected:
			virtual int performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
				Set<sh_ptr<AtomicPart> > &setOfVisitedPartIds) const;

			void updateBuildDate(sh_ptr<AtomicPart> apart) const;
	};

	class Traversal3b : public Traversal3a {
		public:
			Traversal3b(DataHolder *dh) : Traversal3a(TRAVERSAL, "T3b", dh) {
			}

		protected:
			virtual int performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
				Set<sh_ptr<AtomicPart> > &setOfVisitedPartIds) const;
	};

	class Traversal3c : public Traversal3a {
		public:
			Traversal3c(DataHolder *dh) : Traversal3a(TRAVERSAL, "T3c", dh) {
			}

		protected:
			virtual int performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
				Set<sh_ptr<AtomicPart> > &setOfVisitedPartIds) const;
	};

	class Traversal4 : public Traversal1 {
		protected:
			Traversal4(optype t, const char *n, DataHolder *dh)
				: Traversal1(t, n, dh) {
			}

		public:
			Traversal4(DataHolder *dh) : Traversal1(TRAVERSAL_RO, "T4", dh) {
			}

		protected:
			virtual int traverse(sh_ptr<CompositePart> component) const;
			virtual int traverse(sh_ptr<Document> doc) const;
			virtual int traverse(sh_ptr<AtomicPart> part,
				Set<sh_ptr<AtomicPart> > &setOfVisitedParts) const;
			virtual int performOperationOnAtomicPart(sh_ptr<AtomicPart> apart,
				Set<sh_ptr<AtomicPart> > &setOfVisitedPartIds) const;
	};

	class Traversal5 : public Traversal4 {
		public:
			Traversal5(DataHolder *dh) : Traversal4(TRAVERSAL, "T5", dh) {
			}

		protected:
			virtual int traverse(sh_ptr<Document> doc) const;
	};

	class Traversal6 : public Traversal1 {
		public:
			Traversal6(DataHolder *dh) : Traversal1(TRAVERSAL_RO, "T6", dh) {
			}

		protected:
			// I made this shorter than in original bench
			virtual int traverse(sh_ptr<CompositePart> component) const;
	};

	// following traversal is actually short traversal, but is here because
	// of it's slightly misleading name
	class Traversal7 : public Operation {
		protected:
			Traversal7(optype t, const char *n, DataHolder *dh)
				: Operation(t, n, dh) {
			}

		public:
			Traversal7(DataHolder *dh)
				: Operation(SHORT_TRAVERSAL_RO, "T7", dh) {
			}

			virtual int run() const;

		protected:
			int traverse(sh_ptr<CompositePart> cpart) const;
			int traverse(sh_ptr<Assembly> assembly,
				Set<sh_ptr<Assembly> > &visitedAssemblies) const;
			virtual void performOperationOnAssembly(
				sh_ptr<Assembly> assembly) const;
	};

	// following two traversals are actually ro operations, but are here
	// because of their slightly misleading names

	class Traversal8 : public Operation {
		protected:
			Traversal8(optype t, const char *n, DataHolder *dh)
				: Operation(t, n, dh) {
			}

		public:
			Traversal8(DataHolder *dh)
				: Operation(OPERATION_RO, "T8", dh) {
			}

			virtual int run() const;

		protected:
			virtual int traverse(sh_ptr<Manual> manual) const;
	};

	class Traversal9 : public Traversal8 {
		public:
			Traversal9(DataHolder *dh)
				: Traversal8(OPERATION_RO, "T9", dh) {
			}

		protected:
			virtual int traverse(sh_ptr<Manual> manual) const;
	};

}

#endif // SB7_TRAVERSAL_OPS_
