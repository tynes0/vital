#pragma once
#ifndef _VITAL_WIN_CCONV_
#define _VITAL_WIN_CCONV_

#include "../vtl/core.h"
#include "../vtl/vector.h"
#include "../vtl/locale.h"
#include "../vtl/algorithms.h"

#include <xutility>

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <Windows.h>


#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

void multibyte_to_wide_one_by_one(int codepage, const char* begin, const char* end, vector<wchar_t>& buf)
{
	buf.reserve(end - begin);
    DWORD flags = MB_ERR_INVALID_CHARS;
    while (begin != end)
    {
        wchar_t wide_buf[4];
        int n = 0;
        int len = IsDBCSLeadByteEx(codepage, *begin) ? 2 : 1;
        if (len == 2 && begin + 1 == end)
            return;
        n = MultiByteToWideChar(codepage, flags, begin, len, wide_buf, 4);
        if (n == 0 && flags != 0 && GetLastError() == ERROR_INVALID_FLAGS)
        {
            flags = 0;
            n = MultiByteToWideChar(codepage, flags, begin, len, wide_buf, 4);
        }
        for (int i = 0; i < n; i++)
            buf.push_back(wide_buf[i]);
        begin += len;
    }
}

void multibyte_to_wide(int codepage, const char* begin, const char* end, bool do_skip, vector<wchar_t>& buf)
{
    if (begin == end)
        return;
    const std::ptrdiff_t num_chars = end - begin;
    if (num_chars > std::numeric_limits<int>::max())
        throw conversion_error();
    DWORD flags = MB_ERR_INVALID_CHARS;
    int n = MultiByteToWideChar(codepage, flags, begin, static_cast<int>(num_chars), 0, 0);
    if (n == 0 && GetLastError() == ERROR_INVALID_FLAGS) 
    {
        flags = 0;
        n = MultiByteToWideChar(codepage, flags, begin, static_cast<int>(num_chars), 0, 0);
    }

    if (n == 0)
    {
        if (do_skip) 
        {
            multibyte_to_wide_one_by_one(codepage, begin, end, buf);
            return;
        }
        throw conversion_error();
    }

    buf.resize(n);
    if (MultiByteToWideChar(codepage, flags, begin, static_cast<int>(num_chars), buf.data(), n) == 0)
        throw conversion_error();
}

void wide_to_multibyte_non_zero(int codepage, const wchar_t* begin, const wchar_t* end, bool do_skip, vector<char>& buf)
{
    buf.clear();
    if (begin == end)
        return;
    BOOL substitute = FALSE;
    BOOL* substitute_ptr = (codepage == CP_UTF7 || codepage == CP_UTF8) ? nullptr : &substitute;
    char subst_char = 0;
    char* subst_char_ptr = (codepage == CP_UTF7 || codepage == CP_UTF8) ? nullptr : &subst_char;

    if ((end - begin) > std::numeric_limits<int>::max())
        throw conversion_error();
    const int num_chars = static_cast<int>(end - begin);
    int n = WideCharToMultiByte(codepage, 0, begin, num_chars, nullptr, 0, subst_char_ptr, substitute_ptr);
    if (n == 0 && GetLastError() == ERROR_INVALID_PARAMETER) 
    {
        subst_char_ptr = nullptr;
        substitute_ptr = nullptr;
        n = WideCharToMultiByte(codepage, 0, begin, num_chars, nullptr, 0, subst_char_ptr, substitute_ptr);
    }
    buf.resize(n);

    if (WideCharToMultiByte(codepage, 0, begin, num_chars, buf.data(), n, subst_char_ptr, substitute_ptr) == 0)
        throw conversion_error();
    if (substitute) 
    {
        if (do_skip)
            buf.erase(std::remove(buf.begin(), buf.end(), subst_char), buf.end());
        else
            throw conversion_error();
    }
}

void wide_to_multibyte(int codepage, const wchar_t* begin, const wchar_t* end, bool do_skip, vector<char>& buf)
{
    if (begin == end)
        return;
    buf.reserve(end - begin);
    const wchar_t* e = std::find(begin, end, L'\0');
    const wchar_t* b = begin;
    vector<char> tmp;
    for (;;) 
    {
        wide_to_multibyte_non_zero(codepage, b, e, do_skip, tmp);
        const size_t osize = buf.size();
        buf.resize(osize + tmp.size());
        // todo: FIXME
        std::copy(tmp.begin(), tmp.end(), buf.begin() + osize);
        if (e == end)
            break;
        buf.push_back('\0');
        b = e + 1;
        e = std::find(b, end, L'0');
    }
}

template<typename _ChTy>
bool validate_utf16(const _ChTy* str, size_t len)
{
    const _ChTy* begin = str;
    const _ChTy* end = str + len;
    while (begin != end) 
    {
        /*utf::code_point c = utf::utf_traits<_ChTy, 2>::decode(begin, end);
        if (c == utf::illegal || c == utf::incomplete)
            return false;*/
    }
    return true;
}

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_WIN_CCONV_
