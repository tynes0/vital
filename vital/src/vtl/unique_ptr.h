#pragma once
#ifndef _VTL_UNIQUE_PTR_
#define _VTL_UNIQUE_PTR_

#include "core.h"
#include "type_traits.h"
#include "pair.h"
#include "memory.h"
#include "utility.h"
#include "auto_ptr.h"
#include "hash.h"

#include <ostream>

#if _VTL_HAS_CPP_VERSION(20)
#include <compare>
#endif // _VTL_HAS_CPP_VERSION(20)

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

namespace detail_smart_ptr
{
	template <class _Dx2>
	using unique_ptr_enable_default_t = enable_if_t<conjunction_v<negation<is_pointer<_Dx2>>, is_default_constructible<_Dx2>>, int>;
}

template <class _Ty, class _Dx = default_delete<_Ty>>
class unique_ptr 
{
public:
    using pointer = typename get_deleter_pointer_type<_Ty, remove_reference_t<_Dx>>::type;
    using element_type = _Ty;
    using deleter_type = _Dx;

    template <class _Dx2 = _Dx, detail_smart_ptr::unique_ptr_enable_default_t<_Dx2> = 0>
    constexpr unique_ptr() noexcept : m_pair(zero_then_variadic_args_t{}) {}

    template <class _Dx2 = _Dx, detail_smart_ptr::unique_ptr_enable_default_t<_Dx2> = 0>
    constexpr unique_ptr(nullptr_t) noexcept : m_pair(zero_then_variadic_args_t{}) {}

    VTL_CONSTEXPR23 unique_ptr& operator=(nullptr_t) noexcept 
    {
        reset();
        return *this;
    }

    template <class _Dx2 = _Dx, detail_smart_ptr::unique_ptr_enable_default_t<_Dx2> = 0>
    VTL_CONSTEXPR23 explicit unique_ptr(pointer ptr) noexcept : m_pair(zero_then_variadic_args_t{}, ptr) {}

    template <class _Dx2 = _Dx, enable_if_t<is_constructible_v<_Dx2, const _Dx2&>, int> = 0>
    VTL_CONSTEXPR23 unique_ptr(pointer ptr, const _Dx& _Dt) noexcept : m_pair(one_then_variadic_args_t{}, _Dt, ptr) {}

    template <class _Dx2 = _Dx, enable_if_t<conjunction_v<negation<is_reference<_Dx2>>, is_constructible<_Dx2, _Dx2>>, int> = 0>
    VTL_CONSTEXPR23 unique_ptr(pointer ptr, _Dx&& _Dt) noexcept : m_pair(one_then_variadic_args_t{}, vtl::move(_Dt), ptr) {}

    template <class _Dx2 = _Dx, enable_if_t<conjunction_v<is_reference<_Dx2>, is_constructible<_Dx2, remove_reference_t<_Dx2>>>, int> = 0>
    unique_ptr(pointer, remove_reference_t<_Dx>&&) = delete;

    template <class _Dx2 = _Dx, enable_if_t<is_move_constructible_v<_Dx2>, int> = 0>
    VTL_CONSTEXPR23 unique_ptr(unique_ptr&& right) noexcept : m_pair(one_then_variadic_args_t{}, vtl::forward<_Dx>(right.get_deleter()), right.release()) {}

    template <class _Ty2, class _Dx2, enable_if_t<conjunction_v<negation<is_array<_Ty2>>, 
        is_convertible<typename unique_ptr<_Ty2, _Dx2>::pointer, pointer>, 
        conditional_t<is_reference_v<_Dx>, is_same<_Dx2, _Dx>, is_convertible<_Dx2, _Dx>>>, int> = 0>
    VTL_CONSTEXPR23 unique_ptr(unique_ptr<_Ty2, _Dx2>&& right) noexcept : m_pair(one_then_variadic_args_t{}, vtl::forward<_Dx2>(right.get_deleter()), right.release()) {}

    template <class _Ty2,
        enable_if_t<conjunction_v<is_convertible<_Ty2*, _Ty*>, is_same<_Dx, default_delete<_Ty>>>, int> = 0>
    unique_ptr(auto_ptr<_Ty2>&& right) noexcept : m_pair(zero_then_variadic_args_t{}, right.release()) {}

    template <class _Ty2, class _Dx2, enable_if_t<conjunction_v<negation<is_array<_Ty2>>, is_assignable<_Dx&, _Dx2>, is_convertible<typename unique_ptr<_Ty2, _Dx2>::pointer, pointer>>, int> = 0>
    VTL_CONSTEXPR23 unique_ptr& operator=(unique_ptr<_Ty2, _Dx2>&& right) noexcept 
    {
        reset(right.release());
        m_pair.get_first() = vtl::forward<_Dx2>(right.m_pair.get_first());
        return *this;
    }

    template <class _Dx2 = _Dx, enable_if_t<is_move_assignable_v<_Dx2>, int> = 0>
    VTL_CONSTEXPR23 unique_ptr& operator=(unique_ptr&& right) noexcept
    {
        if (this != vtl::addressof(right))
        {
            reset(right.release());
            m_pair.get_first() = vtl::forward<_Dx>(right.m_pair.get_first());
        }
        return *this;
    }

    VTL_CONSTEXPR23 void swap(unique_ptr& right) noexcept 
    {
        swap_adl(m_pair.second, right.m_pair.second);
        swap_adl(m_pair.get_first(), right.m_pair.get_first());
    }

    VTL_CONSTEXPR23 ~unique_ptr() noexcept 
    {
        if (m_pair.second)
            m_pair.get_first()(m_pair.second);
    }

    VTL_NODISCARD VTL_CONSTEXPR23 _Dx& get_deleter() noexcept
    {
        return m_pair.get_first();
    }

    VTL_NODISCARD VTL_CONSTEXPR23 const _Dx& get_deleter() const noexcept
    {
        return m_pair.get_first();
    }

    VTL_NODISCARD VTL_CONSTEXPR23 add_lvalue_reference_t<_Ty> operator*() const noexcept(noexcept(*vtl::declval<pointer>()))
    {
        return *m_pair.second;
    }

    VTL_NODISCARD VTL_CONSTEXPR23 pointer operator->() const noexcept
    {
        return m_pair.second;
    }

    VTL_NODISCARD VTL_CONSTEXPR23 pointer get() const noexcept
    {
        return m_pair.second;
    }

    VTL_CONSTEXPR23 explicit operator bool() const noexcept
    {
        return static_cast<bool>(m_pair.second);
    }

    VTL_CONSTEXPR23 pointer release() noexcept
    {
        return vtl::exchange(m_pair.second, nullptr);
    }

    VTL_CONSTEXPR23 void reset(pointer ptr = nullptr) noexcept
    {
        pointer old = vtl::exchange(m_pair.second, ptr);
        if (old)
            m_pair.get_first()(old);
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

private:
    template <class, class>
    friend class unique_ptr;

    compressed_pair<_Dx, pointer> m_pair;
};

template <class _Ty, class _Dx>
class unique_ptr<_Ty[], _Dx> 
{
public:
    using pointer = typename get_deleter_pointer_type<_Ty, remove_reference_t<_Dx>>::type;
    using element_type = _Ty;
    using deleter_type = _Dx;

    template <class _Dx2 = _Dx, detail_smart_ptr::unique_ptr_enable_default_t<_Dx2> = 0>
    constexpr unique_ptr() noexcept : m_pair(zero_then_variadic_args_t{}) {}

    template <class _Uty, class _Is_nullptr = is_same<_Uty, nullptr_t>>
    using enable_ctor_reset = enable_if_t<is_same_v<_Uty, pointer> || _Is_nullptr::value || (is_same_v<pointer, element_type*> && is_pointer_v<_Uty> && is_convertible_v<remove_pointer_t<_Uty>(*)[], element_type(*)[]>), int>;

    template <class _Uty, class _Dx2 = _Dx, detail_smart_ptr::unique_ptr_enable_default_t<_Dx2> = 0, enable_ctor_reset<_Uty> = 0>
    VTL_CONSTEXPR23 explicit unique_ptr(_Uty ptr) noexcept : m_pair(zero_then_variadic_args_t{}, ptr) {}

    template <class _Uty, class _Dx2 = _Dx, enable_if_t<is_constructible_v<_Dx2, const _Dx2&>, int> = 0, enable_ctor_reset<_Uty> = 0>
    VTL_CONSTEXPR23 unique_ptr(_Uty ptr, const _Dx & _Dt) noexcept : m_pair(one_then_variadic_args_t{}, _Dt, ptr) {}

    template <class _Uty, class _Dx2 = _Dx, enable_if_t<conjunction_v<negation<is_reference<_Dx2>>, is_constructible<_Dx2, _Dx2>>, int> = 0, enable_ctor_reset<_Uty> = 0>
    VTL_CONSTEXPR23 unique_ptr(_Uty ptr, _Dx && _Dt) noexcept : m_pair(one_then_variadic_args_t{}, vtl::move(_Dt), ptr) {}

    template <class _Uty, class _Dx2 = _Dx, enable_if_t<conjunction_v<is_reference<_Dx2>, is_constructible<_Dx2, remove_reference_t<_Dx2>>>, int> = 0>
    unique_ptr(_Uty, remove_reference_t<_Dx>&&) = delete;

    template <class _Dx2 = _Dx, enable_if_t<is_move_constructible_v<_Dx2>, int> = 0>
    VTL_CONSTEXPR23 unique_ptr(unique_ptr&& right) noexcept : m_pair(one_then_variadic_args_t{}, vtl::forward<_Dx>(right.get_deleter()), right.release()) {}

    template <class _Dx2 = _Dx, enable_if_t<is_move_assignable_v<_Dx2>, int> = 0>
    VTL_CONSTEXPR23 unique_ptr& operator=(unique_ptr&& right) noexcept 
    {
        if (this != vtl::addressof(right)) 
        {
            reset(right.release());
            m_pair.get_first() = vtl::move(right.m_pair.get_first());
        }
        return *this;
    }

    template <class _Uty, class _Ex, class _More, class _UP_pointer = typename unique_ptr<_Uty, _Ex>::pointer, class _UP_element_type = typename unique_ptr<_Uty, _Ex>::element_type>
    using enable_conversion = enable_if_t<conjunction_v<is_array<_Uty>, is_same<pointer, element_type*>, is_same<_UP_pointer, _UP_element_type*>, is_convertible<_UP_element_type(*)[], element_type(*)[]>, _More>,int>;

    template <class _Uty, class _Ex, enable_conversion<_Uty, _Ex, conditional_t<is_reference_v<_Dx>, is_same<_Ex, _Dx>, is_convertible<_Ex, _Dx>>> = 0>
    VTL_CONSTEXPR23 unique_ptr(unique_ptr<_Uty, _Ex> && right) noexcept : m_pair(one_then_variadic_args_t{}, vtl::forward<_Ex>(right.get_deleter()), right.release()) {}

    template <class _Uty, class _Ex, enable_conversion<_Uty, _Ex, is_assignable<_Dx&, _Ex>> = 0>
    VTL_CONSTEXPR23 unique_ptr& operator=(unique_ptr<_Uty, _Ex>&& right) noexcept
    {
        reset(right.release());
        m_pair.get_first() = vtl::forward<_Ex>(right.m_pair.get_first());
        return *this;
    }

    template <class _Dx2 = _Dx, detail_smart_ptr::unique_ptr_enable_default_t<_Dx2> = 0>
    constexpr unique_ptr(nullptr_t) noexcept : m_pair(zero_then_variadic_args_t{}) {}

    VTL_CONSTEXPR23 unique_ptr& operator=(nullptr_t) noexcept 
    {
        reset();
        return *this;
    }

    VTL_CONSTEXPR23 void reset(nullptr_t = nullptr) noexcept
    {
        reset(pointer());
    }

    VTL_CONSTEXPR23 void swap(unique_ptr& right) noexcept 
    {
        swap_adl(m_pair.second, right.m_pair.second);
        swap_adl(m_pair.get_first(), right.m_pair.get_first());
    }

    VTL_CONSTEXPR23 ~unique_ptr() noexcept
    {
        if (m_pair.second)
            m_pair.get_first()(m_pair.second);
    }

    VTL_NODISCARD VTL_CONSTEXPR23 _Dx& get_deleter() noexcept 
    {
        return m_pair.get_first();
    }

    VTL_NODISCARD VTL_CONSTEXPR23 const _Dx& get_deleter() const noexcept
    {
        return m_pair.get_first();
    }

    VTL_NODISCARD VTL_CONSTEXPR23 _Ty& operator[](size_t _Idx) const noexcept
    {
        return m_pair.second[_Idx];
    }

    VTL_NODISCARD VTL_CONSTEXPR23 pointer get() const noexcept 
    {
        return m_pair.second;
    }

    VTL_CONSTEXPR23 explicit operator bool() const noexcept 
    {
        return static_cast<bool>(m_pair.second);
    }

    VTL_CONSTEXPR23 pointer release() noexcept 
    {
        return vtl::exchange(m_pair.second, nullptr);
    }

    template <class _Uty, enable_ctor_reset<_Uty, false_type> = 0>
    VTL_CONSTEXPR23 void reset(_Uty ptr) noexcept 
    {
        pointer old = vtl::exchange(m_pair.second, ptr);
        if (old)
            m_pair.get_first()(old);
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

private:
    template <class, class>
    friend class unique_ptr;

    compressed_pair<_Dx, pointer> m_pair;
};

template <class _Ty, class... _Types, enable_if_t<!is_array_v<_Ty>, int> = 0>
VTL_NODISCARD_SMART_PTR VTL_CONSTEXPR23 unique_ptr<_Ty> make_unique(_Types&&... _Args) 
{
    return unique_ptr<_Ty>(new _Ty(vtl::forward<_Types>(_Args)...));
}

template <class _Ty, enable_if_t<is_array_v<_Ty>&& extent_v<_Ty> == 0, int> = 0>
VTL_NODISCARD_SMART_PTR VTL_CONSTEXPR23 unique_ptr<_Ty> make_unique(const size_t size)
{
    using _Elem = remove_extent_t<_Ty>;
    return unique_ptr<_Ty>(new _Elem[size]());
}

template <class _Ty, class... _Types, enable_if_t<extent_v<_Ty> != 0, int> = 0>
void make_unique(_Types&&...) = delete;

template <class _Ty, enable_if_t<!is_array_v<_Ty>, int> = 0>
VTL_NODISCARD_SMART_PTR VTL_CONSTEXPR23 unique_ptr<_Ty> make_unique_for_overwrite() 
{
    return unique_ptr<_Ty>(new _Ty);
}

template <class _Ty, enable_if_t<is_unbounded_array_v<_Ty>, int> = 0>
VTL_NODISCARD_SMART_PTR VTL_CONSTEXPR23 unique_ptr<_Ty> make_unique_for_overwrite(const size_t size) 
{
    using _Elem = remove_extent_t<_Ty>;
    return unique_ptr<_Ty>(new _Elem[size]);
}

template <class _Ty, class... _Types, enable_if_t<is_bounded_array_v<_Ty>, int> = 0>
void make_unique_for_overwrite(_Types&&...) = delete;

template <class _Ty, class _Dx, enable_if_t<is_swappable<_Dx>::value, int> = 0>
VTL_CONSTEXPR23 void swap(unique_ptr<_Ty, _Dx>& left, unique_ptr<_Ty, _Dx>& right) noexcept
{
    left.swap(right);
}

template <class _Ty1, class _Dx1, class _Ty2, class _Dx2>
VTL_NODISCARD VTL_CONSTEXPR23 bool operator==(const unique_ptr<_Ty1, _Dx1>& left, const unique_ptr<_Ty2, _Dx2>& right)
{
    return left.get() == right.get();
}

#if !_VTL_HAS_CPP_VERSION(20)
template <class _Ty1, class _Dx1, class _Ty2, class _Dx2>
VTL_NODISCARD bool operator!=(const unique_ptr<_Ty1, _Dx1>& left, const unique_ptr<_Ty2, _Dx2>& right) 
{
    return !(left == right);
}
#endif // !_VTL_HAS_CPP_VERSION(20)

template <class _Ty1, class _Dx1, class _Ty2, class _Dx2>
VTL_NODISCARD bool operator<(const unique_ptr<_Ty1, _Dx1>& left, const unique_ptr<_Ty2, _Dx2>& right) 
{
    using _Ptr1 = typename unique_ptr<_Ty1, _Dx1>::pointer;
    using _Ptr2 = typename unique_ptr<_Ty2, _Dx2>::pointer;
    using _Common = common_type_t<_Ptr1, _Ptr2>;
    return std::less<_Common>{}(left.get(), right.get());
}

template <class _Ty1, class _Dx1, class _Ty2, class _Dx2>
VTL_NODISCARD bool operator>=(const unique_ptr<_Ty1, _Dx1>& left, const unique_ptr<_Ty2, _Dx2>& right)
{
    return !(left < right);
}

template <class _Ty1, class _Dx1, class _Ty2, class _Dx2>
VTL_NODISCARD bool operator>(const unique_ptr<_Ty1, _Dx1>& left, const unique_ptr<_Ty2, _Dx2>& right)
{
    return right < left;
}

template <class _Ty1, class _Dx1, class _Ty2, class _Dx2>
VTL_NODISCARD bool operator<=(const unique_ptr<_Ty1, _Dx1>& left, const unique_ptr<_Ty2, _Dx2>& right)
{
    return !(right < left);
}

#if _VTL_TEST_CPP_FT(concepts)
template <class _Ty1, class _Dx1, class _Ty2, class _Dx2>
    requires std::three_way_comparable_with<typename unique_ptr<_Ty1, _Dx1>::pointer, typename unique_ptr<_Ty2, _Dx2>::pointer>
VTL_NODISCARD std::compare_three_way_result_t<typename unique_ptr<_Ty1, _Dx1>::pointer, typename unique_ptr<_Ty2, _Dx2>::pointer> operator<=>(const unique_ptr<_Ty1, _Dx1>& left, const unique_ptr<_Ty2, _Dx2>& right) 
{
    return left.get() <=> right.get();
}
#endif //  _VTL_TEST_CPP_FT(concepts)

template <class _Ty, class _Dx>
VTL_NODISCARD _CONSTEXPR23 bool operator==(const unique_ptr<_Ty, _Dx>& left, nullptr_t) noexcept
{
    return !left;
}

#if !_VTL_HAS_CPP_VERSION(20)
template <class _Ty, class _Dx>
VTL_NODISCARD bool operator==(nullptr_t, const unique_ptr<_Ty, _Dx>& right) noexcept 
{
    return !right;
}

template <class _Ty, class _Dx>
VTL_NODISCARD bool operator!=(const unique_ptr<_Ty, _Dx>& left, nullptr_t right) noexcept
{
    return !(left == right);
}

template <class _Ty, class _Dx>
VTL_NODISCARD bool operator!=(nullptr_t left, const unique_ptr<_Ty, _Dx>& right) noexcept 
{
    return !(left == right);
}
#endif // !_VTL_HAS_CPP_VERSION(20)

template <class _Ty, class _Dx>
VTL_NODISCARD VTL_CONSTEXPR23 bool operator<(const unique_ptr<_Ty, _Dx>& left, nullptr_t right) 
{
    using _Ptr = typename unique_ptr<_Ty, _Dx>::pointer;
    return std::less<_Ptr>{}(left.get(), right);
}

template <class _Ty, class _Dx>
VTL_NODISCARD VTL_CONSTEXPR23 bool operator<(nullptr_t left, const unique_ptr<_Ty, _Dx>& right) 
{
    using _Ptr = typename unique_ptr<_Ty, _Dx>::pointer;
    return std::less<_Ptr>{}(left, right.get());
}

template <class _Ty, class _Dx>
VTL_NODISCARD VTL_CONSTEXPR23 bool operator>=(const unique_ptr<_Ty, _Dx>& left, nullptr_t right)
{
    return !(left < right);
}

template <class _Ty, class _Dx>
VTL_NODISCARD VTL_CONSTEXPR23 bool operator>=(nullptr_t left, const unique_ptr<_Ty, _Dx>& right) 
{
    return !(left < right);
}

template <class _Ty, class _Dx>
VTL_NODISCARD VTL_CONSTEXPR23 bool operator>(const unique_ptr<_Ty, _Dx>& left, nullptr_t right) 
{
    return right < left;
}

template <class _Ty, class _Dx>
VTL_NODISCARD VTL_CONSTEXPR23 bool operator>(nullptr_t left, const unique_ptr<_Ty, _Dx>& right)
{
    return right < left;
}

template <class _Ty, class _Dx>
VTL_NODISCARD VTL_CONSTEXPR23 bool operator<=(const unique_ptr<_Ty, _Dx>& left, nullptr_t right) 
{
    return !(right < left);
}

template <class _Ty, class _Dx>
VTL_NODISCARD VTL_CONSTEXPR23 bool operator<=(nullptr_t left, const unique_ptr<_Ty, _Dx>& right) 
{
    return !(right < left);
}

#ifdef _VTL_TEST_CPP_FT(concepts)
template <class _Ty, class _Dx>
    requires std::three_way_comparable<typename unique_ptr<_Ty, _Dx>::pointer>
VTL_NODISCARD VTL_CONSTEXPR23 std::compare_three_way_result_t<typename unique_ptr<_Ty, _Dx>::pointer> operator<=>(const unique_ptr<_Ty, _Dx>& _Left, nullptr_t) 
{
    return _Left.get() <=> static_cast<typename unique_ptr<_Ty, _Dx>::pointer>(nullptr);
}
#endif // _VTL_TEST_CPP_FT(concepts)

namespace detail_smart_ptr
{
    template <class _OutTy, class _PxTy, class = void>
    struct can_stream_unique_ptr : false_type {};
    template <class _OutTy, class _PxTy>
    struct can_stream_unique_ptr<_OutTy, _PxTy, void_t<decltype(_STD declval<_OutTy>() << _STD declval<_PxTy>().get())>> : true_type {};
}

template <class _Elem, class _Traits, class _Yty, class _Dx, enable_if_t<detail_smart_ptr::can_stream_unique_ptr<std::basic_ostream<_Elem, _Traits>&, const unique_ptr<_Yty, _Dx>&>::value, int> = 0>
std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& ostr, const unique_ptr<_Yty, _Dx>& ptr)
{
    ostr << ptr.get();
    return ostr;
}

template <class _Ty, class _Dx>
struct hash<unique_ptr<_Ty, _Dx>> : conditionally_enabled_hash<unique_ptr<_Ty, _Dx>, is_default_constructible_v<hash<typename unique_ptr<_Ty, _Dx>::pointer>>> 
{
    static size_t do_hash(const unique_ptr<_Ty, _Dx>& key_value) noexcept(is_nothrow_hashable<typename unique_ptr<_Ty, _Dx>::pointer>::value)
    {
        return hash<typename unique_ptr<_Ty, _Dx>::pointer>{}(key_value.get());
    }
};

_VTL_END

#pragma warning(pop)

#endif // !_VTL_UNIQUE_PTR_