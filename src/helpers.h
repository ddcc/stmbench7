#ifndef SB7_HELPERS_H_
#define SB7_HELPERS_H_

#include <iostream>

#define MAX_INT_LEN 65
#define HUNDRED_PERCENT 100

#define ITOA(itoa_buf, i)						\
	char itoa_buf[MAX_INT_LEN];					\
	snprintf(itoa_buf, MAX_INT_LEN, "%d", i);

#define MAX(a, b) ((a) < (b)) ? (b) : (a)
#define MIN(a, b) ((a) > (b)) ? (b) : (a)

namespace sb7 {

	void printSection(std::ostream &out, const char *text);

	std::string boolToStr(bool val);
	int strToBool(const std::string &str);
	bool equalNoCase(const std::string &s1, const std::string &s2);

	int strToUint(const std::string &str);

	std::string memSizeToStr(long size);

	const int line_width = 78;
	const char section_fill_char = '-';
}

#endif // SB7_HELPERS_H_
