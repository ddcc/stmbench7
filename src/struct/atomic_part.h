#ifndef SB7_ATOMIC_PART_H_
#define SB7_ATOMIC_PART_H_

#include "../tm/tm_ptr.h"

#include "../string.h"
#include "../containers.h"
#include "design_obj.h"

namespace sb7 {

	class Connection;
	class CompositePart;

	class AtomicPart : public DesignObj {
		protected:
			typedef Set<sh_ptr<Connection> > connection_set;
			typedef sh_ptr<connection_set> shared_connection_set;

		public:
			AtomicPart(int id, string type, int buildDate, int x, int y)
				: DesignObj(id, type, buildDate), m_x(x), m_y(y),
				m_to(new connection_set()), m_from(new connection_set()) {
			}

			AtomicPart(const AtomicPart &apart)
				: DesignObj(apart.m_id, apart.m_type, apart.m_buildDate),
				m_x(apart.m_x), m_y(apart.m_y),
				m_to(apart.m_to), m_from(apart.m_from),
				m_partOf(apart.m_partOf) {
			}

			virtual ~AtomicPart() {
			}

			virtual AtomicPart *clone() const {
				return new AtomicPart(*this);
			}

			void connectTo(sh_ptr<AtomicPart> dest,
					const string &type, int len) const;

			void addConnectionFromOtherPart(sh_ptr<Connection> conn) const {
				wr_ptr<connection_set> wr_from(m_from);
				wr_from->add(conn);
			}

			void removeConnectionFromOtherPart(sh_ptr<Connection> conn) const {
				wr_ptr<connection_set> wr_from(m_from);
				wr_from->remove(conn);
			}

			void removeConnectionToOtherPart(sh_ptr<Connection> conn) const {
				wr_ptr<connection_set> wr_to(m_to);
				wr_to->remove(conn);
			}

			/**
			 * Return number of outgoing connections.
		 	*/
			void setCompositePart(sh_ptr<CompositePart> cp) {
				m_partOf = cp;
			}

			int getNumConnections() const {
				rd_ptr<connection_set> rd_to(m_to);
				return rd_to->size();
			}

			shared_connection_set getToConnections() const {
				return m_to;
			}

			shared_connection_set getFromConnections() const {
				return m_from;
			}

			sh_ptr<CompositePart> getPartOf() const {
				return m_partOf;
			}

			void swapXY() {
				int tmp = m_y;
				m_y = m_x;
				m_x = tmp;
			}

			int getX() const {
				return m_x;
			}

			int getY() const {
				return m_y;
			}

			void freeMemory() {
				tx_delete(m_to);
				tx_delete(m_from);
			}

		private:
			int m_x, m_y;
			shared_connection_set m_to, m_from;
			sh_ptr<CompositePart> m_partOf;
	};

}

#endif /*SB7_ATOMIC_PART_H_*/
