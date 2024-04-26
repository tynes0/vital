#pragma once
#ifndef _VITAL_CORE_
#define _VITAL_CORE_

#include <version>

#define _VTL_START namespace vtl {
#define _VTL_END }					

#define VTL_BIT(x) (1 << x)			

#define DREF(x) *(x)				

#ifdef _WIN64
	#define VTL_PLATFORM_WINDOWS
#endif // _WIN64


#define _VTL_STRINGIZE_(x) #x
#define _VTL_STRINGIZE(x) _VTL_STRINGIZE_(x)

#define _VTL_CONCATENATE_(a, b) a ## b
#define _VTL_CONCATENATE(a, b)  _VTL_CONCATENATE_(a, b)

#define _VTL_HAS_CPP_VERSION(x) _VTL_CONCATENATE(_HAS_CXX, x)

#define _VTL_TEST_CPP_FT(x) _VTL_CONCATENATE(__cpp_, x)

#if !_VTL_HAS_CPP_VERSION(17)
	#define _VTL_DISABLED_WARNING_C4984 4984
#else // !_VTL_HAS_CPP_VERSION(17)
	#define _VTL_DISABLED_WARNING_C4984
#endif // !_VTL_HAS_CPP_VERSION(17)

#define _VTL_DISABLED_WARNINGS	\
4180 4390 4514 4619 5053		\
_VTL_DISABLED_WARNING_C4984

#if defined(__CUDACC__) || defined(__INTEL_COMPILER)
	#define _VTL_PRAGMA(PRAGMA) __pragma(PRAGMA)
#else
	#define _VTL_PRAGMA(PRAGMA) _Pragma(#PRAGMA)
#endif

#define _VTL_PRAGMA_MESSAGE(MESSAGE) _VTL_PRAGMA(message(MESSAGE))
#define _VTL_PRAGMA_WARNING(MESSAGE) _VTL_PRAGMA(warning(MESSAGE))
#define _VTL_PRAGMA_WARNING_DISABLE(warning_code) _VTL_PRAGMA(warning(disable : warning_code))

#define _EMIT_VTL_MESSAGE(MESSAGE)   _VTL_PRAGMA_MESSAGE(__FILE__ "(" _VTL_STRINGIZE(__LINE__) "): " MESSAGE)

#ifdef _VTL_DISABLE_EMIT_WARNINGS
	#define _EMIT_VTL_WARNING(NUMBER, MESSAGE)
#else // _VTL_DISABLE_EMIT_WARNINGS
	#define _EMIT_VTL_WARNING(NUMBER, MESSAGE) _EMIT_VTL_MESSAGE("warning " #NUMBER ": " MESSAGE " (define _VTL_DISABLE_EMIT_WARNINGS to suppress this warning)")
#endif // _VTL_DISABLE_EMIT_WARNINGS

#ifdef _VTL_DISABLE_EMIT_ERRORS
	#define _EMIT_VTL_ERROR(NUMBER, MESSAGE)
#else // _VTL_DISABLE_EMIT_ERRORS
	#define _EMIT_VTL_ERROR(NUMBER, MESSAGE) _EMIT_VTL_MESSAGE("error " #NUMBER ": " MESSAGE " (define _VTL_DISABLE_EMIT_ERRORS to suppress this error[not recomended])")
#endif // _VTL_DISABLE_EMIT_ERRORS


#ifndef __has_cpp_attribute
	#define _VTL_HAS_CPP_ATTRIBUTE(x) 0
#elif defined(__CUDACC__)
	#define _VTL_HAS_CPP_ATTRIBUTE(x) 0
#else
	#define _VTL_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#endif

#if _VTL_HAS_CPP_ATTRIBUTE(nodiscard)
	#define VTL_NODISCARD [[nodiscard]]								// nodiscard attribute define
#else // _VTL_HAS_CPP_ATTRIBUTE(nodiscard)
	#define VTL_NODISCARD 											// nodiscard attribute define
#endif // _VTL_HAS_CPP_ATTRIBUTE(nodiscard)

#if _VTL_HAS_CPP_ATTRIBUTE(nodiscard) >= 201907L
	#define VTL_NODISCARD20 [[nodiscard]]
	#define VTL_NODISCARD_MSG(msg) [[nodiscard(msg)]]
#else // _VTL_HAS_CPP_ATTRIBUTE(nodiscard) >= 201907L
	#define VTL_NODISCARD20 
	#if _VTL_HAS_CPP_ATTRIBUTE(nodiscard)
		#define VTL_NODISCARD_MSG(msg) [[nodiscard]]				
	#else // _VTL_HAS_CPP_ATTRIBUTE(nodiscard)
		#define VTL_NODISCARD_MSG(msg) 
	#endif // _VTL_HAS_CPP_ATTRIBUTE(nodiscard)
#endif //_VTL_HAS_CPP_ATTRIBUTE(nodiscard) >= 201907L

#if _VTL_HAS_CPP_ATTRIBUTE(noreturn)
	#define VTL_NORETURN [[noreturn]]
#else // !_VTL_HAS_CPP_ATTRIBUTE(noreturn)
	#define VTL_NORETURN
#endif //_VTL_HAS_CPP_ATTRIBUTE(noreturn)

#if _VTL_HAS_CPP_VERSION(17)
	#define VTL_INLINE inline			// inline keyword for constants
#else // _VTL_HAS_CPP_VERSION(17)
	#define VTL_INLINE					// inline keyword for constants
#endif // _VTL_HAS_CPP_VERSION(17)

#if _MSC_VER
	#define VTL_FORCE_INLINE __forceinline
#elif __GNUC__ || __clang__
	#define VTL_FORCE_INLINE [[gnu::always_inline]]
#else
	#define VTL_FORCE_INLINE
#endif

#if _VTL_HAS_CPP_VERSION(17)
	#define VTL_CONSTEXPR17 constexpr
#else
	#define VTL_CONSTEXPR17
#endif

#if _VTL_HAS_CPP_VERSION(20)
	#define VTL_CONSTEXPR20 constexpr		// constexpr keyword for constants
#else
	#define VTL_CONSTEXPR20		// constexpr keyword for constants
#endif // _VTL_HAS_CPP_VERSION(20)

#if _VTL_HAS_CPP_VERSION(23)
	#define VTL_CONSTEXPR23 constexpr
#else
	#define VTL_CONSTEXPR23
#endif

#pragma push_macro("msvc")
#pragma push_macro("known_semantics")
#pragma push_macro("noop_dtor")
#pragma push_macro("intrinsic")
#undef msvc
#undef known_semantics
#undef noop_dtor
#undef intrinsic

#define _VTL_HAS_MSVC_ATTRIBUTE(x) _VTL_HAS_CPP_ATTRIBUTE(msvc::x)

#if _VTL_HAS_MSVC_ATTRIBUTE(known_semantics)
	#define _VTL_MSVC_KNOWN_SEMANTICS [[msvc::known_semantics]]
#else
	#define _VTL_MSVC_KNOWN_SEMANTICS
#endif

#if _VTL_HAS_MSVC_ATTRIBUTE(noop_dtor)
	#define _VTL_MSVC_NOOP_DTOR [[msvc::noop_dtor]]
#else
	#define _VTL_MSVC_NOOP_DTOR
#endif

#if _VTL_HAS_MSVC_ATTRIBUTE(intrinsic)
	#define _VTL_MSVC_INTRINSIC [[msvc::intrinsic]]
#else
	#define _VTL_MSVC_INTRINSIC
#endif

#undef _VTL_HAS_MSVC_ATTRIBUTE
#pragma pop_macro("intrinsic")
#pragma pop_macro("noop_dtor")
#pragma pop_macro("known_semantics")
#pragma pop_macro("msvc")

#ifdef VITAL_DEBUG
	#define VTL_ENABLE_ASSERTS 1			// vtl asserts enabled
#endif // VITAL_DEBUG

_VTL_START

typedef decltype(nullptr) nullptr_t;

_VTL_END

#endif // !_VITAL_CORE_
