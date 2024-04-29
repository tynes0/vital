#pragma once
#ifndef _VITAL_CONCEPTS_
#define _VITAL_CONCEPTS_

#include "core.h"
#include "type_traits.h"

#include <type_traits>

#if !_VTL_TEST_CPP_FT(concepts)
_EMIT_VTL_WARNING(VTL0005, "The contents of vital concept library are available only with C++20 concept support.");
#else // !_VTL_TEST_CPP_FT(concepts)

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

template <class T>
concept arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <class T, class U>
concept same_as = is_same_v<T, U> && is_same_v<U, T>;

template <class T, class U>
concept other_than = !is_same_v<std::decay_t<T>, std::decay_t<U>>;

template <class T>
concept stateless = is_empty_v<std::decay_t<T>>;

template <class U, class T>
concept assignable_to = requires(U u, T t)
{
    t = u;
};

template <class T>
concept ordered = requires(T const& t)
{
    { t <=> t };
};

template <class T, class U>
concept ordered_with = requires(T const& t, U const& u)
{
    { t <=> u };
};

template <class T>
concept equality_comparable = requires(T const& t)
{
    { t == t } -> same_as<bool>;
};

template <class T, class U>
concept equality_comparable_with = requires(T const& t, U const& u)
{
    { t == u } -> same_as<bool>;
};

template <class T>
concept not_equality_comparable = requires(T const& t)
{
    { t != t } -> same_as<bool>;
};

template <class T>
concept equality_and_not_equality_compareable = equality_comparable<T> && not_equality_comparable<T>;

template <class T, class U>
concept not_equality_comparable_with = requires(T const& t, U const& u)
{
    { t != u } -> same_as<bool>;
};

template <class T>
concept less_then_compareable = equality_comparable<T> && requires(T const& t)
{
    { t < t } -> same_as<bool>;
};

template <class T, class U>
concept partial_comparable_with = equality_comparable_with<T, U> && requires(T const& t, U const& u)
{
    { t < u } -> same_as<bool>;
    { t > u } -> same_as<bool>;
};

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // _VTL_HAS_CPP_VERSION(20)

#endif // !_VITAL_CONCEPTS_