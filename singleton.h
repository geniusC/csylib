#pragma once

#include <mutex>
#include <memory>

namespace sy
{
template <typename T>
class Singleton
{
public:
    static T& instance()
    {
        static T m_instance;
        return m_instance;
    }
protected:
    Singleton() = default;
};
} //namespace sy

