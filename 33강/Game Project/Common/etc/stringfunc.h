#pragma once


namespace common
{
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
	string& trim(string &str);

	std::string wstr2str(const std::wstring &wstr);
	std::wstring str2wstr(const std::string &str);

	std::string format(const char* fmt, ...);
	std::wstring formatw(const char* fmt, ...);

}
