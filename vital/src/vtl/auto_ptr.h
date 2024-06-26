#pragma once
#ifndef _VTL_AUTO_PTR_
#define _VTL_AUTO_PTR_

#include "core.h"

#ifdef _VITAL_MSVC
#pragma warning(push)
#pragma warning(disable : _VTL_DISABLED_WARNINGS)
#endif // _VITAL_MSVC

_VTL_START

template <class _Ty>
class auto_ptr;

template <class _Ty>
struct auto_ptr_reference
{
    explicit auto_ptr_reference(_Ty* right) : m_ref(right) {}

    _Ty* m_ref;
};

template <class _Ty>
class auto_ptr 
{
public:
    using element_type = _Ty;

    explicit auto_ptr(_Ty* ptr = nullptr) noexcept : m_ptr(ptr) {}

    auto_ptr(auto_ptr& right) noexcept : m_ptr(right.release()) {}

    auto_ptr(auto_ptr_reference<_Ty> right) noexcept 
    {
        _Ty* ptr = right.m_ref;
        right.m_ref = nullptr;
        m_ptr = ptr;
    }

    template <class _Other>
    auto_ptr(auto_ptr<_Other>& right) noexcept : m_ptr(right.release()) {}

    template <class _Other>
    auto_ptr& operator=(auto_ptr<_Other>& right) noexcept
    {
        reset(right.release());
        return *this;
    }

    auto_ptr& operator=(auto_ptr& right) noexcept
    {
        reset(right.release());
        return *this;
    }

    auto_ptr& operator=(auto_ptr_reference<_Ty> right) noexcept
    {
        _Ty* _Ptr = right.m_ref;
        right.m_ref = 0;
        reset(_Ptr);
        return *this;
    }

    ~auto_ptr() noexcept
    {
        delete m_ptr;
    }

    template <class _Other>
    operator auto_ptr<_Other>() noexcept
    {
        return auto_ptr<_Other>(*this);
    }

    template <class _Other>
    operator auto_ptr_reference<_Other>() noexcept
    {
        _Other* other_ptr = m_ptr;
        auto_ptr_reference<_Other> other_ref(other_ptr);
        m_ptr = nullptr;
        return other_ref;
    }


    VTL_NODISCARD _Ty& operator*() const noexcept
    {
        return *get();
    }

    VTL_NODISCARD _Ty* operator->() const noexcept
    {
        return get();
    }

    VTL_NODISCARD _Ty* get() const noexcept
    {
        return m_ptr;
    }

    _Ty* release() noexcept
    {
        _Ty* temp = m_ptr;
        m_ptr = nullptr;
        return temp;
    }

    void reset(_Ty* ptr = nullptr) noexcept 
    {
        if (ptr != m_ptr)
            delete m_ptr;
        m_ptr = ptr;
    }

private:
    _Ty* m_ptr;
};

_VTL_END

#ifdef _VITAL_MSVC
#pragma warning(pop)
#endif // _VITAL_MSVC

#endif // !_VTL_AUTO_PTR_