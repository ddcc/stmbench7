#ifndef SB7_COMPOSITE_PART_H_
#define SB7_COMPOSITE_PART_H_

#include "../tm/tm_ptr.h"

#include "../string.h"
#include "../containers.h"
#include "design_obj.h"
#include "assembly.h"
#include "document.h"

using namespace sb7;

namespace sb7 {

	class Document;
	class AtomicPart;

	class CompositePart : public DesignObj {
		protected:
			typedef Bag<sh_ptr<BaseAssembly> > base_assembly_bag;
			typedef sh_ptr<base_assembly_bag> shared_base_assembly_bag;
			typedef Set<sh_ptr<AtomicPart> > atomic_part_set;
			typedef sh_ptr<atomic_part_set> shared_atomic_part_set;

		public:
			CompositePart(int id, string type, int buildDate,
						sh_ptr<Document> doc)
					: DesignObj(id, type, buildDate), m_doc(doc),
					m_usedIn(new base_assembly_bag()),
					m_parts(new atomic_part_set()) {
			}

			CompositePart(const CompositePart &part)
					: DesignObj(part.m_id, part.m_type, part.m_buildDate),
					m_doc(part.m_doc), m_usedIn(part.m_usedIn),
					m_parts(part.m_parts), m_rootPart(part.m_rootPart) {
			}

			virtual ~CompositePart() {
			}

			virtual CompositePart *clone() const {
				return new CompositePart(*this);
			}

			void addAssembly(sh_ptr<BaseAssembly> assembly) const {
				wr_ptr<base_assembly_bag> wr_usedIn(m_usedIn);
				wr_usedIn->add(assembly);
			}

			void removeAssembly(sh_ptr<BaseAssembly> assembly) const {
				wr_ptr<base_assembly_bag> wr_usedIn(m_usedIn);
				wr_usedIn->remove(assembly);
			}

			bool addPart(sh_ptr<AtomicPart> part);

			sh_ptr<AtomicPart> getRootPart() const {
				return m_rootPart;
			}

			sh_ptr<Document> getDocumentation() const {
				return m_doc;
			}

			shared_atomic_part_set getParts() const {
				return m_parts;
			}

			shared_base_assembly_bag getUsedIn() const {
				return m_usedIn;
			}

			void freeMemory() {
				tx_delete(m_usedIn);
				tx_delete(m_parts);
			}

		private:
			sh_ptr<Document> m_doc;
			shared_base_assembly_bag m_usedIn;
			shared_atomic_part_set m_parts;
			sh_ptr<AtomicPart> m_rootPart;
	};
}

#endif /*SB7_COMPOSITE_PART_H_*/
