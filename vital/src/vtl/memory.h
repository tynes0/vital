#pragma once
#ifndef _VTL_MEMORY_
#define _VTL_MEMORY_

#include "core.h"
#include "type_traits.h"

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

template <class _Ty>
struct default_delete 
{
    constexpr default_delete() noexcept = default;

    template <class _Ty2, enable_if_t<is_convertible_v<_Ty2*, _Ty*>, int> = 0>
    VTL_CONSTEXPR23 default_delete(const default_delete<_Ty2>&) noexcept {}

    VTL_CONSTEXPR23 void operator()(_Ty* ptr) const noexcept
    {
        static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");
        delete ptr;
    }
};

template <class _Ty>
struct default_delete<_Ty[]> 
{
    constexpr default_delete() noexcept = default;

    template <class _Uty, enable_if_t<is_convertible_v<_Uty(*)[], _Ty(*)[]>, int> = 0>
    VTL_CONSTEXPR23 default_delete(const default_delete<_Uty[]>&) noexcept {}

    template <class _Uty, enable_if_t<is_convertible_v<_Uty(*)[], _Ty(*)[]>, int> = 0>
    VTL_CONSTEXPR23 void operator()(_Uty* ptr) const noexcept
    {
        static_assert(0 < sizeof(_Uty), "can't delete an incomplete type");
        delete[] ptr;
    }
};

template <class _Ty, class _Dx_noref, class = void>
struct get_deleter_pointer_type 
{
    using type = _Ty*;
};

template <class _Ty, class _Dx_noref>
struct get_deleter_pointer_type<_Ty, _Dx_noref, void_t<typename _Dx_noref::pointer>> 
{
    using type = typename _Dx_noref::pointer;
};

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VTL_MEMORY_