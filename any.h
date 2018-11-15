#pragma once
#include <iostream>
#include <typeindex>

namespace sy
{
    class Any
    {
    public:
        Any():m_ptr(nullptr), m_index(typeid(void))
        {
        }
        template<class T, class = typename std::enable_if<!std::is_same<typename std::decay<T>::type, Any>::value>::type>
        Any(T &&value)
            :m_ptr(new Derived<typename std::decay<T>::type>(std::forward<T>(value))),
            m_index(typeid(typename std::decay<T>::type))
        {
        }
        Any(Any& rhs)
            :m_ptr(rhs.m_ptr->clone()),
            m_index(rhs.m_index)
        {
        }
        template<typename T>
        bool is() const
        {
            return m_index == typeid(T);
        }
        bool null() const
        {
            return m_ptr == nullptr;
        }

        template<typename T>
        T& anycast() const
        {
            if(!is<T>())
            {
                std::cerr << "cannot cast " << m_index.name() << " to "  << typeid(T).name() << std::endl;
                throw std::bad_cast();
            }
            return dynamic_cast<Derived<T>*>(m_ptr)->m_value;
        }

        Any(Any&& rhs)
           :m_ptr(std::move(rhs.m_ptr)),
          m_index(rhs.m_index)
        {
        }
        Any& operator=(const Any& rhs)
        {
            if(m_ptr == rhs.m_ptr)
            {
                return *this;
            }

            if(m_ptr != nullptr)
            {
                delete m_ptr;
            }
            m_ptr = rhs.m_ptr->clone();
            m_index = rhs.m_index;
            return *this;
        }
        ~Any()
        {
            if(m_ptr != nullptr)
            {
                delete m_ptr;
            }
        }
    private:
        struct Base
        {
            virtual Base* clone() = 0;
            virtual ~Base() {}
        };

        template<typename T>
        struct Derived : Base
        {
            Base* clone()
            {
                return new Derived<T>(m_value);
            }
            Derived(const T &value):m_value(value)
            {
            }

            T m_value;
        };
        Base* m_ptr;
        std::type_index m_index;
    };
}
