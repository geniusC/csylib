#pragma once
#include "common.h"
#include <map>
#include <string>
#include <memory>
#include <functional>

namespace sy
{
class ObjectPool
{
public:
    ObjectPool()
    {
    }
    template<typename T>
    void prepare(size_t size)
    {
        auto name = typeid(T).name();
        for(size_t i = 0; i < size; i++)
        {
            T *obj = new T();
            m_objects.emplace(name, obj);
        }
        auto iter = deleters.find(name);
        if(iter == deleters.end())
        {
            deleters[name] = [](void *p){delete static_cast<T*>(p);};
        }
    }
    template<typename T>
    std::shared_ptr<T> get()
    {
        T* obj = nullptr;
        auto typeName = typeid(T).name();
        auto bound = m_objects.find(typeName);
        if(bound == m_objects.end())
        {
            obj = new T();
            auto iter = deleters.find(typeName);
            if(iter == deleters.end())
            {
                deleters[typeName] = [](void *p){delete static_cast<T*>(p);};
            }
        }
        else
        {
            obj = static_cast<T*>(bound->second);
            m_objects.erase(bound);
        }
        return std::shared_ptr<T>(obj, 
                [typeName, this](T *p)
                {
                m_objects.emplace(typeName, p);
                });
    }
    ~ObjectPool()
    {
        release();
    }
private:
    std::multimap<std::string, void*> m_objects;
    std::map<std::string, std::function<void(void*)>> deleters;
    void release()
    {
        for(auto iter = m_objects.begin(); iter != m_objects.end(); iter ++)
        {
            deleters[iter->first](iter->second);
        }
    }
};
}
