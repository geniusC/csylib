#pragma once

#include <mutex>
#include <memory>

namespace sy
{
    template <typename T>
    class Singleton
    {
    public:
        static std::shared_ptr<T> instance()
        {
            std::call_once(ocg, 
                    []() 
                    {
                        m_instance = std::shared_ptr<T>(new T(), [](T *p){delete p;});
                    });
            return m_instance;
        }
    protected:
        Singleton(){}
        virtual ~Singleton(){}
    private:
        static std::once_flag ocg;
        static std::shared_ptr<T> m_instance;
    };
    template<typename T>
    std::once_flag Singleton<T>::ocg;

    template<typename T>
    std::shared_ptr<T> Singleton<T>::m_instance;
#define DECLARE_SINGLETON(T) friend class Singleton<T>;
}

