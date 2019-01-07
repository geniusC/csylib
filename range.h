#pragma once

#include "common.h"
#include <stdexcept>

namespace sy 
{
template<typename T>
class RangeImpl
{
    class Iterator;
public:
    RangeImpl(T begin, T end, T step)
        :m_begin(begin),
        m_end(end),
        m_step(step),
        m_stepEnd((end - begin) / step)
    {
        if(m_begin >= m_end && step > 0)
        {
            throw std::logic_error("end must greater than begin");
        }
        else if(m_begin <= m_end && step < 0)
        {
            throw std::logic_error("begin must greater than end");
        }
        if(m_begin + m_stepEnd * m_step < m_end)
        {
            m_stepEnd++;
        }
    }
    Iterator begin()
    {
        return Iterator(0, *this);
    }
    Iterator end()
    {
        return Iterator(m_stepEnd + 1, *this);
    }
    size_t size() const
    {
        return m_stepEnd;
    }
    T operator[](size_t index)
    {
        if(index > m_stepEnd)
        {
            throw std::out_of_range("index out of range");
        }
        return m_begin + index * m_step;
    }
private:
    T m_begin;
    T m_end;
    T m_step;
    size_t m_stepEnd;

    class Iterator
    {
    public:
        Iterator(size_t cur, RangeImpl &range)
            :m_cur(cur),
            m_range(range)
        {
            m_value = cur * m_range.m_step + m_range.m_begin;
        }
        Iterator& operator++()
        {
            m_cur ++;
            m_value += m_range.m_step;
            return *this;
        }
        Iterator& operator--()
        {
            m_cur --;
            m_value -= m_range.m_step;
            return *this;
        }
        T operator*()
        {
            return m_range[m_cur];
        }
        bool operator==(const Iterator &rhs)
        {
            return rhs.m_cur == m_cur;
        }
        bool operator!=(const Iterator &rhs)
        {
            return rhs.m_cur != m_cur;
        }
    private:
        size_t m_cur;
        RangeImpl& m_range;
        T m_value;
    };

};
template<typename T, typename U>
auto Range(T begin, T end, U step) -> RangeImpl<decltype(begin + step)>
{
    return RangeImpl<decltype(begin + step)>(begin, end, step);
}
    template<typename T>
RangeImpl<T> Range(T begin, T end)
{
    return RangeImpl<T>(begin, end, 1);
}
    template<typename T>
RangeImpl<T> Range(T end)
{
    return RangeImpl<T>(0, end, 1);
}

}

