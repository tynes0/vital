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

#include "locale_encoding.h"

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
            buf.erase(std::remove(buf.begin(), buf.end(), subst_char), buf.end()); // todo fixme
        else
            throw conversion_error();
    }
}

void wide_to_multibyte(int codepage, const wchar_t* begin, const wchar_t* end, bool do_skip, vector<char>& buf)
{
    if (begin == end)
        return;
    buf.reserve(end - begin);
    const wchar_t* e = basic_find(begin, end, L'\0');
    const wchar_t* b = begin;
    vector<char> tmp;
    for (;;) 
    {
        wide_to_multibyte_non_zero(codepage, b, e, do_skip, tmp);
        const size_t osize = buf.size();
        buf.resize(osize + tmp.size());
        std::copy(tmp.begin(), tmp.end(), buf.begin() + osize); // todo: FIXME
        if (e == end)
            break;
        buf.push_back('\0');
        b = e + 1;
        e = basic_find(b, end, L'0');
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
    }// TODO: FIXME
    return true;
}

template<typename _ChTy, typename _ChOut>
void clean_invalid_utf16(const _ChTy* str, size_t len, vector<_ChOut>& out)
{
    out.reserve(len);
    for (size_t i = 0; i < len; i++) 
    {
        uint16_t c = static_cast<uint16_t>(str[i]);

        if (0xD800 <= c && c <= 0xDBFF) 
        {
            i++;
            if (i >= len)
                return;
            uint16_t c2 = static_cast<uint16_t>(str[i]);
            if (0xDC00 <= c2 && c2 <= 0xDFFF) 
            {
                out.push_back(static_cast<_ChOut>(c));
                out.push_back(static_cast<_ChOut>(c2));
            }
        }
        else if (0xDC00 <= c && c <= 0xDFFF)
            continue;
        else
            out.push_back(static_cast<_ChOut>(c));
    }
}

class wconv_between final : public narrow_converter
{
public:
    wconv_between() : m_how(skip), m_to_code_page(-1), m_from_code_page(-1) {}
    bool open(const std::string& to_charset, const std::string& from_charset, method_type how)
    {
        m_how = how;
        
        m_to_code_page = encoding_to_windows_codepage(to_charset);
        m_from_code_page = encoding_to_windows_codepage(from_charset);
        if (m_to_code_page == -1 || m_from_code_page == -1)
            return false;
        return true;
    }

    template<typename _ChTy>
    std::basic_string<_ChTy> convert(const char* begin, const char* end)
    {
        static_assert(sizeof(_ChTy) == sizeof(char), "Not a narrow char type");
        if (m_to_code_page == CP_UTF8 && m_from_code_page == CP_UTF8)
            return utf_to_utf<_ChTy>(begin, end, m_how);

        std::basic_string<_ChTy> res;

        vector<wchar_t> tmp;
        std::wstring tmps;
        const wchar_t* wbegin = nullptr;
        const wchar_t* wend = nullptr;

        if (m_from_code_page == CP_UTF8) 
        {
            tmps/* = utf_to_utf<wchar_t>(begin, end, m_how)*/; // todo: fixme
            if (tmps.empty())
                return res;
            wbegin = tmps.c_str();
            wend = wbegin + tmps.size();
        }
        else
        {
            multibyte_to_wide(m_from_code_page, begin, end, m_how == skip, tmp);
            if (tmp.empty())
                return res;
            wbegin = tmp.data();
            wend = wbegin + tmp.size();
        }

        /*if (m_to_code_page == CP_UTF8)
            return utf_to_utf<_ChTy>(wbegin, wend, m_how);*/ // todo fixme

        vector<char> ctmp;
        wide_to_multibyte(m_to_code_page, wbegin, wend, m_how == skip, ctmp);
        if (ctmp.empty())
            return res;
        res.assign(reinterpret_cast<const _ChTy*>(ctmp.data()), ctmp.size());
        return res;
    }

    std::string convert(const char* begin, const char* end) override { return convert<char>(begin, end); }

private:
    method_type m_how;
    int m_to_code_page;
    int m_from_code_page;
};

template<typename _ChTy, int size = sizeof(_ChTy)>
class wconv_to_utf;

template<typename _ChTy, int size = sizeof(_ChTy)>
class wconv_from_utf;

template<typename _ChTy>
class wconv_to_utf<_ChTy, 1> final : public utf_encoder<_ChTy> 
{
public:
    bool open(const std::string& cs, method_type how) { return cvt.open("UTF-8", cs, how); }
    std::basic_string<_ChTy> convert(const char* begin, const char* end) override
    {
        return cvt.convert<_ChTy>(begin, end);
    }

private:
    wconv_between cvt;
};

template<typename _ChTy>
class wconv_from_utf<_ChTy, 1> final : public utf_decoder<_ChTy> 
{
public:
    bool open(const std::string& cs, method_type how) { return cvt.open(cs, "UTF-8", how); }
    std::string convert(const _ChTy* begin, const _ChTy* end) override
    {
        return cvt.convert(reinterpret_cast<const char*>(begin), reinterpret_cast<const char*>(end));
    }

private:
    wconv_between cvt;
};

template<typename _ChTy>
class wconv_to_utf<_ChTy, 2> final : public utf_encoder<_ChTy>
{
public:
    using string_type = std::basic_string<_ChTy>;

    wconv_to_utf() : m_how(skip), m_code_page(-1) {}

    bool open(const std::string& charset, method_type how)
    {
        m_how = how;
        m_code_page = encoding_to_windows_codepage(charset);
        return m_code_page != -1;
    }

    string_type convert(const char* begin, const char* end) override
    {
        /*if (m_code_page == CP_UTF8)
            return utf_to_utf<_ChTy>(begin, end, m_how);*/ // todo fixme
        vector<wchar_t> tmp;
        multibyte_to_wide(m_code_page, begin, end, m_how == skip, tmp);
        string_type res;
        if (!tmp.empty()) 
        {
            static_assert(sizeof(_ChTy) == sizeof(wchar_t), "Cast not possible");
            res.assign(reinterpret_cast<_ChTy*>(tmp.data()), tmp.size());
        }
        return res;
    }

private:
    method_type m_how;
    int m_code_page;
};

template<typename _ChTy>
class wconv_from_utf<_ChTy, 2> final : public utf_decoder<_ChTy> 
{
public:
    wconv_from_utf() : m_how(skip), m_code_page(-1) {}

    bool open(const std::string& charset, method_type how)
    {
        m_how = how;
        m_code_page = encoding_to_windows_codepage(charset);
        return m_code_page != -1;
    }

    std::string convert(const _ChTy* begin, const _ChTy* end) override
    {
        /*if (m_code_page == CP_UTF8)
            return utf_to_utf<char>(begin, end, m_how);*/ // todo fixme
        const wchar_t* wbegin;
        const wchar_t* wend;
        vector<wchar_t> buffer;
        if (validate_utf16(begin, end - begin))
        {
            static_assert(sizeof(_ChTy) == sizeof(wchar_t), "Cast not possible");
            wbegin = reinterpret_cast<const wchar_t*>(begin);
            wend = reinterpret_cast<const wchar_t*>(end);
        }
        else 
        {
            if (m_how == stop)
                throw conversion_error();
            else 
            {
                clean_invalid_utf16(begin, end - begin, buffer);
                if (buffer.empty())
                    wbegin = wend = nullptr;
                else 
                {
                    wbegin = buffer.data();
                    wend = wbegin + buffer.size();
                }
            }
        }
        std::string res;
        if (wbegin == wend)
            return res;
        vector<char> ctmp;
        wide_to_multibyte(m_code_page, wbegin, wend, m_how == skip, ctmp);
        if (ctmp.empty())
            return res;
        res.assign(ctmp.data(), ctmp.size());
        return res;
    }

private:
    method_type m_how;
    int m_code_page;
};

template<typename _ChTy>
class wconv_to_utf<_ChTy, 4> final : public utf_encoder<_ChTy>
{
public:
    using string_type = std::basic_string<_ChTy>;

    wconv_to_utf() : m_how(skip), m_code_page(-1) {}

    bool open(const std::string& charset, method_type how)
    {
        m_how = how;
        m_code_page = encoding_to_windows_codepage(charset);
        return m_code_page != -1;
    }

    string_type convert(const char* begin, const char* end) override
    {
        /*if (m_code_page == CP_UTF8)
            return utf_to_utf<_ChTy>(begin, end, m_how);*/ // todo fixme
        vector<wchar_t> buf;
        multibyte_to_wide(m_code_page, begin, end, m_how == skip, buf);

        if (buf.empty())
            return string_type();

        /*return utf_to_utf<_ChTy>(buf.data(), buf.data() + buf.size(), m_how);*/ // todo fixme
    }

private:
    method_type m_how;
    int m_code_page;
};
template<typename _ChTy>
class wconv_from_utf<_ChTy, 4> final : public utf_decoder<_ChTy> 
{
public:
    wconv_from_utf() : m_how(skip), m_code_page(-1) {}

    bool open(const std::string& charset, method_type how)
    {
        m_how = how;
        m_code_page = encoding_to_windows_codepage(charset);
        return m_code_page != -1;
    }

    std::string convert(const _ChTy* begin, const _ChTy* end) override
    {
        /*if (m_code_page == CP_UTF8)
            return utf_to_utf<char>(begin, end, m_how);*/
        std::wstring tmp /*= utf_to_utf<wchar_t>(begin, end, m_how)*/; // todo fixme

        vector<char> ctmp;
        wide_to_multibyte(m_code_page, tmp.c_str(), tmp.c_str() + tmp.size(), m_how == skip, ctmp);
        std::string res;
        if (ctmp.empty())
            return res;
        res.assign(ctmp.data(), ctmp.size());
        return res;
    }

private:
    method_type m_how;
    int m_code_page;
};


_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_WIN_CCONV_
