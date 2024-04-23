#pragma once
#ifndef _VITAL_EXTERNAL_OPERATOR_WRAPPER_
#define _VITAL_EXTERNAL_OPERATOR_WRAPPER_

// Reference from: https://github.com/tynes0/Naz-Operator-Wrapper

#include "core.h"
#include "type_traits.h"

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

template <class F>
struct operator_wrapper
{
	F fun;
};

template <class F, class _Ty>
struct operator_wrapper_lhs
{
	F fun;
	_Ty val;
};

template <class _Fun>
inline constexpr operator_wrapper<_Fun> make_operator(_Fun fun)
{
	return operator_wrapper<_Fun> { fun };
}

template <class F, class _Ty>
inline constexpr operator_wrapper_lhs<F, _Ty&> operator||(_Ty& left, const operator_wrapper<F>& right)
{
	return operator_wrapper_lhs<F, _Ty&> { right.fun, left };
}

template <class F, class _Ty>
inline constexpr operator_wrapper_lhs<F, _Ty> operator|(_Ty left, const operator_wrapper<F>& right)
{
	return operator_wrapper_lhs<F, _Ty> { right.fun, left };
}

template <class F, class _Ty1, class _Ty2>
inline constexpr auto operator||(const operator_wrapper_lhs<F, _Ty1&>& left, _Ty2& right)
-> decltype(left.fun(vtl::declval<_Ty1&>(), vtl::declval<_Ty2&>()))
{
	return left.fun(left.val, right);
}

template <class F, class _Ty1, class _Ty2>
inline constexpr auto operator||(const operator_wrapper_lhs<F, _Ty1>& left, _Ty2& right)
-> decltype(left.fun(vtl::declval<_Ty1>(), vtl::declval<_Ty2&>()))
{
	return left.fun(left.val, right);
}

template <class F, class _Ty1, class _Ty2>
inline constexpr auto operator|(const operator_wrapper_lhs<F, _Ty1>& left, _Ty2 right)
-> decltype(left.fun(vtl::declval<_Ty1>(), vtl::declval<_Ty2>()))
{
	return left.fun(left.val, right);
}

template <class F, class _Ty1, class _Ty2>
inline constexpr auto operator|(const operator_wrapper_lhs<F, _Ty1&>& left, _Ty2 right)
-> decltype(left.fun(vtl::declval<_Ty1>(), vtl::declval<_Ty2>()))
{
	return left.fun(left.val, right);
}

#define vital_operator(operator_name, return_type, left_type, right_type) inline return_type __impl_of_ ## operator_name (left_type left, right_type right); VTL_INLINE constexpr auto operator_name = vtl::make_operator(__impl_of_ ## operator_name); return_type __impl_of_ ## operator_name (left_type left, right_type right)

_VTL_END

#pragma warning(pop)

#endif // !_VITAL_EXTERNAL_OPERATOR_WRAPPER_