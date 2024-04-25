#pragma once
#ifndef _VITAL_ARITHMETIC_ARRAY_
#define _VITAL_ARITHMETIC_ARRAY_

#include "core.h"

#if !_VTL_HAS_CPP_VERSION(20)
_EMIT_VTL_WARNING(VTL0004, "The contents of vtl::arithmetic_array are available only with C++20 or later.");
#else //_VTL_HAS_CPP_VERSION(20)

#include "log.h"
#include "type_traits.h"
#include "utility.h"
#include "iterator.h"
#include "concepts.h"
#include "bitset.h"

// arithmetic array uses algorithms
#include "algorithms.h"

#include <stdexcept>
#include <cstring>

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

template <class _Ty>
VTL_INLINE constexpr bool is_bitwiseable_v = vtl::is_any_of_v<_Ty, int, char, short, long, unsigned int, unsigned char, unsigned short, unsigned long>;

template <class _Ty>
struct is_bitwiseable : bool_constant<is_bitwiseable_v<_Ty>> {};

template <class _Ty>
concept bitwiseable = is_bitwiseable_v<_Ty>;

template <class _Ty>
class arithmetic_array_iterator : public iterator_base<_Ty>
{
public:
	using m_base		= iterator_base<_Ty>;
    using value_type	= _Ty;
    using pointer		= _Ty*;
    using reference		= _Ty&;
    using size_type		= typename m_base::size_type;
    using diff_type		= typename m_base::diff_type;

    template <class _Ty>
    friend class const_arithmetic_array_iterator;

	VTL_CONSTEXPR17 arithmetic_array_iterator(pointer ptr = nullptr, size_type offset = 0) : m_ptr(ptr), m_offset(offset) {}

	VTL_CONSTEXPR17 arithmetic_array_iterator(const arithmetic_array_iterator& other) : m_ptr(other.m_ptr), m_offset(other.m_offset) {}

	VTL_CONSTEXPR17 ~arithmetic_array_iterator() {}

	VTL_CONSTEXPR17 reference operator*() const
    {
        return *(m_ptr + m_offset);
    }

	VTL_CONSTEXPR17 arithmetic_array_iterator& operator++()
    {
        ++m_offset;
        return *this;
    }

	VTL_CONSTEXPR17 arithmetic_array_iterator operator++(int)
    {
        arithmetic_array_iterator temp(*this);
        ++(*this);
        return temp;
    }

	VTL_CONSTEXPR17 arithmetic_array_iterator& operator--()
    {
        --m_offset;
        return *this;
    }

	VTL_CONSTEXPR17 arithmetic_array_iterator operator--(int)
    {
        arithmetic_array_iterator temp(*this);
        --(*this);
        return temp;
    }

	VTL_CONSTEXPR17 bool operator==(const arithmetic_array_iterator& other)
    {
        return ((m_ptr + m_offset) == (other.m_ptr + other.m_offset));
    }

	VTL_CONSTEXPR17 bool operator==(arithmetic_array_iterator&& other)
    {
        return ((m_ptr + m_offset) == (other.m_ptr + other.m_offset));
    }

	VTL_CONSTEXPR17 bool operator!=(const arithmetic_array_iterator& other)
    {
        return !(this->operator==(move(other)));
    }

	VTL_CONSTEXPR17 arithmetic_array_iterator operator+(size_type n) const
    {
        return arithmetic_array_iterator(m_ptr, m_offset + n);
    }

	VTL_CONSTEXPR17 size_type operator+(const arithmetic_array_iterator& other) const
    {
        pointer other_ptr = other.m_ptr + other.m_offset;
        pointer ptr = m_ptr + m_offset;
        return ptr + other_ptr;
    }

	VTL_CONSTEXPR17 arithmetic_array_iterator operator-(size_type n) const
    {
        return arithmetic_array_iterator(m_ptr, m_offset - n);
    }

	VTL_CONSTEXPR17 size_type operator-(const arithmetic_array_iterator& other) const
    {
        pointer other_ptr = other.m_ptr + other.m_offset;
        pointer ptr = m_ptr + m_offset;
        return ptr - other_ptr;
    }

    constexpr void reset(pointer ptr = nullptr, size_type offset = 0) noexcept
    {
        m_ptr = ptr;
        m_offset = offset;
    }

    constexpr pointer unwrapped()
    {
        return (m_ptr + m_offset);
    }

    constexpr const pointer unwrapped() const
    {
        return (m_ptr + m_offset);
    }

private:
    pointer m_ptr;
    size_type m_offset;
};

template <class _Ty>
class const_arithmetic_array_iterator : public iterator_base<const _Ty>
{
public:
    using m_base		= iterator_base<const _Ty>;
    using value_type	= const _Ty;
    using pointer		= const _Ty* const;
    using reference		= const _Ty&;
    using size_type		= typename m_base::size_type;
    using diff_type		= typename m_base::diff_type;


	VTL_CONSTEXPR17 const_arithmetic_array_iterator(pointer ptr = nullptr, size_type offset = 0)
        : m_ptr(ptr), m_offset(offset)
    {}

	VTL_CONSTEXPR17 const_arithmetic_array_iterator(const arithmetic_array_iterator<_Ty>& other)
        : m_ptr(other.m_ptr), m_offset(other.m_offset)
    {}

	VTL_CONSTEXPR17 ~const_arithmetic_array_iterator() {}

	VTL_CONSTEXPR17 reference operator*() const
    {
        return *(m_ptr + m_offset);
    }

	VTL_CONSTEXPR17 const_arithmetic_array_iterator& operator++()
    {
        ++m_offset;
        return *this;
    }

	VTL_CONSTEXPR17 const_arithmetic_array_iterator operator++(int)
    {
        const_arithmetic_array_iterator temp(*this);
        ++(*this);
        return temp;
    }

	VTL_CONSTEXPR17 const_arithmetic_array_iterator& operator--()
    {
        --m_offset;
        return *this;
    }

	VTL_CONSTEXPR17 const_arithmetic_array_iterator operator--(int)
    {
        const_arithmetic_array_iterator temp(*this);
        --(*this);
        return temp;
    }

	VTL_CONSTEXPR17 bool operator==(const const_arithmetic_array_iterator& other) const
    {
        return ((m_ptr + m_offset) == (other.m_ptr + other.m_offset));
    }

	VTL_CONSTEXPR17 bool operator==(const_arithmetic_array_iterator&& other) const
    {
        return ((m_ptr + m_offset) == (other.m_ptr + other.m_offset));
    }

	VTL_CONSTEXPR17 bool operator!=(const const_arithmetic_array_iterator& other) const
    {
        return !(this->operator==(move(other)));
    }

	VTL_CONSTEXPR17 const_arithmetic_array_iterator operator+(size_type n) const
    {
        return const_arithmetic_array_iterator(m_ptr, m_offset + n);
    }

	VTL_CONSTEXPR17 const_arithmetic_array_iterator operator-(size_type n) const
    {
        return const_arithmetic_array_iterator(m_ptr, m_offset - n);
    }

	VTL_CONSTEXPR17 size_type operator-(const const_arithmetic_array_iterator& other) const
    {
        pointer other_ptr = other.m_ptr + other.m_offset;
        pointer ptr = m_ptr + m_offset;
        return ptr - other_ptr;
    }

    constexpr void reset(pointer ptr = nullptr, size_type offset = 0) noexcept
    {
        m_ptr = ptr;
        m_offset = offset;
    }

    constexpr pointer unwrapped() const
    {
        return (m_ptr + m_offset);
    }

private:
    pointer m_ptr;
    size_type m_offset;
};

template <class _Ty>
struct pointer_traits<arithmetic_array_iterator<_Ty>>
{
	using pointer = arithmetic_array_iterator<_Ty>;
    using element_type = const _Ty;
    using difference_type = ptrdiff_t;

    VTL_NODISCARD static constexpr element_type* to_address(const pointer iter) noexcept
    {
        return iter.unwrapped();
    }
};

template <class _Ty>
struct pointer_traits<const_arithmetic_array_iterator<_Ty>>
{
    using pointer = const_arithmetic_array_iterator<_Ty>;
    using element_type = const _Ty;
    using difference_type = ptrdiff_t;

    VTL_NODISCARD static constexpr element_type* to_address(const pointer iter) noexcept
    {
        return iter.unwrapped();
    }
};

template <class _Ty, size_t _Size>
	requires arithmetic<_Ty>
class arithmetic_array
{
public:
	using value_type				= _Ty;
	using size_type					= size_t;
	using difference_type			= ptrdiff_t;
	using pointer					= _Ty*;
	using const_pointer				= const _Ty*;
	using reference					= _Ty&;
	using const_reference			= const _Ty&;
	using iterator					= arithmetic_array_iterator<_Ty>;
	using const_iterator			= const_arithmetic_array_iterator<_Ty>;
	using reverse_iterator			= vtl::reverse_iterator<arithmetic_array_iterator<_Ty>>;
	using const_reverse_iterator	= vtl::reverse_iterator<const_arithmetic_array_iterator<_Ty>>;

	constexpr reference operator[](size_type index) noexcept
	{
		VTL_ASSERT(index < _Size, "arithmetic_array subscript out of range");
		return *(m_data + index);
	}

	constexpr const_reference operator[](size_type index) const noexcept
	{
		VTL_ASSERT(index < _Size, "arithmetic_array subscript out of range");
		return *(m_data + index);
	}

	constexpr size_type size() const noexcept
	{
		return _Size;
	}

	constexpr size_type max_size() const noexcept
	{
		return _Size;
	}

	constexpr iterator begin() noexcept
	{
		return iterator(m_data);
	}

	constexpr iterator end() noexcept
	{
		return iterator(m_data, _Size);
	}

	constexpr const_iterator begin() const noexcept
	{
		return const_iterator(m_data);
	}

	constexpr const_iterator end() const noexcept
	{
		return const_iterator(m_data, _Size);
	}

	constexpr const_iterator cbegin() const noexcept
	{
		return const_iterator(m_data);
	}

	constexpr const_iterator cend() const noexcept
	{
		return const_iterator(m_data, _Size);
	}

	constexpr reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	constexpr reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}

	constexpr const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}

	constexpr const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	constexpr const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}

	constexpr const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	constexpr pointer data() noexcept
	{
		return m_data;
	}

	constexpr const_pointer data() const noexcept
	{
		return m_data;
	}

	constexpr reference at(size_t index)
	{
		if (index >= _Size) throw_oran();
		return *(m_data + index);
	}

	constexpr const_reference at(size_t index) const
	{
		if (index >= _Size) throw_oran();
		return *(m_data + index);
	}

	constexpr reference front() noexcept
	{
		return m_data[0];
	}

	constexpr const_reference front() const noexcept
	{
		return m_data[0];
	}

	constexpr reference back() noexcept
	{
		return m_data[_Size - 1];
	}

	constexpr const_reference back() const noexcept
	{
		return m_data[_Size - 1];
	}

	constexpr bool empty() const noexcept
	{
		return false;
	}

	constexpr void fill(const_reference _Val) noexcept
	{
		vtl::fill(m_data, m_data + _Size, _Val);
	}

	constexpr void swap(arithmetic_array& right) noexcept
	{
		if (addressof(right) != this)
		{
			vtl::swap(m_data, right.m_data);
		}
	}

	constexpr void copy(const arithmetic_array& other) noexcept
	{
		if (addressof(other) != this)
		{
			::memcpy(m_data, other.m_data, _Size * sizeof(_Ty));
		}
	}

	constexpr pointer unchecked_begin() noexcept
	{
		return m_data;
	}

	constexpr const_pointer unchecked_begin() const noexcept
	{
		return m_data;
	}

	constexpr pointer unchecked_end() noexcept
	{
		return m_data + _Size;
	}

	constexpr const_pointer unchecked_end() const noexcept
	{
		return m_data + _Size;
	}

	// --------------------------------------------------------------
	// -------------------- ASSIGNMENT OPERATORS --------------------
	// --------------------------------------------------------------

	constexpr arithmetic_array& operator=(_Ty value) noexcept
	{
		vtl::fill(m_data, m_data + _Size, value);
	}

	constexpr arithmetic_array& operator=(const arithmetic_array& right) noexcept
	{
		::memcpy(m_data, right.m_data, _Size * sizeof(_Ty));
		return *this;
	}

	constexpr arithmetic_array& operator+=(_Ty value) noexcept
	{
		vtl::for_each(m_data, m_data + _Size, [value](_Ty& data) { data += value; });
		return *this;
	}

	constexpr arithmetic_array& operator+=(const arithmetic_array& right) noexcept
	{
		size_t idx = 0;
		vtl::for_each(m_data, m_data + _Size, [right, &idx](_Ty& data) { data += right[idx++]; });
		return *this;
	}

	constexpr arithmetic_array& operator-=(_Ty value) noexcept
	{
		vtl::for_each(m_data, m_data + _Size, [value](_Ty& data) { data -= value; });
		return *this;
	}

	constexpr arithmetic_array& operator-=(const arithmetic_array& right) noexcept
	{
		size_t idx = 0;
		vtl::for_each(m_data, m_data + _Size, [right, &idx](_Ty& data) { data -= right[idx++]; });
		return *this;
	}

	constexpr arithmetic_array& operator*=(_Ty value) noexcept
	{
		vtl::for_each(m_data, m_data + _Size, [value](_Ty& data) { data *= value; });
		return *this;
	}

	constexpr arithmetic_array& operator*=(const arithmetic_array& right) noexcept
	{
		size_t idx = 0;
		vtl::for_each(m_data, m_data + _Size, [right, &idx](_Ty& data) { data *= right[idx++]; });
		return *this;
	}

	constexpr arithmetic_array& operator/=(_Ty value) noexcept
	{
		vtl::for_each(m_data, m_data + _Size, [value](_Ty& data) { data /= value; });
		return *this;
	}

	constexpr arithmetic_array& operator/=(const arithmetic_array& right) noexcept
	{
		size_t idx = 0;
		vtl::for_each(m_data, m_data + _Size, [right, &idx](_Ty& data) { data /= right[idx++]; });
		return *this;
	}

	constexpr arithmetic_array& operator%=(_Ty value) noexcept
	{
		vtl::for_each(m_data, m_data + _Size, [value](_Ty& data) { data %= value; });
		return *this;
	}

	constexpr arithmetic_array& operator%=(const arithmetic_array& right) noexcept
	{
		size_t idx = 0;
		vtl::for_each(m_data, m_data + _Size, [right, &idx](_Ty& data) { data %= right[idx++]; });
		return *this;
	}

	// --------------------------------------------------------------
	// -------------------- RELATIONAL OPERATORS --------------------
	// --------------------------------------------------------------

	constexpr bool operator==(const arithmetic_array& right) const noexcept
	{
		return vtl::all_of_2range(m_data, right.m_data, _Size, vtl::equal_to<_Ty>{});
	}

	constexpr bool operator!=(const arithmetic_array& right) const noexcept
	{
		return vtl::any_of_2range(m_data, right.m_data, _Size, vtl::not_equal_to<_Ty>{});
	}

	constexpr bitset<_Size> operator>(const arithmetic_array& right) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] > right.m_data[i])
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator>(_Ty value) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] > value)
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator<(const arithmetic_array& right) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] < right.m_data[i])
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator<(_Ty value) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] < value)
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator>=(const arithmetic_array& right) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] >= right.m_data[i])
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator>=(_Ty value) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] >= value)
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator<=(const arithmetic_array& right) const noexcept
	{
		bitset<_Size> result{};
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] <= right.m_data[i])
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator<=(_Ty value) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] <= value)
				result[_Size - i - 1] = true;
		return result;
	}

	// --------------------------------------------------------------
	// ---------------------- LOGICAL OPERATORS ---------------------
	// --------------------------------------------------------------

	constexpr bitset<_Size> operator&&(const arithmetic_array& right) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] && right[i])
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator&&(_Ty value) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] && value)
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator||(const arithmetic_array& right) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] || right[i])
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator||(_Ty value) const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (m_data[i] || value)
				result[_Size - i - 1] = true;
		return result;
	}

	constexpr bitset<_Size> operator!() const noexcept
	{
		bitset<_Size> result{}; // all bits false initialized
		for (size_t i = 0; i < _Size; ++i)
			if (!m_data[i])
				result[_Size - i - 1] = true;
		return result;
	}

	// --------------------------------------------------------------
	// -------------- INCREMENT AND DECREMENT OPERATORS -------------
	// --------------------------------------------------------------

	constexpr arithmetic_array& operator++() noexcept
	{
		vtl::for_each(m_data, m_data + _Size, [](_Ty& data) { ++data; });
		return *this;
	}

	constexpr arithmetic_array operator++(int) noexcept
	{
		arithmetic_array temp{};
		temp.copy(*this);
		this->operator++();
		return temp;
	}

	constexpr arithmetic_array& operator--() noexcept
	{
		vtl::for_each(m_data, m_data + _Size, [](_Ty& data) { --data; });
		return *this;
	}

	constexpr arithmetic_array operator--(int) noexcept
	{
		arithmetic_array temp{};
		temp.copy(*this);
		this->operator--();
		return temp;
	}
	
public:
	_Ty m_data[_Size];
private:
	void throw_oran()
	{
		std::_Xout_of_range("arithmetic_array subscript out of range");
	}
};

template <class _Ty>
	requires arithmetic<_Ty>
class arithmetic_array<_Ty, 0>
{
public:
	using value_type				= _Ty;
	using size_type					= size_t;
	using difference_type			= ptrdiff_t;
	using pointer					= _Ty*;
	using const_pointer				= const _Ty*;
	using reference					= _Ty&;
	using const_reference			= const _Ty&;
	using iterator					= arithmetic_array_iterator<_Ty>;
	using const_iterator			= const_arithmetic_array_iterator<_Ty>;
	using reverse_iterator			= vtl::reverse_iterator<arithmetic_array_iterator<_Ty>>;
	using const_reverse_iterator	= vtl::reverse_iterator<const_arithmetic_array_iterator<_Ty>>;

	constexpr reference operator[](size_type index)
	{
		return *data();
	}

	constexpr const_reference operator[](size_type index) const
	{
		return *data();
	}

	constexpr size_type size() const noexcept
	{
		return 0;
	}

	constexpr size_type max_size() const noexcept
	{
		return 0;
	}

	constexpr iterator begin() noexcept
	{
		return iterator{};
	}

	constexpr iterator end() noexcept
	{
		return iterator{};
	}

	constexpr const_iterator begin() const noexcept
	{
		return const_iterator{};
	}

	constexpr const_iterator end() const noexcept
	{
		return const_iterator{};
	}

	constexpr const_iterator cbegin() const noexcept
	{
		return const_iterator{};
	}

	constexpr const_iterator cend() const noexcept
	{
		return const_iterator{};
	}

	constexpr reverse_iterator rbegin() noexcept
	{
		return reverse_iterator{};
	}

	constexpr reverse_iterator rend() noexcept
	{
		return reverse_iterator{};
	}

	constexpr const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator{};
	}

	constexpr const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator{};
	}

	constexpr const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator{};
	}

	constexpr const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator{};
	}

	constexpr pointer data() noexcept
	{
		return nullptr;
	}

	constexpr const_pointer data() const noexcept
	{
		return nullptr;
	}

	constexpr reference at(size_t index) noexcept
	{
		return *data();
	}

	constexpr const_reference at(size_t index) const noexcept
	{
		return *data();
	}

	constexpr reference front() noexcept
	{
		return *data();
	}

	constexpr const_reference front() const noexcept
	{
		return *data();
	}

	constexpr reference back() noexcept
	{
		return *data();
	}

	constexpr const_reference back() const noexcept
	{
		return *data();
	}

	constexpr bool empty() const noexcept
	{
		return true;
	}

	constexpr void fill(const_reference _Val) noexcept {}

	constexpr void swap(arithmetic_array& other) noexcept {}

	constexpr void copy(const arithmetic_array& other) noexcept {}

	constexpr pointer unchecked_begin() noexcept
	{
		return data();
	}

	constexpr const_pointer unchecked_begin() const noexcept
	{
		return data();
	}

	constexpr pointer unchecked_end() noexcept
	{
		return data();
	}

	constexpr const_pointer unchecked_end() const noexcept
	{
		return data();
	}
};

template <class _Ty, size_t _Size>
constexpr void swap(arithmetic_array<_Ty, _Size>& lhs, arithmetic_array<_Ty, _Size>& rhs)
{
	lhs.swap(rhs);
}

template <class _Ty, size_t _Size>
struct tuple_size<arithmetic_array<_Ty, _Size>> : integral_constant<size_t, _Size> {};

template <size_t _Idx, class _Ty, size_t _Size>
struct tuple_element<_Idx, arithmetic_array<_Ty, _Size>>
{
	static_assert(_Idx < _Size, "arithmetic_array index out of bounds");
	using type = _Ty;
};

_VTL_END

#pragma warning(pop)

#endif // _VTL_HAS_CPP_VERSION(20)
#endif // !_VITAL_ARITHMETIC_ARRAY_