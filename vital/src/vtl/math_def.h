#pragma once
#ifndef _VITAL_MATH_DEF_
#define _VITAL_MATH_DEF_

#include "core.h"

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

//todo is transparent

_VTL_START

template <class _Ty = void>
struct plus 
{
    VTL_NODISCARD constexpr _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left + _Right;
    }
};

template <class _Ty = void>
struct minus 
{
    VTL_NODISCARD constexpr _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left - _Right;
    }
};

template <class _Ty = void>
struct multiplies 
{
    VTL_NODISCARD constexpr _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left * _Right;
    }
};

template <class _Ty = void>
struct equal_to 
{
    VTL_NODISCARD constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left == _Right;
    }
};

template <class _Ty = void>
struct not_equal_to 
{
    VTL_NODISCARD constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left != _Right;
    }
};

template <class _Ty = void>
struct greater 
{
    VTL_NODISCARD constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left > _Right;
    }
};

template <class _Ty = void>
struct less 
{
    VTL_NODISCARD constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left < _Right;
    }
};

template <class _Ty = void>
struct greater_equal 
{
    VTL_NODISCARD constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left >= _Right;
    }
};

template <class _Ty = void>
struct less_equal 
{
    VTL_NODISCARD constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left <= _Right;
    }
};

template <>
struct plus<void> 
{
    template <class _Ty1, class _Ty2>
    VTL_NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(noexcept(static_cast<_Ty1&&>(_Left) + static_cast<_Ty2&&>(_Right)))
        -> decltype(static_cast<_Ty1&&>(_Left) + static_cast<_Ty2&&>(_Right)) 
    {
        return static_cast<_Ty1&&>(_Left) + static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};

template <>
struct minus<void> 
{
    template <class _Ty1, class _Ty2>
    VTL_NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(noexcept(static_cast<_Ty1&&>(_Left) - static_cast<_Ty2&&>(_Right)))
        -> decltype(static_cast<_Ty1&&>(_Left) - static_cast<_Ty2&&>(_Right)) 
    {
        return static_cast<_Ty1&&>(_Left) - static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};

template <>
struct multiplies<void> 
{
    template <class _Ty1, class _Ty2>
    VTL_NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(noexcept(static_cast<_Ty1&&>(_Left)* static_cast<_Ty2&&>(_Right)))
        -> decltype(static_cast<_Ty1&&>(_Left)* static_cast<_Ty2&&>(_Right)) 
    {
        return static_cast<_Ty1&&>(_Left) * static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};

template <>
struct equal_to<void>
{
    template <class _Ty1, class _Ty2>
    VTL_NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(noexcept(static_cast<_Ty1&&>(_Left) == static_cast<_Ty2&&>(_Right)))
        -> decltype(static_cast<_Ty1&&>(_Left) == static_cast<_Ty2&&>(_Right)) 
    {
        return static_cast<_Ty1&&>(_Left) == static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};

template <>
struct not_equal_to<void> 
{
    template <class _Ty1, class _Ty2>
    VTL_NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(noexcept(static_cast<_Ty1&&>(_Left) != static_cast<_Ty2&&>(_Right)))
        -> decltype(static_cast<_Ty1&&>(_Left) != static_cast<_Ty2&&>(_Right))
    {
        return static_cast<_Ty1&&>(_Left) != static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};

template <>
struct greater<void>
{
    template <class _Ty1, class _Ty2>
    VTL_NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(noexcept(static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right)))
        -> decltype(static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right)) 
    {
        return static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};

template <>
struct less<void> 
{
    template <class _Ty1, class _Ty2>
    VTL_NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(noexcept(static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right)))
        -> decltype(static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right)) 
    {
        return static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};

template <>
struct greater_equal<void> 
{
    template <class _Ty1, class _Ty2>
    VTL_NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(noexcept(static_cast<_Ty1&&>(_Left) >= static_cast<_Ty2&&>(_Right)))
        -> decltype(static_cast<_Ty1&&>(_Left) >= static_cast<_Ty2&&>(_Right))
    {
        return static_cast<_Ty1&&>(_Left) >= static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};

template <>
struct less_equal<void> 
{
    template <class _Ty1, class _Ty2>
    VTL_NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(noexcept(static_cast<_Ty1&&>(_Left) <= static_cast<_Ty2&&>(_Right)))
        -> decltype(static_cast<_Ty1&&>(_Left) <= static_cast<_Ty2&&>(_Right)) 
    {
        return static_cast<_Ty1&&>(_Left) <= static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};

VTL_NODISCARD constexpr float calculate_aspect_ratio(float x, float y)
{
    return (x / y);
}

template <class _Ty, enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
constexpr _Ty wabs(const _Ty& _val) noexcept
{
    return (_val < 0) ? (-_val) : _val;
}

template <class _Ty, enable_if_t<std::is_floating_point_v<_Ty>, int> = 0>
constexpr _Ty wceil(const _Ty& _val) noexcept
{
    _Ty result = static_cast<_Ty>(static_cast<long long>(_val));
    if (_val > result)
    {
        result += 1;
    }
    return result;
}

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_MATH_DEF_