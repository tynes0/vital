#pragma once
#ifndef _VITAL_MINIMAL_SMART_POINTERS_
#define _VITAL_MINIMAL_SMART_POINTERS_

#include "auto_ptr.h"
#include "minimal_unique_ptr.h"
#include "minimal_shared_ptr.h"

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif

_VTL_START

template <class _Ty>
minimal_shared_ptr<_Ty> minimal_shared_from_minimal_unique(minimal_unique_ptr<_Ty>& mup)
{
	return minimal_shared_ptr<_Ty>{ mup.release() };
}

_VTL_END


#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif

#endif // !_VITAL_MINIMAL_SMART_POINTERS_