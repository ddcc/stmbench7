#ifndef SB7_MANUAL_H_
#define SB7_MANUAL_H_

#include "../tm/tm_ptr.h"

#include "../string.h"

namespace sb7 {

	class Module;

	class Manual : public Object<Manual> {
		public:
			Manual(int id, const string &title, string text)
				: m_id(id), m_title(title), m_text(text) { }

			virtual Manual* clone() const {
				Manual *ret = new Manual(m_id, m_title, m_text);
				ret->setModule(m_module);

				return ret;
			}

			virtual ~Manual() { }

			void setModule(sh_ptr<Module> mod) {
				m_module = mod;
			}

			/**
			 * Count number of occurencest of c in text.
			 */
			int countOccurences(char c) const;

			/**
			 * Check whether first and last character of text are equal.
			 */
			bool checkFirstLastCharTheSame() const;

			/**
			 * Check if text starts with character c.
			 */
			bool startsWith(char c) const;

			/**
			 * Replace all characters from with characters to in text.
			 */
			int replaceChar(char from, char to);

		protected:
			int m_id;
			string m_title;
			string m_text;
			sh_ptr<Module> m_module;
	};
}

#endif /*SB7_MANUAL_H_*/
