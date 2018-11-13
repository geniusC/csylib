#pragma once

#include "common.h"
#include <type_traits>
#include <functional>
#include <tuple>

namespace csy
{
    #define HAS_MEMBER(member) \
    template<typename T, typename... Args> \
    struct has_member_##member \
    {\
    private:\
        template<typename U> static std::false_type check(...);\
        template<typename U> static auto check(int)-> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());\
    public:\
        enum{value = std::is_same<std::true_type, decltype(check<T>(0))>::value};\
    };

    template<typename T, typename... List>
    struct Contains;

    template<typename T, typename Head, typename... Rst>
    struct Contains<T, Head, Rst...>: std::conditional<std::is_same<T, Head>::value, std::true_type, Contains<T, Rst...>>::type{};

    template<typename T>
    struct Contains<T>:std::false_type{};

    template<size_t arg, size_t... rst>
    struct IntegerMax;

    template<size_t arg>
    struct IntegerMax<arg>:std::integral_constant<size_t, arg>{};

    template<size_t arg1, size_t arg2, size_t... rst>
    struct IntegerMax<arg1, arg2, rst...>:std::integral_constant<size_t, (arg1 >= arg2 ? IntegerMax<arg1, rst...>::value : IntegerMax<arg2, rst...>::value)>{};

    template<typename T, typename... List> 
    struct IndexOf;

    template<typename T, typename Head, typename... Rst> 
    struct IndexOf<T, Head, Rst...> 
    { 
        enum{value = IndexOf<T, Rst...>::value + 1}; 
    }; 

    template<typename T, typename... Rst> 
    struct IndexOf<T, T, Rst...> 
    { 
        enum{value = 0}; 
    }; 

    template<typename T> 
    struct IndexOf<T> 
    { 
        enum{value = -0xFF}; 
    }; 

    template<int index, typename... List> 
    struct At; 

    template<int index, typename Head, typename... Rst> 
    struct At<index, Head, Rst...> 
    {
        using type = typename At<index - 1, Rst...>::type; 
    }; 

    template<typename T, typename... List> 
    struct At<0, T, List...> 
    { 
        using type = T; 
    }; 

    template<typename... List>
    struct AlignmentMax:std::integral_constant<int, IntegerMax<std::alignment_of<List>::value...>::value>{};

    template<typename T>
    struct function_trait;

    template<typename Ret, typename... Args>
    struct function_trait<Ret(Args...)>
    {
        typedef Ret return_type;
        typedef Ret function_type(Args...);
        typedef Ret (*pointer)(Args...);
        using stl_funtion_type = std::function<function_type>;

        enum
        {
            arity = sizeof... (Args),
        };

        template<size_t index, class = typename std::enable_if<index <arity>::type>
        struct args
        {
            using type = typename std::tuple_element<index, std::tuple<Args...>>::type;
        };
    };

    template<typename Ret, typename... Args>
    struct function_trait<Ret(*)(Args...)>:function_trait<Ret(Args...)>{};

    template<typename Ret, typename... Args>
    struct function_trait<std::function<Ret(Args...)>>:function_trait<Ret(Args...)>{}; 

    #define FUNTION_TRAIT(...) \
    template<typename Ret, typename ClassType, typename... Args> \
    struct function_trait<Ret(ClassType::*)(Args...) __VA_ARGS__>:function_trait<Ret(Args...)>{};

    FUNTION_TRAIT()
    FUNTION_TRAIT(const)
    FUNTION_TRAIT(volatile)
    FUNTION_TRAIT(const volatile)
}
