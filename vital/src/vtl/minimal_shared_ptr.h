#pragma once
#ifndef _VTL_MINIMAL_SHARED_PTR_
#define _VTL_MINIMAL_SHARED_PTR_

#include "core.h"

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

namespace detail_smart_ptr
{
    template <class _Ty>
    constexpr void minimal_sp_swap(_Ty& left, _Ty& right)
    {
        _Ty temp    = left;
        left        = right;
        right       = temp;
    }

    class deleter_base 
    {
    public:
        virtual ~deleter_base() = default;
        virtual void operator()(void*) = 0;
    };

    template <class _Ty>
    class deleter : public deleter_base 
    {
    public:
        void operator()(void* p) override 
        {
            delete static_cast<_Ty*>(p);
        }
    };
}

template<class _Ty>
class minimal_shared_ptr 
{
public:
    template<class _Uty>
    friend class minimal_shared_ptr;
    constexpr minimal_shared_ptr() noexcept = default;

    constexpr minimal_shared_ptr(vtl::nullptr_t) noexcept {}

    minimal_shared_ptr(_Ty* ptr) : m_ptr(ptr), m_ref_count(new long{1}), m_deleter(new detail_smart_ptr::deleter<_Ty>()) {}

    template<class _Uty>
    minimal_shared_ptr(_Uty* ptr) : m_ptr(ptr), m_ref_count(new long{1}), m_deleter(new detail_smart_ptr::deleter<_Uty>()) {}

    minimal_shared_ptr(const minimal_shared_ptr& sp) noexcept : m_ptr(sp.m_ptr), m_ref_count(sp.m_ref_count), m_deleter(sp.m_deleter)
    {
        if (m_ptr) ++(*m_ref_count);
    }

    template<class _Uty>
    minimal_shared_ptr(const minimal_shared_ptr<_Uty>& sp) noexcept : m_ptr(sp.m_ptr), m_ref_count(sp.m_ref_count), m_deleter(sp.m_deleter)
    {
        if (m_ptr) ++(*m_ref_count);
    }

    ~minimal_shared_ptr()
    {
        if (m_ptr)
        {
            if (--(*m_ref_count) == 0) 
            {
                delete m_ref_count;
                (*m_deleter)(m_ptr);
                delete m_deleter;
            }
        }
    }

    minimal_shared_ptr& operator=(const minimal_shared_ptr& sp) noexcept
    {
        minimal_shared_ptr tmp(sp);
        tmp.swap(*this);
        return *this;
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

    long use_count() const noexcept
    {
        if (m_ptr)
            return *m_ref_count;
        else
            return 0;
    }

    bool unique() const noexcept
    {
        return (use_count() == 1) ? true : false;
    }

    explicit operator bool() const noexcept
    {
        return (m_ptr) ? true : false;
    }

    void reset() noexcept
    {
        minimal_shared_ptr tmp{};
        tmp.swap(*this);
    }

    template<typename U>
    void reset(U* p)
    {
        minimal_shared_ptr tmp(p);
        tmp.swap(*this);
    }

    void swap(minimal_shared_ptr& sp) noexcept 
    {
        detail_smart_ptr::minimal_sp_swap(m_ptr, sp.m_ptr);
        detail_smart_ptr::minimal_sp_swap(m_ref_count, sp.m_ref_count);
        detail_smart_ptr::minimal_sp_swap(m_deleter, sp.m_deleter);
    }

private:
    _Ty* m_ptr;
    long* m_ref_count;
    detail_smart_ptr::deleter_base* m_deleter;
};


template<class _Ty, class _Uty>
VTL_NODISCARD inline bool operator==(const minimal_shared_ptr<_Ty>& lhs, const minimal_shared_ptr<_Uty>& rhs)
{
    return lhs.get() == rhs.get();
}

template<class _Ty>
VTL_NODISCARD inline bool operator==(const minimal_shared_ptr<_Ty>& lhs, vtl::nullptr_t) noexcept
{
    return !lhs;
}

template<class _Ty>
VTL_NODISCARD inline bool operator==(vtl::nullptr_t, const minimal_shared_ptr<_Ty>& rhs) noexcept
{
    return !rhs;
}

template<class _Ty, class _Uty>
VTL_NODISCARD inline bool operator!=(const minimal_shared_ptr<_Ty>& lhs, const minimal_shared_ptr<_Uty>& rhs)
{
    return lhs.get() != rhs.get();
}

template<class _Ty>
VTL_NODISCARD inline bool operator!=(const minimal_shared_ptr<_Ty>& lhs, vtl::nullptr_t) noexcept
{
    return bool(lhs);
}

template<class _Ty>
VTL_NODISCARD inline bool operator!=(vtl::nullptr_t, const minimal_shared_ptr<_Ty>& rhs) noexcept
{
    return bool(rhs);
}

template <class _Ty>
constexpr void swap(minimal_shared_ptr<_Ty>& left, minimal_shared_ptr<_Ty>& right)
{
    left.swap(right);
}

template <class _Ty>
_Ty* get(const minimal_shared_ptr<_Ty>& up)
{
    return up.get();
}


template <class _Ty, class ..._Args>
VTL_NODISCARD_SMART_PTR minimal_shared_ptr<_Ty> make_minimal_shared_ptr(_Args&&... args)
{
    return minimal_shared_ptr<_Ty>(new _Ty(static_cast<_Args&&>(args)...));
}

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VTL_MINIMAL_SHARED_PTR_