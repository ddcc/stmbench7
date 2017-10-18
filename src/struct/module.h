#ifndef SB7_MODULE_H_
#define SB7_MODULE_H_

#include "../tm/tm_ptr.h"

#include "manual.h"
#include "design_obj.h"

namespace sb7 {

	class ComplexAssembly;

	class Module : public DesignObj {
		public:
			Module(int id, string type, int buildDate,
						sh_ptr<Manual> manual)
					: DesignObj(id, type, buildDate),
					m_manual(manual) { }

			// call after constructor to finish initialization
			void connectManual() const {
				wr_ptr<Manual> manual(m_manual);
				sh_ptr<Module> sh((Module *)this);
				manual->setModule(sh);
			}

			virtual ~Module() { }

			virtual Module *clone() const {
				Module *ret = new Module(m_id, m_type, m_buildDate,
					m_manual);
				ret->setDesignRoot(m_designRoot);
				return ret;
			}

			void setDesignRoot(sh_ptr<ComplexAssembly> designRoot) {
				m_designRoot = designRoot;
			}

			sh_ptr<ComplexAssembly> getDesignRoot() const {
				return m_designRoot;
			}

			sh_ptr<Manual> getManual() const {
				return m_manual;
			}


		private:
			sh_ptr<Manual> m_manual;
			sh_ptr<ComplexAssembly> m_designRoot;
	};
}

#endif /*SB7_MODULE_H_*/
