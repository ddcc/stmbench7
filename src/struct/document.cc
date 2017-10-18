#include "document.h"

int sb7::Document::searchText(char ch) const {
	string::const_iterator curr;
	string::const_iterator end = m_text.end();
	int cnt = 0;

	for(curr = m_text.begin();curr != end;curr++) {
		if(*curr == ch) {
			cnt++;
		}
	}

	return cnt;
}

bool sb7::Document::replaceText(const string &from, const string &to) {
	bool ret = textBeginsWith(from);

	if(ret) {
		m_text.replace(0, from.length(), to);
	}

	return ret;
}
