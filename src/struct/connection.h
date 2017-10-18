#ifndef SB7_CONNECTION_H_
#define SB7_CONNECTION_H_

#include "../tm/tm_ptr.h"

#include "../string.h"

namespace sb7 {

	class AtomicPart;

	class Connection : public Object<Connection> {
		public:
			Connection(sh_ptr<AtomicPart> from,
					sh_ptr<AtomicPart> to,
					const string &type, int length)
				: m_type(type), m_length(length), m_from(from), m_to(to) {
			}

			Connection(const Connection &conn)
				: m_type(conn.m_type), m_length(conn.m_length),
				m_from(conn.m_from), m_to(conn.m_to) { }

			virtual ~Connection() { }

			virtual Connection *clone() const {
				return new Connection(*this);
			}

			Connection *getReversed() const {
				return new Connection(m_to, m_from, m_type, m_length);
			}

			sh_ptr<AtomicPart> getDestination() const {
				return m_to;
			}

			sh_ptr<AtomicPart> getSource() const {
				return m_from;
			}

		private:
			string m_type;
			int m_length;
			sh_ptr<AtomicPart> m_from;
			sh_ptr<AtomicPart> m_to;
	};
}

#endif /*SB7_CONNECTION_H_*/
