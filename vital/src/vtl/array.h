#pragma once
#ifndef _VITAL_ARRAY_
#define _VITAL_ARRAY_

#include "core.h"
#include "log.h"
#include "utility.h"
#include "iterator.h"

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

template <class _Ty>
class array_iterator : public iterator_base<_Ty>
{
public:
    using m_base        = iterator_base<_Ty>;
    using value_type    = _Ty;
    using pointer       = _Ty*;
    using reference     = _Ty&;
    using size_type     = typename m_base::size_type;
    using diff_type     = typename m_base::diff_type;

    template <class _Ty>
    friend class const_array_iterator;

    array_iterator(pointer ptr = nullptr, size_type offset = 0) : m_ptr(ptr), m_offset(offset) {}

    array_iterator(const array_iterator& other) : m_ptr(other.m_ptr), m_offset(other.m_offset) {}

    ~array_iterator() {}

    VTL_CONSTEXPR17 reference operator*() const
    {
        return *(m_ptr + m_offset);
    }

	VTL_CONSTEXPR17 array_iterator& operator++()
    {
        ++m_offset;
        return *this;
    }

	VTL_CONSTEXPR17 array_iterator operator++(int)
    {
        array_iterator temp(*this);
        ++(*this);
        return temp;
    }

	VTL_CONSTEXPR17 array_iterator& operator--()
    {
        --m_offset;
        return *this;
    }

	VTL_CONSTEXPR17 array_iterator operator--(int)
    {
        array_iterator temp(*this);
        --(*this);
        return temp;
    }

    VTL_CONSTEXPR17 bool operator==(const array_iterator& other)
    {
        return ((m_ptr + m_offset) == (other.m_ptr + other.m_offset));
    }

	VTL_CONSTEXPR17 bool operator==(array_iterator&& other)
    {
        return ((m_ptr + m_offset) == (other.m_ptr + other.m_offset));
    }

	VTL_CONSTEXPR17 bool operator!=(const array_iterator& other)
    {
        return !(this->operator==(move(other)));
    }

	VTL_CONSTEXPR17 array_iterator operator+(size_type n) const
    {
        return array_iterator(m_ptr, m_offset + n);
    }

	VTL_CONSTEXPR17 size_type operator+(const array_iterator& other) const
    {
        pointer other_ptr = other.m_ptr + other.m_offset;
        pointer ptr = m_ptr + m_offset;
        return ptr + other_ptr;
    }

	VTL_CONSTEXPR17 array_iterator operator-(size_type n) const
    {
        return array_iterator(m_ptr, m_offset - n);
    }

	VTL_CONSTEXPR17 size_type operator-(const array_iterator& other) const
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
class const_array_iterator : public iterator_base<const _Ty>
{
public:
    using m_base        = iterator_base<const _Ty>;
    using value_type    = const _Ty;
    using pointer       = const _Ty* const;
    using reference     = const _Ty&;
    using size_type     = typename m_base::size_type;
    using diff_type     = typename m_base::diff_type;


	VTL_CONSTEXPR17 const_array_iterator(pointer ptr = nullptr, size_type offset = 0) : m_ptr(ptr), m_offset(offset) {}

	VTL_CONSTEXPR17 const_array_iterator(const array_iterator<_Ty>& other) : m_ptr(other.m_ptr), m_offset(other.m_offset) {}

	VTL_CONSTEXPR17 ~const_array_iterator() {}

	VTL_CONSTEXPR17 reference operator*() const
    {
        return *(m_ptr + m_offset);
    }

	VTL_CONSTEXPR17 const_array_iterator& operator++()
    {
        ++m_offset;
        return *this;
    }

	VTL_CONSTEXPR17 const_array_iterator operator++(int)
    {
        const_array_iterator temp(*this);
        ++(*this);
        return temp;
    }

	VTL_CONSTEXPR17 const_array_iterator& operator--()
    {
        --m_offset;
        return *this;
    }

	VTL_CONSTEXPR17 const_array_iterator operator--(int)
    {
        const_array_iterator temp(*this);
        --(*this);
        return temp;
    }

	VTL_CONSTEXPR17 bool operator==(const const_array_iterator& other) const
	{
		return ((m_ptr + m_offset) == (other.m_ptr + other.m_offset));
	}

	VTL_CONSTEXPR17 bool operator==(const_array_iterator&& other) const
	{
		return ((m_ptr + m_offset) == (other.m_ptr + other.m_offset));
	}

	VTL_CONSTEXPR17 bool operator!=(const const_array_iterator& other) const
	{
		return !(this->operator==(move(other)));
	}

	VTL_CONSTEXPR17 const_array_iterator operator+(size_type n) const
    {
        return const_array_iterator(m_ptr, m_offset + n);
    }

	VTL_CONSTEXPR17 const_array_iterator operator-(size_type n) const
    {
        return const_array_iterator(m_ptr, m_offset - n);
    }

	VTL_CONSTEXPR17 size_type operator-(const const_array_iterator& other) const
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
struct pointer_traits<array_iterator<_Ty>>
{
	using pointer = array_iterator<_Ty>;
	using element_type = const _Ty;
	using difference_type = ptrdiff_t;

	VTL_NODISCARD static constexpr element_type* to_address(const pointer iter) noexcept
	{
		return iter.unwrapped();
	}
};

template <class _Ty>
struct pointer_traits<const_array_iterator<_Ty>>
{
	using pointer = const_array_iterator<_Ty>;
	using element_type = const _Ty;
	using difference_type = ptrdiff_t;

	VTL_NODISCARD static constexpr element_type* to_address(const pointer iter) noexcept
	{
		return iter.unwrapped();
	}
};

template <class _Ty, size_t _Size>
class array
{
public:
	using value_type				= _Ty;
	using size_type					= size_t;
	using difference_type			= ptrdiff_t;
	using pointer					= _Ty*;
	using const_pointer				= const _Ty*;
	using reference					= _Ty&;
	using const_reference			= const _Ty&;
	using iterator					= array_iterator<_Ty>;
	using const_iterator			= const_array_iterator<_Ty>;
	using reverse_iterator			= vtl::reverse_iterator<array_iterator<_Ty>>;
	using const_reverse_iterator	= vtl::reverse_iterator<const_array_iterator<_Ty>>;

	VTL_CONSTEXPR20 reference operator[](size_type index) noexcept
	{
		VTL_ASSERT(index < _Size, "array subscript out of range");
		return *(m_data + index);
	}

	VTL_CONSTEXPR20 const_reference operator[](size_type index) const noexcept
	{
		VTL_ASSERT(index < _Size, "array subscript out of range");
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

	VTL_CONSTEXPR17 iterator begin() noexcept
	{
		return iterator(m_data);
	}

	VTL_CONSTEXPR17 iterator end() noexcept
	{
		return iterator(m_data, _Size);
	}

	VTL_CONSTEXPR17 const_iterator begin() const noexcept
	{
		return const_iterator(m_data);
	}

	VTL_CONSTEXPR17 const_iterator end() const noexcept
	{
		return const_iterator(m_data, _Size);
	}

	VTL_CONSTEXPR17 const_iterator cbegin() const noexcept
	{
		return const_iterator(m_data);
	}

	VTL_CONSTEXPR17 const_iterator cend() const noexcept
	{
		return const_iterator(m_data, _Size);
	}

	VTL_CONSTEXPR17 reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	VTL_CONSTEXPR17 reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}

	VTL_CONSTEXPR17 const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}

	VTL_CONSTEXPR17 const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	VTL_CONSTEXPR17 const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}

	VTL_CONSTEXPR17 const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	VTL_CONSTEXPR17 pointer data() noexcept
	{
		return m_data;
	}

	VTL_CONSTEXPR17 const_pointer data() const noexcept
	{
		return m_data;
	}

	VTL_CONSTEXPR17 reference at(size_t index)
	{
		if (index >= _Size) throw_oran();
		return *(m_data + index);
	}

	constexpr const_reference at(size_t index) const
	{
		if (index >= _Size) throw_oran();
		return *(m_data + index);
	}

	VTL_CONSTEXPR17 reference front() noexcept
	{
		return m_data[0];
	}

	constexpr const_reference front() const noexcept
	{
		return m_data[0];
	}

	VTL_CONSTEXPR17 reference back() noexcept
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

	VTL_CONSTEXPR20 void fill(const_reference _Val) noexcept
	{
		pointer temp_beg = m_data;
		const_pointer temp_end = m_data + _Size;
		if (detail_utility::is_all_bits_zero(_Val))
			detail_utility::fill_zero_memset(temp_beg, _Size);
		else
			for (; temp_beg != temp_end; ++temp_beg)
				DREF(temp_beg) = _Val;
	}

	VTL_CONSTEXPR20 void swap(array& right) noexcept
	{
		if (addressof(right) != this)
		{
			vtl::swap(m_data, right.m_data);
		}
	}

	VTL_CONSTEXPR20 void copy(const array& other) noexcept
	{
		if (addressof(other) != this)
		{
			::memcpy(m_data, other.m_data, _Size * sizeof(_Ty));
		}
	}

	VTL_CONSTEXPR17 pointer unchecked_begin() noexcept
	{
		return m_data;
	}

	VTL_CONSTEXPR17 const_pointer unchecked_begin() const noexcept
	{
		return m_data;
	}

	VTL_CONSTEXPR17 pointer unchecked_end() noexcept
	{
		return m_data + _Size;
	}

	VTL_CONSTEXPR17 const_pointer unchecked_end() const noexcept
	{
		return m_data + _Size;
	}

#if _VTL_TEST_CPP_FT(concepts)
	constexpr bool operator==(const array& right) const noexcept 
		requires equality_and_not_equality_compareable<_Ty>
	{
		pointer temp_beg = m_data;
		const_pointer temp_end = m_data + _Size;
		pointer temp_beg2 = right.m_data;
		for (; temp_beg != temp_end; ++temp_beg, ++temp_beg2)
			if (DREF(temp_beg) != DREF(temp_beg2))
				return false;
		return true;
	}

	constexpr bool operator!=(const array& right) const noexcept
		requires equality_and_not_equality_compareable<_Ty>
	{
		pointer temp_beg = m_data;
		const_pointer temp_end = m_data + _Size;
		pointer temp_beg2 = right.m_data;
		for (; temp_beg != temp_end; ++temp_beg, ++temp_beg2)
			if (DREF(temp_beg) != DREF(temp_beg2))
				return true;
		return false;
	}
#endif // _VTL_TEST_CPP_FT(concepts)

	_Ty m_data[_Size];
private:
	VTL_NORETURN void throw_oran()
	{
		std::_Xout_of_range("array subscript out of range");
	}
};

template <class _Ty>
class array<_Ty, 0>
{
public:
	using value_type				= _Ty;
	using size_type					= size_t;
	using difference_type			= ptrdiff_t;
	using pointer					= _Ty*;
	using const_pointer				= const _Ty*;
	using reference					= _Ty&;
	using const_reference			= const _Ty&;
	using iterator					= array_iterator<_Ty>;
	using const_iterator			= const_array_iterator<_Ty>;
	using reverse_iterator			= vtl::reverse_iterator<array_iterator<_Ty>>;
	using const_reverse_iterator	= vtl::reverse_iterator<const_array_iterator<_Ty>>;

	VTL_CONSTEXPR17 reference operator[](size_type index)
	{
		VTL_ASSERT(false, "vtl::array<T, 0>::operator[]() is invalid");
		return *data();
	}

	VTL_CONSTEXPR17 const_reference operator[](size_type index) const
	{
		VTL_ASSERT(false, "vtl::array<T, 0>::operator[]() is invalid");
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

	VTL_CONSTEXPR17 iterator begin() noexcept
	{
		return iterator{};
	}

	VTL_CONSTEXPR17 iterator end() noexcept
	{
		return iterator{};
	}

	VTL_CONSTEXPR17 const_iterator begin() const noexcept
	{
		return const_iterator{};
	}

	VTL_CONSTEXPR17 const_iterator end() const noexcept
	{
		return const_iterator{};
	}

	VTL_CONSTEXPR17 const_iterator cbegin() const noexcept
	{
		return const_iterator{};
	}

	VTL_CONSTEXPR17 const_iterator cend() const noexcept
	{
		return const_iterator{};
	}

	VTL_CONSTEXPR17 reverse_iterator rbegin() noexcept
	{
		return reverse_iterator{};
	}

	VTL_CONSTEXPR17 reverse_iterator rend() noexcept
	{
		return reverse_iterator{};
	}

	VTL_CONSTEXPR17 const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator{};
	}

	VTL_CONSTEXPR17 const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator{};
	}

	VTL_CONSTEXPR17 const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator{};
	}

	VTL_CONSTEXPR17 const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator{};
	}

	VTL_CONSTEXPR17 pointer data() noexcept
	{
		return nullptr;
	}

	VTL_CONSTEXPR17 const_pointer data() const noexcept
	{
		return nullptr;
	}

	VTL_NORETURN VTL_CONSTEXPR17 reference at(size_t index) noexcept
	{
		throw_oran();
	}

	VTL_NORETURN VTL_CONSTEXPR17 const_reference at(size_t index) const noexcept
	{
		throw_oran();
	}

	VTL_CONSTEXPR17 reference front() noexcept
	{
		VTL_ASSERT(false, "vtl::array<T, 0>::front() is invalid");
		return *data();
	}

	VTL_CONSTEXPR17 const_reference front() const noexcept
	{
		VTL_ASSERT(false, "vtl::array<T, 0>::front() is invalid");
		return *data();
	}

	VTL_CONSTEXPR17 reference back() noexcept
	{
		VTL_ASSERT(false, "vtl::array<T, 0>::back() is invalid");
		return *data();
	}

	VTL_CONSTEXPR17 const_reference back() const noexcept
	{
		VTL_ASSERT(false, "vtl::array<T, 0>::back() is invalid");
		return *data();
	}

	constexpr bool empty() const noexcept
	{
		return true;
	}

	VTL_CONSTEXPR20 void fill(const_reference _Val) noexcept {}

	VTL_CONSTEXPR20 void swap(array& other) noexcept {}

	VTL_CONSTEXPR20 void copy(const array& other) noexcept {}

	VTL_CONSTEXPR17 pointer unchecked_begin() noexcept
	{
		return data();
	}

	VTL_CONSTEXPR17 const_pointer unchecked_begin() const noexcept
	{
		return data();
	}

	VTL_CONSTEXPR17 pointer unchecked_end() noexcept
	{
		return data();
	}

	VTL_CONSTEXPR17 const_pointer unchecked_end() const noexcept
	{
		return data();
	}

	VTL_CONSTEXPR17 bool operator==(const array& right) const noexcept
	{
		return true;
	}

	VTL_CONSTEXPR17 bool operator!=(const array& right) const noexcept
	{
		return false;
	}

private:
	VTL_NORETURN void throw_oran()
	{
		std::_Xout_of_range("array subscript out of range");
	}
};

template <class _Ty, size_t _Size>
constexpr void swap(array<_Ty, _Size>& lhs, array<_Ty, _Size>& rhs)
{
	lhs.swap(rhs);
}

template <class _Ty, size_t _Size>
struct tuple_size<array<_Ty, _Size>> : integral_constant<size_t, _Size> {}; 

template <size_t _Idx, class _Ty, size_t _Size>
struct tuple_element<_Idx, array<_Ty, _Size>> 
{
	static_assert(_Idx < _Size, "array index out of bounds");
	using type = _Ty;
};

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VITAL_ARRAY_