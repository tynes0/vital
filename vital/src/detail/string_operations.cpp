#include "../vtl/string_operations.h"

_VTL_START

// --------------------- STRING OPERATIONS ---------------------

bool string_operations::starts_with(const std::string& str, char token)
{
	if (str.size() == 0) 
		return false;
	return (str[0] == token);
}

bool string_operations::starts_with(const std::string& str, const std::string& token)
{
	if (str.size() < token.size())
		return false;
	if (token.empty())
		return true;
	for (size_t i = 0; i < token.size(); ++i)
		if (str[i] != token[i])
			return false;
	return true;
}

bool string_operations::ends_with(const std::string& str, char token)
{
	if (str.size() == 0)
		return false;
	return (str[str.size() - 1] == token);
}

bool string_operations::ends_with(const std::string& str, const std::string& token)
{
	if (str.size() < token.size())
		return false;
	if (token.empty())
		return true;
	for (size_t i = 0; i < token.size(); ++i)
		if (str[str.size() - i] != token[token.size() - i])
			return false;
	return true;
}

std::string string_operations::trim_left(const std::string& str)
{
	std::string result(str);
	size_t i = result.find_first_not_of(' ');
	if (i != std::string::npos)
		result.erase(result.begin(), result.begin() + i);
	return result;
}

size_t string_operations::trim_left_directly(std::string& str)
{
	size_t i = str.find_first_not_of(' ');
	if (i != std::string::npos)
		str.erase(str.begin(), str.begin() + i);
	return str.size();
}

std::string string_operations::trim_right(const std::string& str)
{
	std::string result(str);
	size_t i = result.find_last_not_of(' ');
	if (i != std::string::npos)
		result.erase(result.begin() + i + 1, result.end());
	return result;
}

size_t string_operations::trim_right_directly(std::string& str)
{
	size_t i = str.find_last_not_of(' ');
	if (i != std::string::npos)
		str.erase(str.begin() + i + 1, str.end());
	return str.size();
}

std::string string_operations::trim(const std::string& str)
{
	return trim_left(trim_right(str));
}

size_t string_operations::trim_directly(std::string& str)
{
	trim_left_directly(str);
	return trim_right_directly(str);
}

std::string string_operations::chop(std::string& str, size_t offset)
{
	std::string result = "";
	if (offset >= str.size())
		return result;
	result.resize(offset);
	std::copy(str.begin(), str.begin() + offset, &result[0]);
	str.erase(str.begin(), str.begin() + offset);
	return result;
}

std::string string_operations::chop_but_left(std::string& str, size_t offset)
{
	std::string result = "";
	if (offset >= str.size())
		return result;
	result.resize(str.size() - offset);
	std::copy(str.begin() + offset, str.end(), &result[0]);
	str.erase(str.begin() + offset, str.end());
	return result;
}

std::string string_operations::reverse(const std::string& str)
{
	const char* fp = str.data();
	const char* ep = str.data() + str.size() - 1;
	std::string result;
	result.resize(str.size());
	size_t i = 0;
	while (ep >= fp)
	{
		result[i++] = DREF(ep--);
		result[str.size() - i] = DREF(fp++);
	}
	return result;
}

std::string& string_operations::reverse_directly(std::string& str)
{
#if	_VTL_HAS_CPP_VERSION(17)
	char* fp = str.data();
	char* ep = str.data() + str.size() - 1;
#else // _VTL_HAS_CPP_VERSION(17)
	char* fp = &str[0];
	char* ep = &str[str.size() - 1];
#endif // _VTL_HAS_CPP_VERSION(17)
	while (ep > fp)
	{
		char temp = DREF(fp);
		DREF(fp++) = DREF(ep);
		DREF(ep--) = temp;
	}
	return str;
}

std::string string_operations::reverse_in_place(const std::string& str, size_t offset, size_t length)
{
	if (offset + length > str.size())
		return str;
	const char* fp = str.data() + offset;
	const char* ep = str.data() + offset + length - 1;
	std::string result;
	result.resize(str.size());
	std::copy(str.begin(), str.begin() + offset, &result[0]);
	size_t i = offset;
	size_t j = offset + length - 1;
	while (fp <= ep)
	{
		result[i++] = DREF(ep--);
		result[j--] = DREF(fp++);
	}
	std::copy(str.begin() + offset + length, str.end(), &result[offset + length]);

	return result;
}

std::string& string_operations::reverse_in_place_directly(std::string& str, size_t offset, size_t length)
{
	if (offset + length > str.size())
		return str;
#if	_VTL_HAS_CPP_VERSION(17)
	char* fp = str.data();
	char* ep = str.data() + str.size() - 1;
#else // _VTL_HAS_CPP_VERSION(17)
	char* fp = &str[0];
	char* ep = &str[str.size() - 1];
#endif // _VTL_HAS_CPP_VERSION(17)
	size_t i = offset;
	size_t j = offset + length - 1;
	while (fp < ep)
	{
		char temp = DREF(fp);
		DREF(fp++) = DREF(ep);
		DREF(ep--) = temp;
	}
	return str;
}

std::string string_operations::remove_prefix(const std::string& str, size_t length)
{
	std::string result = "";
	if (str.size() <= length)
		return result;
	result.resize(str.size() - length);
#if _VTL_HAS_CPP_VERSION(17)
	std::copy(str.begin() + length, str.end(), result.data());
#else // _VTL_HAS_CPP_VERSION(17)
	::memcpy(&result[0], &DREF(str.begin() + length), (str.size() - length));
#endif // _VTL_HAS_CPP_VERSION(17)
	return result;
}

std::string& string_operations::remove_prefix_directly(std::string& str, size_t length)
{
	if (str.size() <= length)
		return str;
	str.erase(str.begin(), str.begin() + length);
	return str;
}

std::string string_operations::remove_suffix(const std::string& str, size_t length)
{
	std::string result = "";
	if (str.size() <= length)
		return result;
	result.resize(str.size() - length);
#if _VTL_HAS_CPP_VERSION(17)
	std::copy(str.begin(), str.end() - length, result.data());
#else // _VTL_HAS_CPP_VERSION(17)
	::memcpy(&result[0], &DREF(str.begin()), (str.size() - length));
#endif // _VTL_HAS_CPP_VERSION(17)
	return result;
}

std::string& string_operations::remove_suffix_directly(std::string& str, size_t length)
{
	if (str.size() <= length)
		return str;
	str.erase(str.end() - length, str.end());
	return str;
}

std::string string_operations::to_lower(const std::string& str)
{
	std::string result = str;
	for (char& item : result)
		item = tolower(item);
	return result;
}

std::string& string_operations::to_lower_directly(std::string& str)
{
	for (char& item : str)
		item = tolower(item);
	return str;
}

std::string string_operations::to_lower_in_place(const std::string& str, size_t offset, size_t length)
{
	std::string result = str;
	if (str.size() < offset + length)
		return result;
	for (auto it = result.begin() + offset; it != result.begin() + offset + length; ++it)
		DREF(it) = tolower(DREF(it));
	return result;
}

std::string& string_operations::to_lower_in_place_directly(std::string& str, size_t offset, size_t length)
{
	if (str.size() < offset + length)
		return str;
	for (auto it = str.begin() + offset; it != str.begin() + offset + length; ++it)
		DREF(it) = tolower(DREF(it));
	return str;
}

std::string string_operations::to_upper(const std::string& str)
{
	std::string result = str;
	for (char& item : result)
		item = toupper(item);
	return result;
}

std::string& string_operations::to_upper_directly(std::string& str)
{
	for (char& item : str)
		item = toupper(item);
	return str;
}

std::string string_operations::to_upper_in_place(const std::string& str, size_t offset, size_t length)
{
	std::string result = str;
	if (str.size() < offset + length)
		return result;
	for (auto it = result.begin() + offset; it != result.begin() + offset + length; ++it)
		DREF(it) = toupper(DREF(it));
	return result;
}

std::string& string_operations::to_upper_in_place_directly(std::string& str, size_t offset, size_t length)
{
	if (str.size() < offset + length)
		return str;
	for (auto it = str.begin() + offset; it != str.begin() + offset + length; ++it)
		DREF(it) = toupper(DREF(it));
	return str;
}

std::string string_operations::filled_copy(const std::string& str, char ch)
{
	std::string result;
	result.resize(str.size());
	vtl::fill(result.begin(), result.end(), ch);
	return result;
}

std::string& string_operations::fill(std::string& str, char ch)
{
	vtl::fill(str.begin(), str.end(), ch);
	return str;
}

std::string string_operations::fill_in_place_copy(const std::string& str, char ch, size_t offset, size_t length)
{
	std::string result = str;
	if (str.size() < offset + length)
		return result;
	vtl::fill(result.begin() + offset, result.begin() + offset + length, ch);
	return result;
}

std::string& string_operations::fill_in_place(std::string& str, char ch, size_t offset, size_t length)
{
	if (str.size() < offset + length)
		return str;
	vtl::fill(str.begin() + offset, str.begin() + offset + length, ch);
	return str;
}

bool string_operations::is_alnum(const std::string& str)
{
	size_t i = 0;
	for (; i < str.size() && isalnum(str[i]); ++i);
	if (i == str.size())
		return true;
	return false;
}

bool string_operations::is_alpha(const std::string& str)
{
	size_t i = 0;
	for (; i < str.size() && isalpha(str[i]); ++i);
	if (i == str.size())
		return true;
	return false;
}

bool string_operations::is_digit(const std::string& str)
{
	size_t i = 0;
	for (; i < str.size() && isdigit(str[i]); ++i);
	if (i == str.size())
		return true;
	return false;
}

bool string_operations::is_space(const std::string& str)
{
	size_t i = 0;
	for (; i < str.size() && isspace(str[i]); ++i);
	if (i == str.size())
		return true;
	return false;
}

bool string_operations::is_lower(const std::string& str)
{
	size_t i = 0;
	for (; i < str.size() && islower(str[i]); ++i);
	if (i == str.size())
		return true;
	return false;
}

bool string_operations::is_upper(const std::string& str)
{
	size_t i = 0;
	for (; i < str.size() && isupper(str[i]); ++i);
	if (i == str.size())
		return true;
	return false;
}

bool string_operations::is_printable(const std::string& str)
{
	size_t i = 0;
	for (; i < str.size() && isprint(str[i]); ++i);
	if (i == str.size())
		return true;
	return false;
}

vector<std::string> string_operations::separate_string(const std::string& source, const std::string& token)
{
	vector<std::string> result;
	size_t pos = 0;
	size_t last_location = 0;
	while (true)
	{
		pos = source.find(token, last_location);
		if (pos == std::string::npos)
		{
			pos = source.size();
			break;
		}
		result.push_back(source.substr(last_location, pos - last_location));
		last_location = pos + token.length();
	}
	return result;
}

vector<std::string> string_operations::separate_string(const std::string& source, char token)
{
	std::string temp = "";
	vector<std::string> result;

	for (size_t i = 0; i < source.size(); i++)
	{
		if (source[i] != token)
		{
			temp += source[i];
		}
		else
		{
			result.push_back(temp);
			temp = "";
		}
	}
	if(!temp.empty())
		result.push_back(temp);

	return result;
}

vector<std::wstring> string_operations::separate_string(const std::wstring& source, const std::wstring& token)
{
	vector<std::wstring> result;
	size_t pos = 0;
	size_t last_location = 0;
	while (true)
	{
		pos = source.find(token, last_location);
		if (pos == std::string::npos)
		{
			pos = source.size();
			break;
		}
		std::wstring temp = source.substr(last_location, pos - last_location);
		if(!temp.empty())
			result.push_back(temp);
		last_location = pos + token.length();
	}
	return result;
}

vector<std::wstring> string_operations::separate_string(const std::wstring& source, wchar_t token)
{
	std::wstring temp;
	vector<std::wstring> result;

	for (size_t i = 0; i < source.size(); i++)
	{
		if (source[i] != token)
		{
			temp += source[i];
		}
		else
		{
			result.push_back(temp);
			temp = _VTL_WIDE("");
		}
	}
	if (!temp.empty())
		result.push_back(temp);

	return result;
}

vector<std::string> string_operations::separate_string(const std::string& source, char token, char other_token)
{
	std::string temp = "";
	vector<std::string> result;
	bool in_token_range = false;

	for (size_t i = 0; i < source.size(); i++)
	{
		if (source[i] != token)
		{
			temp += source[i];
			if (source[i] == other_token)
				in_token_range = !in_token_range;
		}
		else
		{
			if (in_token_range)
			{
				temp += source[i];
				continue;
			}
			result.push_back(temp);
			temp = "";
		}
	}
	if (!temp.empty())
		result.push_back(temp);

	return result;
}

_VTL_END