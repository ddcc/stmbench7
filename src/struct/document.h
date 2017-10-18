#ifndef SB7_DOCUMENT_H_
#define SB7_DOCUMENT_H_

// TODO inherit this class and Manual from the same base class

#include "../tm/tm_ptr.h"

#include "../string.h"

namespace sb7 {

	class CompositePart;

	class Document : public Object<Document> {
		public:
		    Document(int id, string title, string text)
		    	: m_id(id), m_title(title), m_text(text) { }

			Document(const Document &doc)
				: m_id(doc.m_id), m_title(doc.m_title),
				m_text(doc.m_text), m_part(doc.m_part) {
			}

			virtual Document* clone() const {
				return new Document(*this);
			}

			virtual ~Document() {
			}

			void setPart(sh_ptr<CompositePart> cpart) {
				m_part = cpart;
			}

			sh_ptr<CompositePart> getCompositePart() const {
				return m_part;
			}

			int getDocumentId() const {
				return m_id;
			}

			string getTitle() const {
				return m_title;
			}

			void nullOperation() const {
			}

			int searchText(char ch) const;

			bool replaceText(const string &from, const string &to);

			bool textBeginsWith(const string &prefix) const {
				return (m_text.find(prefix) == 0);
			}

			string getText() const {
				return m_text;
			}

		private:
			int m_id;
			string m_title;
			string m_text;
			sh_ptr<CompositePart> m_part;
	};

}

#endif /*SB7_DOCUMENT_H_*/
