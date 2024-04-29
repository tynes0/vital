#pragma once
#ifndef _VITAL_FILESYSTEM_
#define _VITAL_FILESYSTEM_

// TODO: work with optional

#include "core.h"
#include "vector.h"
#include "pair.h"

#include <string>

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

namespace filesystem
{
	VTL_NODISCARD bool exist(const std::string& filepath);
	VTL_NODISCARD bool exist(const std::wstring& filepath);

	void write_to_file(const std::string& filepath, const std::string& output, bool append = false, bool write_if_exist = false);
	void write_to_file(const std::wstring& filepath, const std::wstring& output, bool append = false, bool write_if_exist = false);

	VTL_NODISCARD std::string read_file(const std::string& filepath);
	VTL_NODISCARD std::wstring read_file(const std::wstring& filepath);

	void clear_file_contents(const std::string& filepath);
	void clear_file_contents(const std::wstring& filepath);

	VTL_NODISCARD vector<std::string> separate_file_contents(const std::string& path, const std::string& token);
	VTL_NODISCARD vector<std::string> separate_file_contents(const std::string& path, char token);
	VTL_NODISCARD vector<std::wstring> separate_file_contents(const std::wstring& path, const std::wstring& token);
	VTL_NODISCARD vector<std::wstring> separate_file_contents(const std::wstring& path, wchar_t token);

	VTL_NODISCARD std::string fetch_filename(const std::string& filepath);
	VTL_NODISCARD std::wstring fetch_filename(const std::wstring& filepath);

	VTL_NODISCARD std::string fetch_file_extension(const std::string& filepath, bool without_dot = false);
	VTL_NODISCARD std::wstring fetch_file_extension(const std::wstring& filepath, bool without_dot = false);

	// first -> file name, second -> file extension
	VTL_NODISCARD pair<std::string> fetch_filename_and_extension(const std::string& filepath);
	VTL_NODISCARD pair<std::wstring> fetch_filename_and_extension(const std::wstring& filepath);

	// first -> filepath without file name and extension, second -> file name, third -> file extension
	VTL_NODISCARD trio<std::string> fetch_all(const std::string& filepath);
	VTL_NODISCARD trio<std::wstring> fetch_all(const std::wstring& filepath);

} // namespace filesystem

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_FILESYSTEM_