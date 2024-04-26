#pragma once
#ifndef _VTL_MINIMAL_UNIQUE_OTR_
#define _VTL_MINIMAL_UNIQUE_OTR_

#include "core.h"

#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)

_VTL_START

namespace detail_smart_ptr
{
    template <class _Ty>
    constexpr void minimal_up_swap(_Ty& left, _Ty& right)
    {
        _Ty temp    = left;
        left        = right;
        right       = temp;
    }
}

template<class _Ty>
class minimal_unique_ptr 
{
public:
    constexpr minimal_unique_ptr() noexcept = default;

    constexpr minimal_unique_ptr(vtl::nullptr_t) noexcept {}

    explicit minimal_unique_ptr(_Ty* ptr) noexcept : m_ptr(ptr) {}

    ~minimal_unique_ptr()
    {
        if (m_ptr) 
            delete m_ptr;
    }

    VTL_NODISCARD _Ty& operator*() const noexcept
    {
        return *m_ptr;
    }

    VTL_NODISCARD _Ty* operator->() const noexcept
    {
        return m_ptr;
    }

    VTL_NODISCARD _Ty* get() const noexcept
    {
        return m_ptr;
    }

    explicit operator bool() const noexcept
    {
        return (m_ptr) ? true : false;
    }

    VTL_NODISCARD _Ty* release() noexcept
    {
        _Ty* temp = m_ptr;
        m_ptr = nullptr;
        return temp;
    }

    void reset(_Ty* p = nullptr)
    {
        minimal_unique_ptr tmp(p);
        tmp.swap(*this);
    }

    void swap(minimal_unique_ptr& up) noexcept
    {
        detail_smart_ptr::minimal_up_swap(m_ptr, up.m_ptr);
    }

    minimal_unique_ptr(const minimal_unique_ptr&) = delete;

    minimal_unique_ptr& operator=(const minimal_unique_ptr&) = delete;
private:
    _Ty* m_ptr;
};

template<class _Ty, class _Uty>
VTL_NODISCARD inline bool operator==(const minimal_unique_ptr<_Ty>& lhs, const minimal_unique_ptr<_Uty>& rhs)
{
    return lhs.get() == rhs.get();
}

template<class _Ty>
VTL_NODISCARD inline bool operator==(const minimal_unique_ptr<_Ty>& lhs, vtl::nullptr_t) noexcept
{
    return !lhs;
}

template<class _Ty>
VTL_NODISCARD inline bool operator==(vtl::nullptr_t, const minimal_unique_ptr<_Ty>& rhs) noexcept
{
    return !rhs;
}

template<class _Ty, class _Uty>
VTL_NODISCARD inline bool operator!=(const minimal_unique_ptr<_Ty>& lhs, const minimal_unique_ptr<_Uty>& rhs)
{
    return lhs.get() != rhs.get();
}

template<class _Ty>
VTL_NODISCARD inline bool operator!=(const minimal_unique_ptr<_Ty>& lhs, vtl::nullptr_t) noexcept
{
    return bool(lhs);
}

template<class _Ty>
VTL_NODISCARD inline bool operator!=(vtl::nullptr_t, const minimal_unique_ptr<_Ty>& rhs) noexcept
{
    return bool(rhs);
}

template <class _Ty>
constexpr void swap(minimal_unique_ptr<_Ty>& left, minimal_unique_ptr<_Ty>& right)
{
    left.swap(right);
}

template <class _Ty>
_Ty* get(const minimal_unique_ptr<_Ty>& up)
{
    return up.get();
}

template <class _Ty, class ..._Args>
VTL_NODISCARD_SMART_PTR minimal_unique_ptr<_Ty> make_minimal_unique_ptr(_Args&&... args)
{
    return minimal_unique_ptr<_Ty>(new _Ty(static_cast<_Args&&>(args)...));
}

_VTL_END

#pragma warning(pop)

#endif // !_VTL_MINIMAL_UNIQUE_OTR_