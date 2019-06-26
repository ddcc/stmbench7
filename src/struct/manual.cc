#include "manual.h"
#include "../string.h"

namespace sb7 {
	
	int Manual::countOccurences(char c) const {
		string::const_iterator curr;
		string::const_iterator end = m_text.end();
		int cnt = 0;

		for(curr = m_text.begin();curr != end;curr++) {
			if(*curr == c) {
				cnt++;
			}
		}

		return cnt;
	}

	bool Manual::checkFirstLastCharTheSame() const {
		return *(m_text.begin()) == *(m_text.rbegin());
	}

	bool Manual::startsWith(char c) const {
		return *(m_text.begin()) == c;
	}

	int Manual::replaceChar(char from, char to) {
		string::iterator curr;
		string::iterator end = m_text.end();
		int cnt = 0;

		for(curr = m_text.begin();curr != end;curr++) {
			char c = *curr;

			if(c == from) {
				*curr = to;
				cnt++;
			}
		}

		return cnt;
	}

#ifndef ORIGINAL
	bool Manual::isCompatible(const Manual &o) const {
		return m_id == o.m_id && m_title == o.m_title && m_text.size() == o.m_text.size() && m_module == o.m_module;
	}

	const string& Manual::getText() const {
		return m_text;
	}

	string&& Manual::getText() {
		return std::move(m_text);
	}

	void Manual::assignText(string &&s) {
		m_text.assign(s);
	}
#endif /* ORIGINAL */
}
