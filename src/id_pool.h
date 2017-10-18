#ifndef SB7_ID_POOL_H_
#define SB7_ID_POOL_H_

#include <vector>

#include "common/memory.h"
#include "tm/tm_ptr.h"

#include "sb7_exception.h"

namespace sb7 {

	class IdPool : public Object<IdPool> {
		protected:
			typedef std::vector<int, Sb7Allocator<int> > id_vector;

		public:
			IdPool(int maxId);

			IdPool(const IdPool &pool) : m_ids(pool.m_ids) { }

			virtual IdPool *clone() const {
				return new IdPool(*this);
			}

			virtual ~IdPool() { }

			int getId();

			void putId(int id);

		private:
			id_vector m_ids;
	};
}

#endif /*SB7_ID_POOL_H_*/
