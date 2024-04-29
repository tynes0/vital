#pragma once
#ifndef _VITAL_REFERENCE_WRAPPER_
#define _VITAL_REFERENCE_WRAPPER_

#include "core.h"

#include "type_traits.h"
#include "invoker.h"

#include <type_traits>

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

namespace detail_refwraper
{
	template <class _Ty>
	void refwrap_ctor_fun(identity_t<_Ty&>) noexcept;
	template <class _Ty>
	void refwrap_ctor_fun(identity_t<_Ty&&>) = delete;

	template <class _Ty, class _Uty, class = void>
	struct refwrap_has_ctor_from : false_type {};

	template <class _Ty, class _Uty>
	struct refwrap_has_ctor_from<_Ty, _Uty, void_t<decltype(refwrap_ctor_fun<_Ty>(vtl::declval<_Uty>()))>> : true_type {};
}

template <class _Ty>
class reference_wrapper
{
public:
	static_assert(std::is_object_v<_Ty> || std::is_function_v<_Ty>, "vtl::reference_wrapper<T> requires T to be an object type or a function type.");

	using type = _Ty;

	template <class _Uty, enable_if_t<conjunction_v<std::negation<is_same<std::_Remove_cvref_t<_Uty>, reference_wrapper>>, detail_refwraper::refwrap_has_ctor_from<_Ty, _Uty>>, int> = 0>
	VTL_CONSTEXPR20 reference_wrapper(_Uty&& _Val) noexcept(noexcept(detail_refwraper::refwrap_ctor_fun<_Ty>(vtl::declval<_Uty>())))
	{
		_Ty& _Ref = static_cast<_Uty&&>(_Val);
		_Ptr = vtl::addressof(_Ref);
	}

	VTL_CONSTEXPR20 operator _Ty& () const noexcept
	{
		return *_Ptr;
	}

	VTL_NODISCARD VTL_CONSTEXPR20 _Ty& get() const noexcept
	{
		return *_Ptr;
	}

private:
	_Ty* _Ptr{};

public:
	template <class... _Types>
	VTL_CONSTEXPR20 auto operator()(_Types&&... _Args) const
		noexcept(noexcept(vtl::invoke(*_Ptr, static_cast<_Types&&>(_Args)...)))-> decltype(vtl::invoke(*_Ptr, static_cast<_Types&&>(_Args)...))
	{
		return vtl::invoke(*_Ptr, static_cast<_Types&&>(_Args)...);
	}
};

#if _HAS_CXX17
template <class _Ty>
reference_wrapper(_Ty&) -> reference_wrapper<_Ty>;
#endif // _HAS_CXX17

template <class _Ty>
VTL_NODISCARD _CONSTEXPR20 reference_wrapper<_Ty> reference(_Ty& val) noexcept
{
	return reference_wrapper<_Ty>(val);
}

template <class _Ty>
void reference(const _Ty&&) = delete;

template <class _Ty>
VTL_NODISCARD _CONSTEXPR20 reference_wrapper<_Ty> reference(reference_wrapper<_Ty> val) noexcept
{
	return val;
}

template <class _Ty>
VTL_NODISCARD _CONSTEXPR20 reference_wrapper<const _Ty> creference(const _Ty& val) noexcept
{
	return reference_wrapper<const _Ty>(val);
}

template <class _Ty>
void creference(const _Ty&&) = delete;

template <class _Ty>
VTL_NODISCARD _CONSTEXPR20 reference_wrapper<const _Ty> creference(reference_wrapper<_Ty> val) noexcept
{
	return val;
}

template <class _Ty>
struct unwrap_reference 
{
	using type = _Ty;
};
template <class _Ty>
struct unwrap_reference<reference_wrapper<_Ty>> 
{
	using type = _Ty&;
};
template <class _Ty>
using unwrap_reference_t = typename unwrap_reference<_Ty>::type;

template <class _Ty>
using unwrap_ref_decay_t = unwrap_reference_t<std::decay_t<_Ty>>;
template <class _Ty>
struct unwrap_ref_decay 
{
	using type = unwrap_ref_decay_t<_Ty>;
};

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_REFERENCE_WRAPPER_