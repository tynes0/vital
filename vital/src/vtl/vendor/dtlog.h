/*
 * This file is part of the dtlog library, originally created by Tynes0.
 * For the latest version and updates, please visit the official dtlog GitHub repository:
 * https://github.com/tynes0/dtlog
 *
 * dtlog is a basic library for logging, providing fast and user-friendly use
 * It is released under the Apache License 2.0. See the LICENSE file in the root of the dtlog repository
 * or visit the above GitHub link for more details.
 *
 * For contributions, bug reports, or other inquiries, feel free to contact the author:
 * - GitHub: https://github.com/tynes0
 * - Email: cihanbilgihan@gmail.com
 */

#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS

#include <string>
#include <sstream>
#include <iomanip>

#if _HAS_NODISCARD
#define DTLOG_NODISCARD [[nodiscard]]								// nodiscard attribute define
#else // _HAS_NODISCARD
#define DTLOG_NODISCARD 											// nodiscard attribute define
#endif // _HAS_NODISCARD

namespace dtlog
{
	template <typename T>
	class helper_vector
	{
	public:
		helper_vector() : m_data(nullptr), m_capacity(0), m_size(0) {}

		~helper_vector()
		{
			delete[] m_data;
		}

		T& operator[](size_t index)
		{
			if (index >= m_size)
				throw std::out_of_range("Index out of range!");
			return m_data[index];
		}

		const T& operator[](size_t index) const
		{
			if (index >= m_size)
				throw std::out_of_range("Index out of range!");
			return m_data[index];
		}

		void push_back(const T& value)
		{
			if (m_size >= m_capacity)
			{
				size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
				reserve(new_capacity);
			}
			m_data[m_size++] = value;
		}

		void pop_back()
		{
			if (size > 0)
				--m_size;
		}

		size_t size() const
		{
			return m_size;
		}

		size_t capacity() const
		{
			return m_capacity;
		}

		T* begin() const
		{
			return m_data;
		}

		T* end() const
		{
			return m_data + m_size;
		}

	private:
		void reserve(size_t new_capacity)
		{
			T* new_data = new T[new_capacity];
			std::memcpy(new_data, m_data, sizeof(T) * m_size);
			delete[] m_data;
			m_data = new_data;
			m_capacity = new_capacity;
		}
	private:
		T* m_data;
		size_t m_capacity;
		size_t m_size;
	};

	class formatter
	{
	public:
		template <typename... _Args>
		DTLOG_NODISCARD static std::string format(const std::string& fmt, _Args&&... args)
		{
			if (sizeof...(args) == 0)
			{
				return fmt;
			}

			argument_array argArray;
			transfer_to_array(argArray, std::forward<_Args>(args)...);
			size_t start = 0;
			size_t pos = 0;
			std::ostringstream oss;
			while (true)
			{
				pos = fmt.find('{', start);
				if (pos == std::string::npos)
				{
					oss << fmt.substr(start);
					break;
				}

				oss << fmt.substr(start, pos - start);
				if (fmt[pos + 1] == '{')
				{
					oss << '{';
					start = pos + 2;
					continue;
				}

				start = pos + 1;
				pos = fmt.find('}', start);
				if (pos == std::string::npos)
				{
					oss << fmt.substr(start - 1);
					break;
				}

				format_item(oss, fmt.substr(start, pos - start), argArray);
				start = pos + 1;
			}

			return oss.str();
		}

		template <typename... Args>
		std::string operator()(const std::string& fmt, Args&&... args)
		{
			return format(fmt, std::forward<Args>(args)...);
		}

	private:
		struct argument_base
		{
			argument_base() {}
			virtual ~argument_base() {}
			virtual void format(std::ostringstream&) {}
		};

		template <class _Ty>
		class argument : public argument_base
		{
		public:
			argument(_Ty arg) : m_argument(arg) {}

			virtual ~argument() override {}

			virtual void format(std::ostringstream& oss) override
			{
				oss << m_argument;
			}

		private:
			_Ty m_argument;
		};

		class argument_array : public helper_vector<argument_base*>
		{
		public:
			argument_array() {}
			~argument_array()
			{
				for (auto item : *this)
				{
					delete item;
				}
			}
		};

		static void format_item(std::ostringstream& oss, const std::string& item, const argument_array& arguments)
		{
			size_t index = 0;
			char* endptr = nullptr;
#if _WIN64
			index = std::strtoull(&item[0], &endptr, 10);
#else // !_WIN64
			index = std::strtoul(&item[0], &endptr, 10);
#endif // _WIN64

			if (index < 0 || index >= arguments.size())
				return;
			arguments[index]->format(oss);
		}

		static void transfer_to_array(argument_array& arg_array) {}

		template <class _Ty>
		static void transfer_to_array(argument_array& arg_array, _Ty arg)
		{
			arg_array.push_back(new argument<_Ty>(arg));
		}

		template <class _Ty, class... _Args>
		static void transfer_to_array(argument_array& arg_array, _Ty arg, _Args&&... args)
		{
			transfer_to_array(arg_array, arg);
			transfer_to_array(arg_array, args...);
		}
	};

	class date_time_formatter
	{
	public:
		date_time_formatter()
		{
			reset_time();
		}

		explicit date_time_formatter(const std::tm* timeptr) : m_timeptr(timeptr) {}

#pragma warning(push)
#pragma warning(disable : 4996)
		void reset_time()
		{
			std::time_t t = std::time(nullptr);
			m_timeptr = std::localtime(&t);
		}
#pragma warning(pop)

		DTLOG_NODISCARD std::string full_weekday_name() const
		{
			return weekdays(m_timeptr->tm_wday);
		}

		DTLOG_NODISCARD std::string full_month_name() const
		{
			return months(m_timeptr->tm_mon);
		}

		DTLOG_NODISCARD std::string year_2_digits() const
		{
			return std::to_string(m_timeptr->tm_year % 100);
		}

		DTLOG_NODISCARD std::string year_4_digits() const
		{
			return std::to_string(m_timeptr->tm_year + 1900);
		}

		DTLOG_NODISCARD std::string date_time_representation() const
		{
			std::ostringstream oss;
			oss << weekdays(m_timeptr->tm_wday)
				<< " "
				<< months(m_timeptr->tm_mon)
				<< " "
				<< m_timeptr->tm_mday
				<< " "
				<< m_timeptr->tm_year + 1900
				<< " "
				<< format_time(m_timeptr->tm_hour)
				<< ":"
				<< format_time(m_timeptr->tm_min)
				<< ":"
				<< format_time(m_timeptr->tm_sec);
			return oss.str();
		}

		DTLOG_NODISCARD std::string short_MMDDYY_date() const
		{
			std::ostringstream oss;
			oss << format_time(m_timeptr->tm_mon + 1)
				<< "/"
				<< format_time(m_timeptr->tm_mday)
				<< "/"
				<< format_time((m_timeptr->tm_year % 100));
			return oss.str();
		}

		DTLOG_NODISCARD std::string month() const
		{
			return std::to_string(m_timeptr->tm_mon + 1);
		}

		DTLOG_NODISCARD std::string day_of_month() const
		{
			return std::to_string(m_timeptr->tm_mday);
		}

		DTLOG_NODISCARD std::string hours_24_format() const
		{
			return std::to_string(m_timeptr->tm_hour);
		}

		DTLOG_NODISCARD std::string hours_12_format() const
		{
			int hours12 = m_timeptr->tm_hour % 12;
			if (hours12 == 0)
				hours12 = 12;
			return std::to_string(hours12);
		}

		DTLOG_NODISCARD std::string minutes() const
		{
			return std::to_string(m_timeptr->tm_min);
		}

		DTLOG_NODISCARD std::string seconds() const
		{
			return std::to_string(m_timeptr->tm_sec);
		}

		DTLOG_NODISCARD std::string AM_PM() const
		{
			return (m_timeptr->tm_hour < 12) ? "AM" : "PM";
		}

		DTLOG_NODISCARD std::string clock_12_hour() const
		{
			std::ostringstream oss;
			oss << format_time((m_timeptr->tm_hour % 12 == 0) ? 12 : m_timeptr->tm_hour % 12)
				<< ":"
				<< format_time(m_timeptr->tm_min)
				<< ":"
				<< format_time(m_timeptr->tm_sec) << " "
				<< ((m_timeptr->tm_hour < 12) ? "AM" : "PM");
			return oss.str();
		}

		DTLOG_NODISCARD std::string HHMM_time_24_hour() const
		{
			std::ostringstream oss;
			oss << format_time(m_timeptr->tm_hour)
				<< ":"
				<< format_time(m_timeptr->tm_min);
			return oss.str();
		}

		DTLOG_NODISCARD std::string ISO8601_time_format() const
		{
			std::ostringstream oss;
			oss << format_time(m_timeptr->tm_hour)
				<< ":"
				<< format_time(m_timeptr->tm_min)
				<< ":"
				<< format_time(m_timeptr->tm_sec);
			return oss.str();
		}

	private:
		DTLOG_NODISCARD std::string format_time(int time_value) const
		{
			std::ostringstream oss;
			oss << std::setw(2)
				<< std::setfill('0')
				<< time_value;
			return oss.str();
		}

		DTLOG_NODISCARD std::string weekdays(int wday) const
		{
			switch (wday)
			{
			case 0: return "Sunday";
			case 1: return "Monday";
			case 2: return "Tuesday";
			case 3: return "Wednesday";
			case 4: return "Thursday";
			case 5: return "Friday";
			case 6: return "Saturday";
			default: return "Invalid Day";
			}
		}

		DTLOG_NODISCARD std::string months(int mon) const
		{
			switch (mon)
			{
			case 0: return "January";
			case 1: return "February";
			case 2: return "March";
			case 3: return "April";
			case 4: return "May";
			case 5: return "June";
			case 6: return "July";
			case 7: return "August";
			case 8: return "September";
			case 9: return "October";
			case 10: return "November";
			case 11: return "December";
			default: return "Invalid Month";
			}
		}

	private:
		const std::tm* m_timeptr;
	};

	enum class log_level
	{
		none,
		trace,
		info,
		debug,
		warning,
		error,
		critical
	};

	DTLOG_NODISCARD inline std::string log_level_to_string(log_level level)
	{
		switch (level)
		{
		case dtlog::log_level::trace:
			return "trace";
		case dtlog::log_level::info:
			return "info";
		case dtlog::log_level::debug:
			return "debug";
		case dtlog::log_level::warning:
			return "warning";
		case dtlog::log_level::error:
			return "error";
		case dtlog::log_level::critical:
			return "critical";
		case dtlog::log_level::none:
		default:
			return "none";
		}
	}

	class logger
	{
	public:
		logger(const std::string& log_name = "dtlog", const std::string& pattern = "[%R] %N: %V") : log_name(log_name), log_pattern(pattern) {}

		template <class ..._Args>
		void log(log_level level, const std::string& message, _Args&&... args)
		{
			std::string formatted_message = formatter::format(message, std::forward<_Args>(args)...);
			std::string log_message;
			pattern(level, formatted_message, log_message);
			set_stdout_color(level);
			std::fwrite(log_message.c_str(), sizeof(char), log_message.length(), stdout);
			std::fflush(stdout);
			set_stdout_color(log_level::none);
		}

		template <class ..._Args>
		void log_stderr(log_level level, const std::string& message, _Args&&... args)
		{
			std::string formatted_message = formatter::format(message, std::forward<_Args>(args)...);
			std::string log_message;
			pattern(level, formatted_message, log_message);
			set_stderr_color(level);
			std::fwrite(log_message.c_str(), sizeof(char), log_message.length(), stderr);
			std::fflush(stderr);
			set_stderr_color(log_level::none);
		}

		template <class ..._Args>
		void log_to_file(const std::string& filename, const std::string& message, _Args&&... args)
		{
			FILE* file = std::fopen(filename.c_str(), "a+");
			if (!file)
				return; // It was not successful, but instead of assertion, we just return. We don't simply log to file.
			log_to_file(file, message, std::forward<_Args>(args)...);
			std::fclose(file);
		}

		template <class ..._Args>
		void log_to_file(FILE* file, const std::string& message, _Args&&... args)
		{
			if (!file)
				return; // It was not successful, but instead of assertion, we just return. We don't simply log to file.
			std::string formatted_message = formatter::format(message, std::forward<_Args>(args)...);
			std::string log_message;
			std::fwrite(log_message.c_str(), sizeof(char), log_message.length(), file);
			std::fflush(file);
		}

		void set_name(const std::string& name)
		{
			log_name = name;
		}

		DTLOG_NODISCARD std::string get_name() const
		{
			return log_name;
		}

		void set_pattern(const std::string& format)
		{
			log_pattern = format;
		}

		DTLOG_NODISCARD std::string get_pattern() const
		{
			return log_pattern;
		}

		template <class ..._Args>
		void trace(const std::string& message, _Args&&... args)
		{
			return this->log(log_level::trace, message, std::forward<_Args>(args)...);
		}

		template <class ..._Args>
		void info(const std::string& message, _Args&&... args)
		{
			return this->log(log_level::info, message, std::forward<_Args>(args)...);
		}

		template <class ..._Args>
		void debug(const std::string& message, _Args&&... args)
		{
			return this->log(log_level::debug, message, std::forward<_Args>(args)...);
		}

		template <class ..._Args>
		void warning(const std::string& message, _Args&&... args)
		{
			return this->log(log_level::warning, message, std::forward<_Args>(args)...);
		}

		template <class ..._Args>
		void error(const std::string& message, _Args&&... args)
		{
			return this->log(log_level::error, message, std::forward<_Args>(args)...);
		}

		template <class ..._Args>
		void critical(const std::string& message, _Args&&... args)
		{
			return this->log(log_level::critical, message, std::forward<_Args>(args)...);
		}

	private:
		void pattern(log_level level, const std::string& message, std::string& formatted_message)
		{
			date_time_formatter time_formatter;
			formatted_message = log_pattern;
			size_t pos = 0;

			while (true)
			{
				pos = formatted_message.find('%', pos);
				if (pos == std::string::npos || pos == formatted_message.size() - 1)
					break;
				char token = formatted_message[pos + 1];
				switch (token)
				{
				case 'V':
					formatted_message.replace(pos, 2, message);
					break;
				case 'N':
					formatted_message.replace(pos, 2, log_name);
					break;
				case 'L':
					formatted_message.replace(pos, 2, log_level_to_string(level));
					break;
				case 'A':
					formatted_message.replace(pos, 2, time_formatter.full_weekday_name());
					break;
				case 'B':
					formatted_message.replace(pos, 2, time_formatter.full_month_name());
					break;
				case 'C':
					formatted_message.replace(pos, 2, time_formatter.year_2_digits());
					break;
				case 'Y':
					formatted_message.replace(pos, 2, time_formatter.year_4_digits());
					break;
				case 'R':
					formatted_message.replace(pos, 2, time_formatter.date_time_representation());
					break;
				case 'D':
					formatted_message.replace(pos, 2, time_formatter.short_MMDDYY_date());
					break;
				case 'm':
					formatted_message.replace(pos, 2, time_formatter.month());
					break;
				case 'd':
					formatted_message.replace(pos, 2, time_formatter.day_of_month());
					break;
				case 'H':
					formatted_message.replace(pos, 2, time_formatter.hours_24_format());
					break;
				case 'h':
					formatted_message.replace(pos, 2, time_formatter.hours_12_format());
					break;
				case 'M':
					formatted_message.replace(pos, 2, time_formatter.minutes());
					break;
				case 'S':
					formatted_message.replace(pos, 2, time_formatter.seconds());
					break;
				case 'F':
					formatted_message.replace(pos, 2, time_formatter.AM_PM());
					break;
				case 'x':
					formatted_message.replace(pos, 2, time_formatter.clock_12_hour());
					break;
				case 'X':
					formatted_message.replace(pos, 2, time_formatter.HHMM_time_24_hour());
					break;
				case 'T':
					formatted_message.replace(pos, 2, time_formatter.ISO8601_time_format());
					break;
				case '%':
					formatted_message.replace(pos, 2, "%");
					break;
				case 'n':
					formatted_message.replace(pos, 2, "\n");
					break;
				default:
					break;
				}
			}
		}

		void set_stdout_color(log_level level);
		void set_stderr_color(log_level level);
	private:
		std::string log_name;
		std::string log_pattern;
	};
} // namespace dtlog