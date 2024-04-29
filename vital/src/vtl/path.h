#pragma once
#ifndef _VITAL_PATH_
#define _VITAL_PATH_

#include "core.h"
#include "log.h"
#include "vector.h"

#include <xstring>
#include <cstdio> // fopen ve fclose için
#include <cwchar>

#include <direct.h> // mkdir için
#include <sys/stat.h>

#ifdef VTL_PLATFORM_WINDOWS
#include <io.h>
#endif

#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

class path 
{
public:
    static constexpr wchar_t default_segment_seperator = L'\\';

    path() = default;

    path(const std::wstring& path) : m_path(path) {}

    path(const wchar_t* path) : m_path(path) {}

    path(const char* path) : m_path(std::wstring(path, path + strlen(path))) {}

    void add_segment(const std::wstring& new_segment)
    {
        m_path += L'\\' + new_segment;
    }

    path& operator+=(const std::wstring& new_segment)
    {
        add_segment(new_segment);
        return *this;
    }

    path& operator+=(const wchar_t* new_segment)
    {
        add_segment(std::wstring(new_segment, new_segment + wcslen(new_segment)));
        return *this;
    }

    path operator+(const path& other) const
    {
        path result = *this;
        result += other.get_path();
        return result;
    }

    std::wstring get_path() const
    {
        return m_path;
    }

    vector<std::wstring> get_segments() const
    {
        vector<std::wstring> segments;
        std::wstring segment;
        for (wchar_t c : m_path)
        {
            if (c == L'\\')
            {
                if (!segment.empty())
                {
                    segments.push_back(segment);
                    segment.clear();
                }
            }
            else
                segment += c;
        }
        if (!segment.empty())
            segments.push_back(segment);
        return segments;
    }

    bool exists() const
    {
        return exist(m_path);
    }

    bool is_directory() const
    {
        return is_directory(m_path);
    }

    static bool exist(const std::wstring& filepath)
    {
#ifdef VTL_PLATFORM_WINDOWS
        return (_waccess(filepath.c_str(), 0) == 0);
#else // VTL_PLATFORM_WINDOWS
        FILE* f = _wfopen(filepath.c_str(), L"r");
        if (f)
        {
            fclose(f);
            return true;
        }
        return false;
#endif // VTL_PLATFORM_WINDOWS
    }

    static bool is_directory(const std::wstring& filepath)
    {
        {
            struct _stat64i32 statbuf;
            if (_wstat(filepath.c_str(), &statbuf) != 0)
                return false;
            return S_ISDIR(statbuf.st_mode);
        }
    }

    bool create() const
    {
        if (exists())
        {
            VTL_ASSERT(false, "path already exists!");
            return false;
        }
        if (_wmkdir(m_path.c_str()) != 0)
        {
            VTL_ASSERT(false, "Failed to create path: Windows error code " + std::to_string(errno));
            return false;
        }
        return true;
    }

    void clean()
    {
        size_t pos = m_path.find_last_not_of(L'\\');
        if (pos != std::wstring::npos)
            m_path = m_path.substr(0, pos + 1);
    }

    std::wstring parent_path() const
    {
        size_t pos = m_path.find_last_of(L'\\');
        if (pos != std::wstring::npos)
            return m_path.substr(0, pos);
        return L"";
    }

    size_t length() const
    {
        return m_path.length();
    }

    std::wstring filename() const
    {
        size_t last_slash = m_path.find_last_of(_VTL_WIDE("/\\"));
        last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
        size_t last_dot = m_path.rfind(_VTL_WIDE('.'));
        size_t length = (last_dot == std::string::npos) ? m_path.size() - last_slash : last_dot - last_slash;
        return m_path.substr(last_slash, length);
    }

    std::wstring stem() const
    {
        return this->filename();
    }

    std::wstring extension() const
    {
        size_t last_dot = m_path.rfind('.');
        if (last_dot == std::string::npos)
            return _VTL_WIDE("");
        return m_path.substr(last_dot + 1);
    }

    const wchar_t* c_str() const
    {
        return m_path.c_str();
    }

    bool operator==(const path& other) const
    {
        return m_path == other.m_path;
    }

    bool operator!=(const path& other) const
    {
        return !(*this == other);
    }
private:
    std::wstring m_path;
};

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_PATH_