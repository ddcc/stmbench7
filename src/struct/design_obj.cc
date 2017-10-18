#include "design_obj.h"

namespace sb7 {

	void DesignObj::updateBuildDate() {	
		if(m_buildDate % 2 == 0) {
			m_buildDate--;
		} else {
			m_buildDate++;
		}
	}

}
