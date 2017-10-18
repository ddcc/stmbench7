#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>

#include "helpers.h"

using namespace std;


void sb7::printSection(ostream &out, const char *text) {
	char fill = out.fill();
	out << setfill(section_fill_char) << setw(line_width) << "" << endl;
	out << text << endl;
	out << setfill(section_fill_char) << setw(line_width) << "" << endl;
	out << setfill(fill);
}

std::string sb7::boolToStr(bool val) {
	return val ? std::string("true") : std::string("false");
}

int sb7::strToBool(const std::string &str) {
	if(equalNoCase(str, "true")) {
		return 1;
	} else if(equalNoCase(str, "false")) {
		return 0;
	} else {
		return -1;
	}
}

bool sb7::equalNoCase(const std::string &s1, const std::string &s2) {
	std::string::const_iterator iter1 = s1.begin();
	std::string::const_iterator iter2 = s2.begin();

	while(iter1 != s1.end() && iter2 != s2.end()) {
		if(::toupper(*iter1) != ::toupper(*iter2)) {
			return false;
		}

		iter1++;
		iter2++;
	}

	return (iter1 == s1.end()) && (iter2 == s2.end());
}

int sb7::strToUint(const std::string &str) {
	std::string::const_iterator iter = str.begin();
	std::string buf;

	// skip whitespace
	while(iter != str.end()) {
		if(!isspace(*iter)) {
			break;
		}

		iter++;
	}

	// read digits
	while(iter != str.end()) {
		if(isdigit(*iter)) {
			buf.push_back(*iter);
			iter++;
		} else {
			break;
		}
	}

	// check to see if there is some garbage left
	while(iter != str.end()) {
		if(!isspace(*iter)) {
			return -1;
		}

		iter++;
	}

	istringstream inputStream(buf);
	int ret = -1;
	inputStream >> ret;

	return ret;
}

std::string sb7::memSizeToStr(long size) {
	std::string sizeSpec;
	long sizeFraction = 0;

	if(size < 1024) {
		sizeSpec = "B";
	} else {
		size /= 1024;

        if(size < 1024) {
        	sizeFraction = size % 1024;
			sizeSpec = "kB";
        } else {
        	sizeFraction = size % 1024;
        	size /= 1024;
        	sizeSpec = "MB";
		}
	}

	std::stringstream ret;
	ret << size;

	if(sizeFraction != 0) {
		ret << '.';
		ret << sizeFraction;
	}

	ret << sizeSpec;

	return ret.str();
}
