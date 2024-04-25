#pragma once
#ifndef _VITAL_RANGE_
#define _VITAL_RANGE_

#include <type_traits>

#include "log.h"
#include "core.h"
#include "iterator.h"
#include "type_traits.h"
#include "utility.h"

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

// NOTE THAT: range_iterator has NOT a pointer data!
template <class _Ty, class _Sty, enable_if_t<std::is_arithmetic_v<_Ty>&& std::is_arithmetic_v<_Sty>, int> = 0>
class range_iterator : public iterator_base<const _Ty>
{
public:

    VTL_CONSTEXPR17 range_iterator(_Ty value, _Sty step) : m_value(value), m_step(step) {}

    VTL_CONSTEXPR17 range_iterator(const range_iterator& other) : m_value(other.m_value), m_step(other.m_step) {}

    VTL_CONSTEXPR17 range_iterator& operator=(const range_iterator& other)
    {
        m_value = other.m_value;
        m_step = other.m_step;
    }

    VTL_CONSTEXPR17 range_iterator(range_iterator&& other) : m_value(move(other.m_value)), m_step(move(other.m_step)) {}

    VTL_CONSTEXPR17 _Ty operator*() const
    {
        return m_value;
    }

    VTL_CONSTEXPR17 range_iterator& operator++()
    {
        m_value += static_cast<_Ty>(m_step);
        return (*this);
    }

    VTL_CONSTEXPR17 range_iterator operator++(int)
    {
        range_iterator temp = *this;
        m_value += static_cast<_Ty>(m_step);
        return temp;
    }

    VTL_CONSTEXPR17 range_iterator& operator--()
    {
        m_value -= static_cast<_Ty>(m_step);
        return (*this);
    }

    VTL_CONSTEXPR17 range_iterator operator--(int)
    {
        range_iterator temp = *this;
        m_value -= static_cast<_Ty>(m_step);
        return temp;
    }

    VTL_CONSTEXPR17 bool operator==(const range_iterator& right) const
    {
        return ((m_step == right.m_step) && (m_step > 0)) ? (m_value >= right.m_value) : (m_value <= right.m_value);
    }

    VTL_CONSTEXPR17 bool operator!=(const range_iterator& right) const
    {
        return !this->operator==(right);
    }

private:
    _Ty m_value;
    _Sty m_step;
};

template <class _Ty, enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
class range
{
public:
    using range_type    = _Ty;
    using step_type     = conditional_t<std::is_integral_v<_Ty>, long long, _Ty>;
    using size_type     = step_type;
    using iterator      = range_iterator<_Ty, step_type>;

    constexpr range() = default;

    constexpr range(_Ty end)
    {
        reset(0, end);
    }

    constexpr range(_Ty start, _Ty end, step_type step = 1)
    {
        reset(start, end, step);
    }

    constexpr void reset(_Ty start, _Ty end, step_type step = 1) noexcept
    {
#ifdef VITAL_DEBUG
        VTL_ASSERT(step != 0, "step cannot be equal to 0");
#else 
        if (step == 0)
            step = 1;
#endif // VITAL_DEBUG

        m_start = start;
        m_end = end;
        if (start <= end)
            m_step = abs(step);
        else
        {
            if (step > 0)
                m_step = step * (-1);
            else
                m_step = step;
        }
    }

    constexpr void swap(range& right) noexcept 
    {
        if (addressof(right) != this)
        {
            vtl::swap_nt(m_start, right.m_start);
            vtl::swap_nt(m_end, right.m_end);
            vtl::swap_nt(m_step, right.m_step);
        }
    }

    VTL_NODISCARD constexpr iterator begin() noexcept
    {
        return iterator(m_start, m_step);
    }

    VTL_NODISCARD constexpr iterator begin() const noexcept
    {
        return iterator(m_start, m_step);
    }

    VTL_NODISCARD constexpr iterator end() noexcept
    {
        return iterator(m_end, m_step);
    }

    VTL_NODISCARD constexpr iterator end() const noexcept
    {
        return iterator(m_end, m_step);
    }

    VTL_NODISCARD constexpr range step(step_type new_step) const noexcept
    {
        return range(m_start, m_end, new_step);
    }

    VTL_NODISCARD constexpr range reverse() const noexcept
    {
        if (m_step < 0)
        {
            return range(m_end + 1, m_start + 1, -m_step);
        }
        return range(m_end - 1, m_start - 1, -m_step);
    }

    VTL_NODISCARD constexpr _Ty nth_step(size_type n) const noexcept
    {
        return m_start + (m_step * (n - 1));
    }

    VTL_NODISCARD constexpr size_type size() const noexcept
    {
        size_type result = 0;
        if (m_start == m_end) return result;
        if constexpr (std::is_integral_v<_Ty>)
        {
            result = abs((m_end - m_start) / abs(m_step));
            if (m_step != 1) result += 1;
            return result;
        }
        else if constexpr (std::is_floating_point_v<_Ty>)
        {
            result = ceil(abs((m_end - m_start) / m_step));
            return result;
        }
        return result;
    }

private:
    _Ty m_start;
    _Ty m_end;
    step_type m_step;
};

template <class _Ty>
void swap(range<_Ty>& lhs, range<_Ty>& rhs)
{
    lhs.swap(rhs);
}

using crange    = range<char>;
using ucrange   = range<unsigned char>;
using srange    = range<short>;
using usrange   = range<unsigned short>;
using irange    = range<int>;
using uirange   = range<unsigned int>;
using lrange    = range<long>;
using ulrange   = range<unsigned long>;
using llrange   = range<long long>;
using ullrange  = range<unsigned long long>;
using frange    = range<float>;
using drange    = range<double>;
using ldrange   = range<long double>;

_VTL_END

#pragma warning(pop)

#endif // !_VITAL_RANGE_