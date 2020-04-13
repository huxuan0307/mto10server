#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <string>
using uint = unsigned int;

#define DECL_EQ_DELETE = delete

#define DISABLE_COPY(Class) \
    Class(const Class &) DECL_EQ_DELETE;\
    Class &operator=(const Class &) DECL_EQ_DELETE;

inline std::string quoted(std::string const &str)
{
    return '\"' + str + '\"';
}
// #define new_assert(expression, prompt)  \
// ((expression)                           \
// ? __ASSERT_VOID_CAST (0)                \
// : __assert_fail (__STRING(expression "\ndetail: "##prompt), __FILE__, __LINE__, __ASSERT_FUNCTION))

#endif