#pragma once
#ifndef _VITAL_LOCALE_
#define _VITAL_LOCALE_

#include "core.h"
#include "unique_ptr.h"

#include <stdexcept>
#include <string_view>

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

class conversion_error : public std::runtime_error 
{
public:
    conversion_error() : std::runtime_error("Conversion failed") {}
};

class invalid_charset_error : public std::runtime_error 
{
public:
    invalid_charset_error(const std::string& charset) : std::runtime_error("Invalid or unsupported charset: " + charset) {}
};

enum method_type 
{
    skip = 0,             // illegal/unconvertible characters
    stop = 1,             // conversion and throw conversion_error
    default_method = skip // Default method - skip
};

template<typename _ChIn, typename _ChOut>
class charset_converter
{
public:
    using char_out_type = _ChOut;
    using char_in_type  = _ChIn;
    using string_type   = std::basic_string<_ChOut>;

    virtual ~charset_converter() = default;
    virtual string_type convert(const _ChIn* begin, const _ChIn* end) = 0;

    string_type convert(const std::basic_string_view<_ChIn>& text)
    {
        return convert(text.data(), text.data() + text.length());
    }
};

using narrow_converter = charset_converter<char, char>;

template<typename _ChTy>
using utf_encoder = charset_converter<char, _ChTy>;

template<typename _ChTy>
using utf_decoder = charset_converter<_ChTy, char>;

enum class conv_backend { Default, IConv, ICU, WinAPI };

template<typename Char>
unique_ptr<utf_encoder<Char>> make_utf_encoder(const std::string& charset, method_type how, conv_backend impl = conv_backend::Default);
template<typename Char>
unique_ptr<utf_decoder<Char>> make_utf_decoder(const std::string& charset, method_type how, conv_backend impl = conv_backend::Default);
unique_ptr<narrow_converter> make_narrow_converter(const std::string& src_encoding, const std::string& target_encoding, method_type how, conv_backend impl = conv_backend::Default);

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_LOCALE_