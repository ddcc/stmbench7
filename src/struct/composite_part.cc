#include "atomic_part.h"
#include "composite_part.h"
#include "document.h"

using namespace sb7;

bool sb7::CompositePart::addPart(sh_ptr<AtomicPart> apart) {
	wr_ptr<atomic_part_set> wr_parts(m_parts);
	bool add = wr_parts->add(apart);

	if(add) {
		sh_ptr<CompositePart> sh_this((CompositePart *)this);
		wr_ptr<AtomicPart> wr_apart(apart);
		wr_apart->setCompositePart(sh_this);

		if(m_rootPart == NULL) {
			m_rootPart = apart;
		}
	}

	return add;
}
