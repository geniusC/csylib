#pragma once

#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <unistd.h>
#include "trait.h"

namespace sy
{
template<typename... Types>
class Variant
{
    enum
    {
        data_size = IntegerMax<sizeof(Types)...>::value,
        align_size = AlignmentMax<Types...>::value
    };
    using data_t = typename std::aligned_storage<data_size, align_size>::type;
public:
    Variant():m_index(typeid(void))
    {
    }

    ~Variant()
    {
        destroy();
    }
    template<class T, class = typename std::enable_if<Contains<typename std::decay<T>::type, Types...>::value>::type>
    Variant(T &&value): m_index(typeid(void))
    {
        typedef typename std::decay<T>::type U;
        new(&m_data)U(std::forward<U>(value));
        m_index = typeid(U);
    }

    Variant(Variant<Types...> &rhs):m_index(rhs.m_index)
    {
        copy(rhs.m_index, &rhs.m_data, &m_data);
    }

    Variant(Variant<Types...> &&rhs):m_index(rhs.m_index)
    {
        move(rhs.m_index, &rhs.m_data, &m_data);
    }

    template<class T, class = typename std::enable_if<Contains<typename std::decay<T>::type, Types...>::value>::type>
    Variant& operator=(T &&value)
    {
        destroy();
        typedef typename std::decay<T>::type U;
        new(&m_data)U(std::forward<U>(value));
        m_index = typeid(U);
    }

    Variant& operator=(Variant<Types...> &rhs)
    {
        copy(rhs.m_index, &rhs.m_data, &m_data);
        m_index = rhs.m_index;
        return *this;
    }
    Variant& operator=(Variant<Types...> &&rhs)
    {
        move(rhs.m_index, &rhs.m_data, &m_data);
        m_index = rhs.m_index;
        return *this;
    }

    template<typename T>
    bool is()
    {
        return m_index == typeid(T);
    }

    template<typename T>
    bool empty()
    {
        return m_index == typeid(void);
    }
    template<typename T>
    typename std::decay<T>::type& get()
    {
        using U = typename std::decay<T>::type;
        if(is<U>())
        {
            return *reinterpret_cast<U *>(&m_data);
        }
        std::cerr << "cannot cast " << m_index.name() << " to " << typeid(U).name() << std::endl;
        throw std::bad_cast{};
    }

private:

    void destroy()
    {
        [](int...){}((destroy0<Types>(m_index, reinterpret_cast<void *>(&m_data)), 0)...);
    }

    template<typename T> 
    void destroy0(const std::type_index &index, void *buffer)
    {
        if(index == m_index)
        {
            reinterpret_cast<T*>(buffer)->~T();
        }
    }
    void move(const std::type_index& oldIndex, void *oldValue, void* newValue)
    {
        [](int...){}((move0<Types>(oldIndex, oldValue, newValue), 0)...);
    }
    template<typename T>
    void move0(const std::type_index& oldIndex, void *oldValue, void *newValue)
    {
        if(std::type_index(typeid(T)) == oldIndex)
        {
            new (newValue)T(std::move(*reinterpret_cast<T *>(oldValue)));
        }
    }
    void copy(const std::type_index& oldIndex, void *oldValue, void* newValue)
    {
        [](int...){}((copy0<Types>(oldIndex, oldValue, newValue), 0)...);
    }
    template<typename T>
    void copy0(const std::type_index& oldIndex, void *oldValue, void *newValue)
    {
        if(std::type_index(typeid(T)) == oldIndex)
        {
            new (newValue)T(std::move(*reinterpret_cast<T *>(oldValue)));
        }
    }
    data_t m_data;
    std::type_index m_index;
};
} //namespace sy
