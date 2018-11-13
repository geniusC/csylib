#include <typeinfo> 
#include <utility>
#include "trait.h"


namespace csy
{
    HAS_MEMBER(before)
    HAS_MEMBER(after)    

    template<typename Fn, typename... Args>
    struct Aspect
    {
    public:
        Aspect(Fn&& fn):m_fn(std::forward<Fn>(fn)){}

        template<typename T>
        typename std::enable_if<has_member_before<T, Args...>::value && has_member_after<T, Args...>::value>::type
        invoke(Args&&... args, T&& aspect)
        {
            aspect.before(std::forward<Args>(args)...);
            m_fn(std::forward<Args>(args)...);
            aspect.after(std::forward<Args>(args)...);
        }

        template<typename T>
        typename std::enable_if<!has_member_before<T, Args...>::value && has_member_after<T, Args...>::value>::type
        invoke(Args&&... args, T&& aspect)
        {
            m_fn(std::forward<Args>(args)...);
            aspect.after(std::forward<Args>(args)...);
        }

        template<typename T>
        typename std::enable_if<has_member_before<T, Args...>::value && !has_member_after<T, Args...>::value>::type
        invoke(Args&&... args, T&& aspect)
        {
            m_fn(std::forward<Args>(args)...);
            aspect.after(std::forward<Args>(args)...);
        }

        template<typename Head, typename... Rst>
        typename std::enable_if<has_member_before<Head, Args...>::value && has_member_after<Head, Args...>::value>::type
        invoke(Args&&... args, Head &&head, Rst&&... rst)
        {
            head.before(std::forward<Args>(args)...);
            invoke(std::forward<Args>(args)..., std::forward<Rst>(rst)...);
            head.after(std::forward<Args>(args)...);
        }
        template<typename Head, typename... Rst>
        typename std::enable_if<!has_member_before<Head, Args...>::value && has_member_after<Head, Args...>::value>::type
        invoke(Args&&... args, Head &&head, Rst&&... rst)
        {
            invoke(std::forward<Args>(args)..., std::forward<Rst>(rst)...);
            head.after(std::forward<Args>(args)...);
        }

        template<typename Head, typename... Rst>
        typename std::enable_if<has_member_before<Head, Args...>::value && !has_member_after<Head, Args...>::value>::type
        invoke(Args&&... args, Head &&head, Rst&&... rst)
        {
            head.before(std::forward<Args>(args)...);
            invoke(std::forward<Args>(args)..., std::forward<Rst>(rst)...);
        }
    private:
        Fn m_fn;
    };

    template<typename... Aop, typename Fn, typename... Args>
    void Invoke(Fn &&func, Args&&... args)
    {
        Aspect<Fn, Args...> aspect(std::forward<Fn>(func));
        aspect.invoke(Aop()..., std::forward<Args>(args)...);
    }
}
