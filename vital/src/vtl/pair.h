#pragma once

#ifndef _VITAL_PAIR_
#define _VITAL_PAIR_

#include "core.h"
#include "type_traits.h"
#include "utility.h"
#include "reference_wrapper.h"
#include "tag.h"

#include <type_traits>

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

#define VTL_PAIR_CHECK_IDX(I) vtl::enable_if_t<I == 0 || I == 1, int> = 0
#define VTL_TRIO_CHECK_IDX(I) vtl::enable_if_t<I == 0 || I == 1 || I == 2, int> = 0
#define VTL_PAIR_CHECK_3_IDXS(I1, I2, I3) vtl::enable_if_t<(I1 == 0 || I1 == 1) && (I2 == 0 || I2 == 1) && (I3 == 0 || I3 == 1) , int> = 0
#define VTL_TRIO_CHECK_2_IDXS(I1, I2) vtl::enable_if_t<(I1 == 0 || I1 == 1 || I1 == 2) && (I2 == 0 || I2 == 1 || I2 == 2), int> = 0

template <class _Ty1, class _Ty2 = _Ty1>
struct pair
{
	using first_type	= _Ty1;
	using second_type	= _Ty2;
	first_type first;
	second_type second;

	constexpr static bool nothrow_swappable = (is_nothrow_swappable_v<_Ty1> && is_nothrow_swappable_v<_Ty2>);

	template <class _Uty1 = _Ty1, class _Uty2 = _Ty2>
	constexpr explicit(!conjunction_v<std::_Is_implicitly_default_constructible<_Uty1>, std::_Is_implicitly_default_constructible<_Uty2>>)
		pair() noexcept : first(), second() {}

	template <class _Uty1 = _Ty1, class _Uty2 = _Ty2>
	constexpr explicit(!conjunction_v<is_convertible<const _Uty1&, _Uty1>, is_convertible<const _Uty2&, _Uty2>>)
		pair(const _Ty1& first_in, const _Ty2& second_in) noexcept : first(first_in), second(second_in) {}

	template <class _Other1, class _Other2>
	constexpr explicit(!conjunction_v<is_convertible<const _Other1&, _Ty1>, is_convertible<const _Other2&, _Ty2>>)
		pair(const pair<_Other1, _Other2>& other) noexcept : first(other.first), second(other.second) {}

	pair(const pair&) = default;
	pair(pair&&) = default;

	pair& operator=(const volatile pair&) = delete;

	constexpr pair& operator=(const pair& _Right)
	{
		first = _Right.first;
		second = _Right.second;
		return *this;
	}

	constexpr bool operator==(const pair& _Right) const 
	{
		return first == _Right.first && second == _Right.second;
	}

	constexpr bool operator!=(const pair& _Right) const 
	{
		return !(*this == _Right);
	}

	template <size_t I, VTL_PAIR_CHECK_IDX(I)>
	constexpr decltype(auto) operator[](tag<I>) &
	{
		if constexpr (I == 0)
			return first;
		else // I == 1
			return second;
	}

	template <size_t I, VTL_PAIR_CHECK_IDX(I)>
	constexpr decltype(auto) operator[](tag<I>) const&
	{
		if constexpr (I == 0)
			return first;
		else // I == 1
			return second;
	}

	template <size_t I, VTL_PAIR_CHECK_IDX(I)>
	constexpr decltype(auto) operator[](tag<I>) &&
	{
		if constexpr (I == 0)
			return static_cast<pair<_Ty1, _Ty2>&&>(*this).first;
		else // I == 1
			return static_cast<pair<_Ty1, _Ty2>&&>(*this).second;
	}

	constexpr void swap(pair<_Ty1, _Ty2>& _Other)
	{
		if (this != addressof(_Other))
		{
			swap_adl(first, _Other.first);
			swap_adl(second, _Other.second);
		}
	}

	constexpr bool types_are_the_same() const noexcept
	{
		return is_same_v<_Ty1, _Ty2>;
	}
};

struct zero_then_variadic_args_t
{
	explicit zero_then_variadic_args_t() = default;
};

struct one_then_variadic_args_t
{
	explicit one_then_variadic_args_t() = default;
};

template <class _Ty1, class _Ty2, bool = is_empty_v<_Ty1> && !std::is_final_v<_Ty1>>
class compressed_pair final : private _Ty1
{
public:
	_Ty2 second;

	using m_base = _Ty1;

	template <class... Other2>
	constexpr explicit compressed_pair(zero_then_variadic_args_t, Other2&&... value2) noexcept(conjunction_v<is_nothrow_default_constructible<_Ty1>, is_nothrow_constructible<_Ty2, Other2...>>)
		: _Ty1(), second(forward<Other2>(value2)...) {}

	template <class Other1, class... Other2>
	constexpr explicit compressed_pair(one_then_variadic_args_t, Other1&& value1, Other2&&... value2) noexcept(conjunction_v<is_nothrow_constructible<_Ty1, Other1>, is_nothrow_constructible<_Ty2, Other2...>>)
		: _Ty1(forward<Other1>(value1)), second(forward<Other2>(value2)...) {}

	constexpr _Ty1& get_first() noexcept
	{
		return *this;
	}

	constexpr const _Ty1& get_first() const noexcept
	{
		return *this;
	}
};

template <class _Ty1, class _Ty2>
class compressed_pair<_Ty1, _Ty2, false> final
{
public:
	_Ty1 first;
	_Ty2 second;

	template <class... Other2>
	constexpr explicit compressed_pair(zero_then_variadic_args_t, Other2&&... value2) noexcept(conjunction_v<is_nothrow_default_constructible<_Ty1>, is_nothrow_constructible<_Ty2, Other2...>>)
		: first(), second(forward<Other2>(value2)...) {}

	template <class Other1, class... Other2>
	constexpr explicit compressed_pair(one_then_variadic_args_t, Other1&& value1, Other2&&... value2) noexcept(conjunction_v<is_nothrow_constructible<_Ty1, Other1>, is_nothrow_constructible<_Ty2, Other2...>>)
		: first(forward<Other1>(value1)), second(forward<Other2>(value2)...) {}

	constexpr _Ty1& get_first() noexcept
	{
		return first;
	}

	constexpr const _Ty1& get_first() const noexcept
	{
		return first;
	}
};

template <class _Ty1, class _Ty2 = _Ty1, class _Ty3 = _Ty1>
struct trio
{
	using first_type	= _Ty1;
	using second_type	= _Ty2;
	using third_type	= _Ty3;

	constexpr static bool nothrow_swappable = (is_nothrow_swappable_v<_Ty1> && is_nothrow_swappable_v<_Ty2> && is_nothrow_swappable_v<_Ty3>);

	first_type first;
	second_type second;
	third_type third;

	template <class _Uty1 = _Ty1, class _Uty2 = _Ty2, class _Uty3 = _Ty3>
	constexpr explicit(!conjunction_v<std::_Is_implicitly_default_constructible<_Uty1>, std::_Is_implicitly_default_constructible<_Uty2>, std::_Is_implicitly_default_constructible<_Uty3>>)
		trio() noexcept : first(), second(), third() {}

	template <class _Uty1 = _Ty1, class _Uty2 = _Ty2, class _Uty3 = _Ty3>
	constexpr explicit(!conjunction_v<is_convertible<const _Uty1&, _Uty1>, is_convertible<const _Uty2&, _Uty2>, is_convertible<const _Uty3&, _Uty3>>)
		trio(const _Ty1& first_in, const _Ty2& second_in, const _Ty3& third_in) noexcept : first(first_in), second(second_in), third(third_in) {}

	template <class _Other1, class _Other2, class _Other3>
	constexpr explicit(!conjunction_v<is_convertible<const _Other1&, _Ty1>, is_convertible<const _Other2&, _Ty2>, is_convertible<const _Other3&, _Ty3>>)
		trio(const trio<_Other1, _Other2, _Other3>& other) noexcept : first(other.first), second(other.second), third(other.third) {}

	trio(const trio&) = default;
	trio(trio&&) = default;

	trio& operator=(const volatile trio&) = delete;

	constexpr trio& operator=(const trio& _Right)
	{
		first = _Right.first;
		second = _Right.second;
		third = _Right.third;
		return *this;
	}

	bool operator==(const trio& _Right) const
	{
		return first == _Right.first && second == _Right.second && third == _Right.third;
	}

	bool operator!=(const trio& _Right) const 
	{
		return !(*this == _Right);
	}

	template <size_t I, VTL_TRIO_CHECK_IDX(I)>
	constexpr decltype(auto) operator[](tag<I>)&
	{
		if constexpr (I == 0)
			return first;
		else if constexpr (I == 1)
			return second;
		else // I == 2
			return third;
	}

	template <size_t I, VTL_TRIO_CHECK_IDX(I)>
	constexpr decltype(auto) operator[](tag<I>) const&
	{
		if constexpr (I == 0)
			return first;
		else if constexpr (I == 1)
			return second;
		else // I == 2
			return third;
	}

	template <size_t I, VTL_TRIO_CHECK_IDX(I)>
	constexpr decltype(auto) operator[](tag<I>)&&
	{
		if constexpr (I == 0)
			return static_cast<trio<_Ty1, _Ty2, _Ty3>&&>(*this).first;
		else if constexpr (I == 1)
			return static_cast<trio<_Ty1, _Ty2, _Ty3>&&>(*this).second;
		else // I == 2
			return static_cast<trio<_Ty1, _Ty2, _Ty3>&&>(*this).third;
	}

	constexpr void swap(trio<_Ty1, _Ty2, _Ty3>& _Other)
	{
		if (this != addressof(_Other))
		{
			swap_adl(first, _Other.first);
			swap_adl(second, _Other.second);
			swap_adl(third, _Other.third);
		}
	}

	constexpr bool types_are_the_same() const noexcept
	{
		return is_same_v<_Ty1, _Ty2> && is_same_v<_Ty1, _Ty3>;
	}
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// PAIR ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

template <class A, class B>
VTL_NODISCARD constexpr pair<unwrap_reference_t<A>, unwrap_reference_t<B>> make_pair(A&& first, B&& second)
	noexcept(is_nothrow_constructible_v<unwrap_reference_t<A>, A> && is_nothrow_constructible_v<unwrap_reference_t<B>, B>) 
{
	return pair<unwrap_reference_t<A>, unwrap_reference_t<B>>{ vtl::forward<A>(first), vtl::forward<B>(second) };
}

template <size_t I, class A, class B, VTL_PAIR_CHECK_IDX(I)>
VTL_INLINE constexpr decltype(auto) get(pair<A, B>& pr)
{
	return pr[tag_v<I>];
}

template <size_t I, class A, class B, VTL_PAIR_CHECK_IDX(I)>
VTL_INLINE constexpr decltype(auto) get(const pair<A, B>& pr)
{
	return pr[tag_v<I>];
}

template <size_t I, class A, class B, VTL_PAIR_CHECK_IDX(I)>
VTL_INLINE constexpr decltype(auto) get(pair<A, B>&& pr)
{
	return vtl::forward<pair<A, B>>(pr)[tag_v<I>];
}

template <class F, class A, class B>
VTL_INLINE constexpr decltype(auto) apply(F&& func, pair<A, B>& pr)
{
	return vtl::forward<F>(func)(pr.first, pr.second);
}

template <class F, class A, class B>

VTL_INLINE constexpr decltype(auto) apply(F&& func, const pair<A, B>& pr)
{
	return vtl::forward<F>(func)(pr.first, pr.second);
}

template <class F, class A, class B>
VTL_INLINE constexpr decltype(auto) apply(F&& func, pair<A, B>&& pr)
{
	return vtl::forward<F>(func)(vtl::forward<pair<A, B>>(pr).first, vtl::forward<pair<A, B>>(pr).second);
}


template <class A, class B>
VTL_INLINE void swap(pair<A, B>& a, pair<A, B>& b) noexcept(pair<A, B>::nothrow_swappable)
{
	a.swap(b);
}

template <class A, class B>
struct tuple_size<pair<A, B>> : integral_constant<size_t, 2> {};

template <class A, class B>
struct tuple_element<0, pair<A, B>>
{
	using type = A;
};

template <class A, class B>
struct tuple_element<1, pair<A, B>>
{
	using type = B;
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// TRIO ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

template <class A, class B, class C>
VTL_NODISCARD constexpr trio<unwrap_reference_t<A>, unwrap_reference_t<B>, unwrap_reference_t<C>> make_trio(A&& first, B&& second, C&& third)
noexcept(is_nothrow_constructible_v<unwrap_reference_t<A>, A> && is_nothrow_constructible_v<unwrap_reference_t<B>, B> && is_nothrow_constructible_v<unwrap_reference_t<C>, C>)
{
	return trio<unwrap_reference_t<A>, unwrap_reference_t<B>, unwrap_reference_t<C>> { vtl::forward<A>(first), vtl::forward<B>(second), vtl::forward<C>(third) };
}


template <size_t I, class A, class B, class C, VTL_TRIO_CHECK_IDX(I)>
VTL_INLINE constexpr decltype(auto) get(trio<A, B, C>& tr)
{
	return tr[tag_v<I>];
}

template <size_t I, class A, class B, class C, VTL_TRIO_CHECK_IDX(I)>
VTL_INLINE constexpr decltype(auto) get(const trio<A, B, C>& tr)
{
	return tr[tag_v<I>];
}

template <size_t I, class A, class B, class C, VTL_TRIO_CHECK_IDX(I)>
VTL_INLINE constexpr decltype(auto) get(trio<A, B, C>&& tr)
{
	return vtl::forward<trio<A, B, C>>(tr)[tag_v<I>];
}

template <class F, class A, class B, class C>
VTL_INLINE constexpr decltype(auto) apply(F&& func, trio<A, B, C>& tr)
{
	return vtl::forward<F>(func)(tr.first, tr.second, tr.third);
}

template <class F, class A, class B, class C>

VTL_INLINE constexpr decltype(auto) apply(F&& func, const trio<A, B, C>& tr)
{
	return vtl::forward<F>(func)(tr.first, tr.second, tr.third);
}

template <class F, class A, class B, class C>
VTL_INLINE constexpr decltype(auto) apply(F&& func, trio<A, B, C>&& tr)
{
	return vtl::forward<F>(func)(vtl::forward<trio<A, B, C>>(tr).first, vtl::forward<trio<A, B, C>>(tr).second, vtl::forward<trio<A, B, C>>(tr).third);
}

template <class A, class B, class C>
VTL_INLINE void swap(trio<A, B, C>& a, trio<A, B, C>& b) noexcept(trio<A, B, C>::nothrow_swappable)
{
	a.swap(b);
}

template <class A, class B, class C>
struct tuple_size<trio<A, B, C>> : integral_constant<size_t, 3> {};

template <class A, class B, class C>
struct tuple_element<0, trio<A, B, C>>
{
	using type = A;
};

template <class A, class B, class C>
struct tuple_element<1, trio<A, B, C>>
{
	using type = B;
};

template <class A, class B, class C>
struct tuple_element<2, trio<A, B, C>>
{
	using type = C;
};

template <size_t I1, size_t I2, class A, class B, class C, VTL_TRIO_CHECK_2_IDXS(I1, I2)>
VTL_INLINE constexpr decltype(auto) pair_from_trio(trio<A, B, C>&& tr)
{
	return vtl::make_pair<typename tuple_element<I1, trio<A, B, C>>::type, typename tuple_element<I2, trio<A, B, C>>::type>(vtl::forward<trio<A, B, C>>(tr)[tag_v<I1>], vtl::forward<trio<A, B, C>>(tr)[tag_v<I2>]);
}

template <size_t I1, size_t I2, size_t I3, class A, class B, VTL_PAIR_CHECK_3_IDXS(I1, I2, I3)>
VTL_INLINE constexpr decltype(auto) trio_from_pair(pair<A, B>& pr)
{
	return vtl::make_trio<typename tuple_element<I1, pair<A, B>>::type, typename tuple_element<I2, pair<A, B>>::type, typename tuple_element<I3, pair<A, B>>::type>(vtl::forward<pair<A, B>>(pr)[tag_v<I1>], vtl::forward<pair<A, B>>(pr)[tag_v<I2>], vtl::forward<pair<A, B>>(pr)[tag_v<I3>]);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#undef VTL_PAIR_CHECK_IDX
#undef VTL_TRIO_CHECK_IDX
#undef VTL_PAIR_CHECK_3_IDXS
#undef VTL_TRIO_CHECK_2_IDXS

_VTL_END

#pragma warning(pop)

#endif // !_VITAL_PAIR_