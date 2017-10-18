#ifndef SB7_DESIGN_OBJ_H_
#define SB7_DESIGN_OBJ_H_

#include "../tm/tm_ptr.h"
#include "../string.h"

namespace sb7 {
	class DesignObj : public Object<DesignObj> {
		public:
			DesignObj(int id, string type, int buildDate)
				: m_id(id), m_type(type), m_buildDate(buildDate) { }

			virtual ~DesignObj() { }

			virtual DesignObj *clone() const {
				return new DesignObj(m_id, m_type, m_buildDate);
			}

			int getId() const {
				return m_id;
			}

			int getBuildDate() const {
				return m_buildDate;
			}

			void updateBuildDate();

			void nullOperation() const { }

		protected:
			int m_id;
			string m_type;
			int m_buildDate;
	};
}

#endif /*SB7_DESIGN_OBJ_H_*/
