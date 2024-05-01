#pragma once
#ifndef _VITAL_LOCALE_ENCODING_
#define _VITAL_LOCALE_ENCODING_

#include "../vtl/core.h"
#include "../vtl/type_traits.h"
#include "../vtl/vector.h"

#include <xstring>

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

template<typename _ChTy
>
const char* utf_name()
{
    constexpr auto char_size = sizeof(_ChTy);
    static_assert(char_size == 1 || char_size == 2 || char_size == 4, "Unknown Character Encoding");
    switch (char_size)
    {
    case 1: return "UTF-8";
    case 2: 
    {
        const int16_t endian_mark = 1;
        const bool is_little_endian = reinterpret_cast<const char*>(&endian_mark)[0] == 1;
        return is_little_endian ? "UTF-16LE" : "UTF-16BE";
    }
    case 4: 
    {
        const int32_t endian_mark = 1;
        const bool is_little_endian = reinterpret_cast<const char*>(&endian_mark)[0] == 1;
        return is_little_endian ? "UTF-32LE" : "UTF-32BE";
    }
    }
    return "Unknown UTF";
}

#if _VTL_TEST_CPP_FT(char8_t)
template<typename T>
struct is_char8_t : is_same<T, char8_t> {};
#else
template<typename T>
struct is_char8_t : false_type {};
#endif

std::string normalize_encoding(std::string_view encoding);

inline bool are_encodings_equal(const std::string& l, const std::string& r)
{
    return normalize_encoding(l) == normalize_encoding(r);
}

vector<std::string> get_simple_encodings();

int encoding_to_windows_codepage(std::string_view encoding);

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_LOCALE_ENCODING_