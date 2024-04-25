#include "filesystem.h"
#include "string_operations.h"

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
				out << output;
				out.close();
			}
	}

	void write_to_file(FILE* file, const std::string& output, bool append)
	{
		std::ofstream out(file);
		if (out)
		{
			out << output;
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

	VTL_NODISCARD std::string read_file(FILE* file)
	{
		std::string result;
		std::ifstream in(file);
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

	FILE* create_file(const std::string& filepath)
	{
		// todo: what is this code???? still user needs to control
		FILE* f = fopen(filepath.c_str(), "w+");
		if (f != NULL)
			return f;
		return nullptr;
	}

	FILE* create_file(const std::string& filepath, const std::string& filename, const std::string& extension)
	{
		if (!exist(filepath))
			return nullptr;
		std::string new_extension = (string_operations::starts_with(extension, '.')) ? extension : '.' + extension;
		std::string new_path = (string_operations::ends_with(filepath, '\\') || string_operations::ends_with(filepath, '/')) ? filepath : filepath + '\\';
		std::string full_path = new_path + filename + new_extension;
		FILE* f = fopen(full_path.c_str(), "w+");
		if (f != NULL)
			return f;
		return nullptr;
	}

	void clear_file_contents(const std::string& filepath)
	{
		std::ofstream ofile(filepath, std::ios::out | std::ios::trunc);
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

	std::string fetch_filename(const std::string& filepath)
	{
		size_t last_slash = filepath.find_last_of("/\\");
		last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
		size_t last_dot = filepath.rfind('.');
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

	pair<std::string> fetch_filename_and_extension(const std::string& filepath)
	{
		size_t last_slash = filepath.find_last_of("/\\");
		last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
		size_t last_dot = filepath.rfind('.');
		size_t length = (last_dot == std::string::npos) ? filepath.size() - last_slash : last_dot - last_slash;
		return vtl::make_pair(filepath.substr(last_slash, length), last_dot != std::string::npos ? filepath.substr(last_dot) : "");
	}

	trio<std::string> fetch_all(const std::string& filepath)
	{
		size_t last_slash = filepath.find_last_of("/\\");
		last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
		size_t last_dot = filepath.rfind('.');
		size_t length = (last_dot == std::string::npos) ? filepath.size() - last_slash : last_dot - last_slash;
		return make_trio(filepath.substr(0, last_slash), filepath.substr(last_slash, length), last_dot != std::string::npos ? filepath.substr(last_dot) : "");
	}

} // namespace filesystem

_VTL_END