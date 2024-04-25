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

#include "dtlog.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void dtlog::logger::set_stdout_color(log_level level)
{
	WORD color_code = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	switch (level)
	{
	case log_level::none:
	case log_level::trace:
		break;
	case log_level::info:
		color_code = FOREGROUND_GREEN;
		break;
	case log_level::debug:
		color_code = FOREGROUND_BLUE;
		break;
	case log_level::warning:
		color_code = FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case log_level::error:
		color_code = FOREGROUND_RED;
		break;
	case log_level::critical:
		color_code = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED;
		break;
	default:
		break;
	}

	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (console_handle == INVALID_HANDLE_VALUE)
		throw std::invalid_argument("INVALID STD HANDLE (logger::set_stdout_color())");
	SetConsoleTextAttribute(console_handle, color_code);
}