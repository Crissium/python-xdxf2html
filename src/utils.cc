#include "xdxf2html.h"

#include <algorithm>
#include <cctype>

void ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
									{ return !std::isspace(ch); }));
}

void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
						 { return !std::isspace(ch); })
				.base(),
			s.end());
}

void trim(std::string &s)
{
	rtrim(s);
	ltrim(s);
}

bool endswith(const std::string &str, const std::string &suffix)
{
	return str.size() >= suffix.size() &&
		   str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}