#include "locale_encoding.h"

#include "win_codepages.h"
#ifndef NOMINMAX
	#define NOMINMAX
#endif
#include <windows.h>

#include "../vtl/utility.h"

#define _VTL_ASCII_IS_UPPER(c) ('A' <= c && c <= 'Z')
#define _VTL_ASCII_IS_LOWER(c) ('a' <= c && c <= 'z')
#define _VTL_ASCII_IS_NUMERIC(c) ('0' <= c && c <= '9')

_VTL_START

std::string normalize_encoding(const std::string_view encoding)
{
    std::string result;
    result.reserve(encoding.length());
    for (char c : encoding) 
    {
        if (_VTL_ASCII_IS_LOWER(c) || _VTL_ASCII_IS_NUMERIC(c))
            result += c;
        else if (_VTL_ASCII_IS_UPPER(c))
            result += char(c - 'A' + 'a');
    }
    return result;
}

static int normalized_encoding_to_windows_codepage(const std::string& encoding)
{
    windows_encoding* end = std::end(all_windows_encodings); // todo fixme

    windows_encoding* ptr = std::lower_bound(all_windows_encodings, end, encoding.c_str()); // todo fixme
    while (ptr != end && ptr->name == encoding) 
    {
        if (ptr->was_tested)
            return ptr->codepage;
        else if (IsValidCodePage(ptr->codepage)) 
        {
            ptr->was_tested = 1;
            return ptr->codepage;
        }
        else
            ++ptr;
    }
    return -1;
}

int encoding_to_windows_codepage(const std::string_view encoding)
{
    return normalized_encoding_to_windows_codepage(normalize_encoding(encoding));
}

_VTL_END

#undef _VTL_ASCII_IS_UPPER
#undef _VTL_ASCII_IS_LOWER
#undef _VTL_ASCII_IS_NUMERIC