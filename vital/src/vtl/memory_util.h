#pragma once
#ifndef _VITAL_MEMORY_UTIL_
#define _VITAL_MEMORY_UTIL_

#include "core.h"
#include "allocator.h"

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

namespace memory
{
	template <class _Ty>
	inline void construct_range(_Ty* begin, _Ty* end)
	{
		allocator<_Ty> al;
		while (begin != end)
			al.construct(begin++);
	}

	template <class _Ty>
	inline void copy_range(const _Ty* begin, const _Ty* end, _Ty* dest)
	{
		allocator<_Ty> al;
		while (begin != end)
			al.construct(dest++, *(begin++));
	}

	template <class _Ty>
	inline void destruct_range(_Ty* begin, _Ty* end)
	{
		allocator<_Ty> al;
		while (begin != end)
			al.destroy(begin++);
	}
}

_VTL_END

#pragma warning(pop)

#endif // !_VITAL_MEMORY_UTIL_