#pragma once
#ifndef _VITAL_SPAN_
#define _VITAL_SPAN_

#include "core.h"

#if !_VTL_HAS_CPP_VERSION(20)
_EMIT_VTL_WARNING(VTL0003, "The contents of vtl::span are available only with C++20 or later.");
#else //_VTL_HAS_CPP_VERSION(20)

#include "utility.h"
#include "type_traits.h"
#include "iterator.h"

#include <type_traits>
#include <compare>
#include <xutility>

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

inline constexpr size_t dynamic_extent = static_cast<size_t>(-1);

template <class _Ty>
struct span_iterator : public iterator_base<_Ty>
{
    using value_type        = remove_cv_t<_Ty>;
    using difference_type   = ptrdiff_t;
    using size_type         = size_t;
    using pointer           = _Ty*;
    using reference         = _Ty&;

    VTL_NODISCARD constexpr reference operator*() const noexcept
    {
        return *m_ptr;
    }

    VTL_NODISCARD constexpr reference operator->() const noexcept
    {
        return m_ptr;
    }

    constexpr span_iterator operator++() noexcept
    {
        ++m_ptr;
        return *this;
    }

    constexpr span_iterator operator++(int) noexcept
    {
        span_iterator tmp{ *this };
        ++* this;
        return *tmp;
    }

    constexpr span_iterator operator--() noexcept
    {
        --m_ptr;
        return *this;
    }

    constexpr span_iterator operator--(int) noexcept
    {
        span_iterator tmp{ *this };
        --* this;
        return *tmp;
    }

    constexpr span_iterator& operator+=(const difference_type offset) noexcept
    {
        m_ptr += offset;
        return *this;
    }

    VTL_NODISCARD constexpr span_iterator operator+(const difference_type offset) const noexcept
    {
        span_iterator tmp{ *this };
        tmp += offset;
        return tmp;
    }

    VTL_NODISCARD friend constexpr span_iterator operator+(const difference_type offset, span_iterator next) noexcept
    {
        next += offset;
        return next;
    }

    constexpr span_iterator& operator-=(const difference_type offset) noexcept
    {
        VTL_CORE_ASSERT(offset != std::_Min_possible_v<difference_type>, "integer overflow");
        m_ptr -= offset;
        return *this;
    }

    VTL_NODISCARD constexpr span_iterator operator-(const difference_type offset) const noexcept
    {
        span_iterator tmp{ *this };
        tmp -= offset;
        return tmp;
    }

    VTL_NODISCARD constexpr difference_type operator-(const span_iterator& _Right) const noexcept
    {
        return m_ptr - _Right.m_ptr;
    }

    VTL_NODISCARD constexpr reference operator[](const difference_type offset) const noexcept
    {
        return DREF(*this + offset);
    }

    VTL_NODISCARD constexpr bool operator==(const span_iterator& _Right) const noexcept
    {
        return m_ptr == _Right.m_ptr;
    }

    VTL_NODISCARD constexpr std::strong_ordering operator<=>(const span_iterator& _Right) const noexcept
    {
        return m_ptr <=> _Right.m_ptr;
    }

    VTL_NODISCARD constexpr pointer unwrapped() const noexcept
    {
        return m_ptr;
    }

    constexpr void reset(const pointer it) noexcept
    {
        m_ptr = it;
    }

    pointer m_ptr;
};

template <class _Ty>
struct pointer_traits<span_iterator<_Ty>>
{
    using pointer           = span_iterator<_Ty>;
    using element_type      = _Ty;
    using difference_type   = ptrdiff_t;

    VTL_NODISCARD static constexpr element_type* to_address(const pointer iter) noexcept
    {
        return iter.unwrapped();
    }
};

namespace detail_span
{
    template <class _Ty, size_t _Extent>
    struct span_extent_type 
    {
        using pointer = _Ty*;

        constexpr span_extent_type() noexcept = default;

        constexpr explicit span_extent_type(const pointer data, size_t) noexcept : m_data{ data } {}

        pointer m_data{ nullptr };
        static constexpr size_t m_size = _Extent;
    };

    template <class _Ty>
    struct span_extent_type<_Ty, dynamic_extent> 
    {
        using pointer = _Ty*;

        constexpr span_extent_type() noexcept = default;

        constexpr explicit span_extent_type(const pointer data, const size_t size) noexcept
            : m_data{ data }, m_size{ size } {}

        pointer m_data{ nullptr };
        size_t m_size{ 0 };
    };
}

template <class _Ty, size_t _Size>
class array;

template <class _Ty, size_t _Extent>
class span;

namespace detail_span
{
    template <class>
    struct is_span : false_type {};

    template <class _Ty, size_t _Extent>
    struct is_span<span<_Ty, _Extent>> : true_type {};

    template <class>
    struct is_vtl_array : false_type {};

    template <class _Ty, size_t _Size>
    struct is_vtl_array<array<_Ty, _Size>> : true_type {};

    template <class _Rng, class _Ty>
    struct is_span_convertible_range : bool_constant<is_convertible_v<remove_pointer_t<decltype(vtl::data(vtl::declval<_Rng&>()))>(*)[], _Ty(*)[]>> {};

    template <class, class = void>
    struct has_container_interface : false_type {};

    template <class _Rng>
    struct has_container_interface<_Rng, void_t<decltype(vtl::data(vtl::declval<_Rng&>())), decltype(vtl::size(vtl::declval<_Rng&>()))>> : true_type {};

    template <class _Rng, class _Ty>
    inline constexpr bool is_span_compatible_range = conjunction_v<std::negation<is_array<_Rng>>,std::negation<is_span<remove_const_t<_Rng>>>, 
        std::negation<is_vtl_array<remove_const_t<_Rng>>>, has_container_interface<_Rng>, is_span_convertible_range<_Rng, _Ty>>;
}

template <class _Ty, size_t _Extent = dynamic_extent>
class span : private detail_span::span_extent_type<_Ty, _Extent>
{
private:
    using base = detail_span::span_extent_type<_Ty, _Extent>;
    using base::m_data;
    using base::m_size;
public:
    using element_type      = _Ty;
    using value_type        = remove_cv_t<_Ty>;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;
    using pointer           = _Ty*;
    using const_pointer     = const _Ty*;
    using reference         = _Ty&;
    using const_reference   = const _Ty&;
    using iterator          = span_iterator<_Ty>;
    using reverse_iterator  = reverse_iterator<iterator>;

    static constexpr size_type extent = _Extent;

    constexpr span() noexcept requires (_Extent == 0 || _Extent == dynamic_extent) = default;

    constexpr explicit(_Extent != dynamic_extent) span(pointer ptr, size_type count) noexcept : base(ptr, count) 
    {
        if constexpr (_Extent != dynamic_extent) 
        {
            VTL_CORE_ASSERT(count == _Extent, "Cannot construct span with static extent from range [ptr, ptr + count) as count != extent");
        }
    }

    constexpr explicit(_Extent != dynamic_extent) span(pointer first, pointer last) noexcept // strengthened
        : base(first, static_cast<size_type>(last - first))
    {
        verify_range(first, last);
        if constexpr (_Extent != dynamic_extent)
        {
            VTL_CORE_ASSERT(last - first == _Extent, "Cannot construct span with static extent from range [ptr, ptr + count) as count != extent");
        }
    }

    template <size_t _Size, enable_if_t<_Extent == dynamic_extent || _Extent == _Size, int> = 0>
    constexpr span(element_type(&arr)[_Size]) noexcept : base(arr, _Size) {}

    template <class _OtherTy, size_t _Size, enable_if_t<conjunction_v<bool_constant<_Extent == dynamic_extent || _Extent == _Size>, is_convertible<_OtherTy(*)[], element_type(*)[]>>, int> = 0>
    constexpr span(array<_OtherTy, _Size>& arr) noexcept : base(arr.data(), _Size) {}

    template <class _OtherTy, size_t _Size, enable_if_t<conjunction_v<bool_constant<_Extent == dynamic_extent || _Extent == _Size>, is_convertible<const _OtherTy(*)[], element_type(*)[]>>, int> = 0>
    constexpr span(const array<_OtherTy, _Size>& arr) noexcept : base(arr.data(), _Size) {}

    template <class _Rng, enable_if_t<detail_span::is_span_compatible_range<_Rng, element_type>, int> = 0>
    constexpr explicit(_Extent != dynamic_extent) span(_Rng& _Range)
        : base(vtl::data(_Range), static_cast<size_type>(vtl::size(_Range)))
    {
        if constexpr (_Extent != dynamic_extent) 
        {
            VTL_CORE_ASSERT(_Range.size() == _Extent, "Cannot construct span with static extent from range r as vtl::size(r) != extent");
        }
    }

    template <class _Rng, enable_if_t<detail_span::is_span_compatible_range<const _Rng, element_type>, int> = 0>
    constexpr explicit(_Extent != dynamic_extent) span(const _Rng& _Range)
        : base(vtl::data(_Range), static_cast<size_type>(vtl::size(_Range)))
    {
        if constexpr (_Extent != dynamic_extent)
        {
            VTL_CORE_ASSERT(_Range.size() == _Extent, "Cannot construct span with static extent from range r as vtl::size(r) != extent");
        }
    }

    template <class _OtherTy, size_t _OtherExtent, enable_if_t<conjunction_v<bool_constant<_Extent == dynamic_extent || _OtherExtent == dynamic_extent || _Extent == _OtherExtent>, is_convertible<_OtherTy(*)[], element_type(*)[]>>, int> = 0>
    constexpr explicit(_Extent != dynamic_extent && _OtherExtent == dynamic_extent) span(const span<_OtherTy, _OtherExtent>& other) noexcept
        : base(other.data(), other.size()) 
    {
        if constexpr (_Extent != dynamic_extent) 
        {
            VTL_CORE_ASSERT(other.size() == _Extent, "Cannot construct span with static extent from other span as other.size() != extent");
        }
    }

    template <size_t _Count>
    VTL_NODISCARD constexpr auto first() const noexcept 
    {
        if constexpr (_Extent != dynamic_extent) 
        {
            static_assert(_Count <= _Extent, "Count out of range in vtl::span::first()");
        }
        else
        {
            VTL_CORE_ASSERT(_Count <= m_size, "Count out of range in vtl::span::first()");
        }
        return span<element_type, _Count>{m_data, _Count};
    }

    VTL_NODISCARD constexpr auto first(const size_type count) const noexcept
    {
        VTL_CORE_ASSERT(count <= m_size, "Count out of range in vtl::span::first(count)");
        return span<element_type, dynamic_extent>{m_data, count};
    }

    template <size_t _Count>
    VTL_NODISCARD constexpr auto last() const noexcept
    {
        if constexpr (_Extent != dynamic_extent) 
        {
            static_assert(_Count <= _Extent, "Count out of range in vtl::span::last()");
        }
        else 
        {
            VTL_CORE_ASSERT(_Count <= m_size, "Count out of range in vtl::span::last()");
        }
        return span<element_type, _Count>{m_data + (m_size - _Count), _Count};
    }

    VTL_NODISCARD constexpr auto last(const size_type count) const noexcept
    {
        VTL_CORE_ASSERT(count <= m_size, "Count out of range in vtl::span::last(count)");
        return span<element_type, dynamic_extent>{m_data + (m_size - count), count};
    }

    template <size_t _Offset, size_t _Count = dynamic_extent>
    VTL_NODISCARD constexpr auto subspan() const noexcept 
    {
        if constexpr (_Extent != dynamic_extent)
        {
            static_assert(_Offset <= _Extent, "Offset out of range in vtl::span::subspan()");
            static_assert(_Count == dynamic_extent || _Count <= _Extent - _Offset, "Count out of range in vtl::span::subspan()");
        }
        else 
        {
            VTL_CORE_ASSERT(_Offset <= m_size, "Offset out of range in vtl::span::subspan()");

            if constexpr (_Count != dynamic_extent) {
                VTL_CORE_ASSERT(_Count <= m_size - _Offset, "Count out of range in vtl::span::subspan()");
            }
        }
        using _ReturnType = span<element_type, _Count != dynamic_extent ? _Count : (_Extent != dynamic_extent ? _Extent - _Offset : dynamic_extent)>;
        return _ReturnType{ m_data + _Offset, _Count == dynamic_extent ? m_size - _Offset : _Count };
    }

    VTL_NODISCARD constexpr auto subspan(const size_type offset, const size_type count = dynamic_extent) const noexcept
    {
        VTL_CORE_ASSERT(offset <= m_size, "Offset out of range in vtl::span::subspan(offset, count)");
        VTL_CORE_ASSERT(count == dynamic_extent || count <= m_size - offset, "Count out of range in vtl::span::subspan(offset, count)");
        using _ReturnType = span<element_type, dynamic_extent>;
        return _ReturnType{ m_data + offset, count == dynamic_extent ? m_size - offset : count };
    }

    VTL_NODISCARD constexpr size_t size() const noexcept 
    {
        return m_size;
    }

#pragma warning(push)
#pragma warning(disable : 4127) // conditional expression is constant
    VTL_NODISCARD constexpr size_type size_bytes() const noexcept 
    {
        VTL_CORE_ASSERT(m_size <= dynamic_extent / sizeof(element_type), "size of vtl::span in bytes exceeds std::numeric_limits<size_t>::max()");
        return m_size * sizeof(element_type);
    }
#pragma warning(pop)

    VTL_NODISCARD constexpr bool empty() const noexcept 
    {
        return m_size == 0;
    }

    VTL_NODISCARD constexpr reference operator[](const size_type off) const noexcept
    {
        VTL_CORE_ASSERT(off < m_size, "vtl::span index out of range");
        return m_data[off];
    }

#pragma warning(push)
#pragma warning(disable : 4127) // conditional expression is constant
    VTL_NODISCARD constexpr reference front() const noexcept 
    {
        VTL_CORE_ASSERT(m_size > 0, "front of empty vtl::span");
        return m_data[0];
    }

    VTL_NODISCARD constexpr reference back() const noexcept
    {
        VTL_CORE_ASSERT(m_size > 0, "back of empty vtl::span");
        return m_data[m_size - 1];
    }
#pragma warning(pop)

    VTL_NODISCARD constexpr pointer data() const noexcept 
    {
        return m_data;
    }

    VTL_NODISCARD constexpr iterator begin() const noexcept 
    {
        return { m_data };
    }

    VTL_NODISCARD constexpr iterator end() const noexcept
    {
        const auto _End = m_data + m_size;
        return { _End };
    }

    VTL_NODISCARD constexpr reverse_iterator rbegin() const noexcept 
    {
        return reverse_iterator{ end() };
    }

    VTL_NODISCARD constexpr reverse_iterator rend() const noexcept 
    {
        return reverse_iterator{ begin() };
    }

    VTL_NODISCARD constexpr pointer _Unchecked_begin() const noexcept 
    {
        return m_data;
    }

    VTL_NODISCARD constexpr pointer _Unchecked_end() const noexcept 
    {
        return m_data + m_size;
    }
};

template <class _Ty, size_t _Extent>
span(_Ty(&)[_Extent]) -> span<_Ty, _Extent>;

template <class _Ty, size_t _Size>
span(array<_Ty, _Size>&) -> span<_Ty, _Size>;

template <class _Ty, size_t _Size>
span(const array<_Ty, _Size>&) -> span<const _Ty, _Size>;

template <class _Rng>
span(_Rng&) -> span<typename _Rng::value_type>;

template <class _Rng>
span(const _Rng&) -> span<const typename _Rng::value_type>;

template <class _Ty, size_t _Extent>
VTL_NODISCARD auto as_bytes(span<_Ty, _Extent> sp) noexcept 
{
    using return_type = span<const std::byte, _Extent == dynamic_extent ? dynamic_extent : sizeof(_Ty) * _Extent>;
    return return_type{ reinterpret_cast<const std::byte*>(sp.data()), sp.size_bytes() };
}

template <class _Ty, size_t _Extent, enable_if_t<!std::is_const_v<_Ty>, int> = 0>
VTL_NODISCARD auto as_writable_bytes(span<_Ty, _Extent> sp) noexcept
{
    using return_type = span<std::byte, _Extent == dynamic_extent ? dynamic_extent : sizeof(_Ty) * _Extent>;
    return return_type{ reinterpret_cast<std::byte*>(sp.data()), sp.size_bytes() };
}

_VTL_END

#pragma warning(pop)

#endif //_VTL_HAS_CPP_VERSION(20)

#endif // !_VITAL_SPAN_