#include <functional>

namespace sy
{
#define defer MAKE_DEFER
#define MAKE_DEFER defer_action DEFER_VAR(__LINE__)
#define DEFER_VAR(line) GENERATE_DEFER_VAR(line)
#define GENERATE_DEFER_VAR(line) _defer_action_##line##_

struct defer_action
{

    typedef std::function<void()> defer_call;
    explicit defer_action(defer_call&& callback)
        :m_call(callback)
    {
    }
    defer_action(defer_action &rhs) = delete;
    defer_action& operator=(defer_action &rhs) = delete;
    defer_action() = delete;
    defer_call m_call;
    ~defer_action()
    {
        m_call();
    }

};

}
