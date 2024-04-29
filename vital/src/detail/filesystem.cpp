#include "../vtl/filesystem.h"
#include "../vtl/string_operations.h"

#include <fstream>

#ifdef VTL_PLATFORM_WINDOWS
#include <io.h> // _access
#endif

_VTL_START

namespace filesystem
{

	bool exist(const std::string& filepath)
	{
#ifdef VTL_PLATFORM_WINDOWS
		return (_access(filepath.c_str(), 0) == 0);
#else
		FILE* f = fopen(filepath.c_str(), "r");
		if (f != NULL)
		{
			fclose(f);
			return true;
		}
		return false;
#endif // VTL_PLATFORM_WINDOWS
	}

	bool exist(const std::wstring& filepath)
	{
#ifdef VTL_PLATFORM_WINDOWS
		return (_waccess(filepath.c_str(), 0) == 0);
#else
		FILE* f = _wfopen(filepath.c_str(), _VTL_WIDE("r"));
		if (f != NULL)
		{
			fclose(f);
			return true;
		}
		return false;
#endif // VTL_PLATFORM_WINDOWS
	}

	void write_to_file(const std::string& filepath, const std::string& output, bool append, bool write_if_exist)
	{
		if (write_if_exist)
		{
			if (!exist(filepath))
				return;
		}
			std::ofstream out(filepath, (!append) ? std::ios::out : std::ios::app);
			if (out)
			{
				out.write(output.c_str(), output.size());
				out.close();
			}
	}

	void write_to_file(const std::wstring& filepath, const std::wstring& output, bool append, bool write_if_exist)
	{
		if (write_if_exist)
		{
			if (!exist(filepath))
				return;
		}
		std::wofstream out(filepath, (!append) ? std::ios::out : std::ios::app);
		if (out)
		{
			out.write(output.c_str(), output.size());
			out.close();
		}
	}

	std::string read_file(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		return result;
	}

	std::wstring read_file(const std::wstring& filepath)
	{
		std::wstring result;
		std::wifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		return result;
	}

	void clear_file_contents(const std::string& filepath)
	{
		std::ofstream ofile(filepath, std::ios::out | std::ios::trunc);
		if (ofile)
			ofile.close();
	}

	void clear_file_contents(const std::wstring& filepath)
	{
		std::wofstream ofile(filepath, std::ios::out | std::ios::trunc);
		if (ofile)
			ofile.close();
	}

	vector<std::string> separate_file_contents(const std::string& path, const std::string& token)
	{
		return string_operations::separate_string(read_file(path), token);
	}

	vector<std::string> separate_file_contents(const std::string& path, char token)
	{
		return string_operations::separate_string(read_file(path), token);
	}

	vector<std::wstring> separate_file_contents(const std::wstring& path, const std::wstring& token)
	{
		return string_operations::separate_string(read_file(path), token);
	}

	vector<std::wstring> separate_file_contents(const std::wstring& path, wchar_t token)
	{
		return string_operations::separate_string(read_file(path), token);
	}

	std::string fetch_filename(const std::string& filepath)
	{
		size_t last_slash = filepath.find_last_of("/\\");
		last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
		size_t last_dot = filepath.rfind('.');
		size_t length = (last_dot == std::string::npos) ? filepath.size() - last_slash : last_dot - last_slash;
		return filepath.substr(last_slash, length);
	}

	std::wstring fetch_filename(const std::wstring& filepath)
	{
		size_t last_slash = filepath.find_last_of(_VTL_WIDE("/\\"));
		last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
		size_t last_dot = filepath.rfind(_VTL_WIDE('.'));
		size_t length = (last_dot == std::string::npos) ? filepath.size() - last_slash : last_dot - last_slash;
		return filepath.substr(last_slash, length);
	}

	std::string fetch_file_extension(const std::string& filepath, bool without_dot)
	{
		size_t last_dot = filepath.rfind('.');
		if (last_dot == std::string::npos)
			return "";
		if(!without_dot)
			return filepath.substr(last_dot);
		return filepath.substr(last_dot + 1);
	}

	std::wstring fetch_file_extension(const std::wstring& filepath, bool without_dot)
	{
		size_t last_dot = filepath.rfind('.');
		if (last_dot == std::string::npos)
			return _VTL_WIDE("");
		if (!without_dot)
			return filepath.substr(last_dot);
		return filepath.substr(last_dot + 1);
	}

	pair<std::string> fetch_filename_and_extension(const std::string& filepath)
	{
		size_t last_slash = filepath.find_last_of("/\\");
		last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
		size_t last_dot = filepath.rfind('.');
		size_t length = (last_dot == std::string::npos) ? filepath.size() - last_slash : last_dot - last_slash;
		return vtl::make_pair(filepath.substr(last_slash, length), last_dot != std::string::npos ? filepath.substr(last_dot) : "");
	}

	pair<std::wstring> fetch_filename_and_extension(const std::wstring& filepath)
	{
		size_t last_slash = filepath.find_last_of(_VTL_WIDE("/\\"));
		last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
		size_t last_dot = filepath.rfind(_VTL_WIDE('.'));
		size_t length = (last_dot == std::string::npos) ? filepath.size() - last_slash : last_dot - last_slash;
		return vtl::make_pair(filepath.substr(last_slash, length), last_dot != std::string::npos ? filepath.substr(last_dot) : _VTL_WIDE(""));
	}

	trio<std::string> fetch_all(const std::string& filepath)
	{
		size_t last_slash = filepath.find_last_of("/\\");
		last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
		size_t last_dot = filepath.rfind('.');
		size_t length = (last_dot == std::string::npos) ? filepath.size() - last_slash : last_dot - last_slash;
		return make_trio(filepath.substr(0, last_slash), filepath.substr(last_slash, length), last_dot != std::string::npos ? filepath.substr(last_dot) : "");
	}

	trio<std::wstring> fetch_all(const std::wstring& filepath)
	{
		size_t last_slash = filepath.find_last_of(_VTL_WIDE("/\\"));
		last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
		size_t last_dot = filepath.rfind(_VTL_WIDE('.'));
		size_t length = (last_dot == std::string::npos) ? filepath.size() - last_slash : last_dot - last_slash;
		return make_trio(filepath.substr(0, last_slash), filepath.substr(last_slash, length), last_dot != std::string::npos ? filepath.substr(last_dot) : _VTL_WIDE(""));
	}

} // namespace filesystem

_VTL_END