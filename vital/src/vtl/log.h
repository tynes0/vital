#pragma once
#ifndef _VTL_LOG_
#define _VTL_LOG_

#include "vendor/dtlog.h"

#include "core.h"

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

class log
{
public:
	static dtlog::logger& get_logger()
	{
		static dtlog::logger s_logger("VITAL", "[%T] %N: %V%n");
		return s_logger;
	}
};

_VTL_END

#define VTL_LOG_TRACE(...)		vtl::log::get_logger().trace(__VA_ARGS__)
#define VTL_LOG_INFO(...)		vtl::log::get_logger().info(__VA_ARGS__)
#define VTL_LOG_DEBUG(...)		vtl::log::get_logger().debug(__VA_ARGS__)
#define VTL_LOG_WARN(...)		vtl::log::get_logger().warning(__VA_ARGS__)
#define VTL_LOG_ERROR(...)		vtl::log::get_logger().error(__VA_ARGS__)
#define VTL_LOG_CRITICAL(...)	vtl::log::get_logger().critical(__VA_ARGS__)

#ifdef VTL_ENABLE_ASSERTS
#define VTL_ASSERT(cond, ...)  do { if(!(cond)) { VTL_LOG_CRITICAL("Vital Assertion Failed: File -> ({0}) Line -> ({1}) Error Message -> {2}", __FILE__, __LINE__ ,__VA_ARGS__); __debugbreak(); } } while (false)
#else // VTL_ENABLE_ASSERTS
#define VTL_ASSERT(cond, ...
#endif // VTL_ENABLE_ASSERTS

#pragma warning(pop)

#endif // !_VTL_LOG_