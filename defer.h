#include <functional>

namespace sy
{
#define defer MAKE_DEFER
#define MAKE_DEFER defer_action DEFER_VAR
#define DEFER_VAR _defer_action_ ## __LINE__ ## _

struct defer_action
{
    defer_action(std::function<void()>&& callback)
        :m_call(std::forward<std::function<void()>&&>(callback))
    {
    }
    ~defer_action()
    {
        m_call();
    }
    defer_action(defer_action &rhs) = delete;
    defer_action& operator=(defer_action &rhs) = delete;
    defer_action() = delete;
    std::function<void()> m_call;
};
}
