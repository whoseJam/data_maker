#ifndef DEFINE_H
#define DEFINE_H

#include <stdarg.h>

#include "Debug.h"

// #define DEBUG_INTEGER_FLAG
#define UNSET -123
#define EMPTY_STATUS 0
#define RANDOM_FROM_RANGE 1的
#define RANDOM_FROM_SET 2
#define ITERATE_ON_VERTEX 3
#define ITERATE_ON_EDGE 4
#define SPEC_LAST "last"
#define SPEC_NLAST "nlast"
#define SPEC_SELF "x"
#define SPEC_ATTR "a"
#define SPEC_START "s"
#define SPEC_TO "t"
#define SPEC_FA "fa"
#define SPEC_SON "son"
#define SPEC_ITERATE_ON_VERTEX "iv"
#define SPEC_ITERATE_ON_EDGE "ie"
#define MAX_RETRY (10)
#define DEBUG

/*
Description:
调用Formatter的parse方法，通用函数（last,nlast...）被放在这个方法中
*/
#define CALL_FORMATTER(spec, n) \
    do { \
        va_list args; \
        va_start(args, n); \
        if (n == 0) Formatable::parse(spec, 0); \
        else if (n == 1) { \
            char* arg0 = va_arg(args, char*); \
            Formatable::parse(spec, 1, arg0); \
        } else if (n == 2) { \
            char* arg0 = va_arg(args, char*); \
            char* arg1 = va_arg(args, char*); \
            Formatable::parse(spec, 2, arg0, arg1); \
        } else if (n == 3) { \
            char* arg0 = va_arg(args, char*); \
            char* arg1 = va_arg(args, char*); \
            char* arg2 = va_arg(args, char*); \
            Formatable::parse(spec, 3, arg0, arg1, arg2); \
        } \
        va_end(args); \
    } while(0);

template<typename T>
struct get_shared_ptr_inner_type {
    using type = T;
};
template<typename T>
struct get_shared_ptr_inner_type<std::shared_ptr<T>> {
    using type = T;
};
template<typename T>
using shared_ptr_t = typename get_shared_ptr_inner_type<T>::type;

#define CK_equal_to(type)     std::is_same_v<type, shared_ptr_t<std::decay_t<T>>>
#define CK_base_is(type)      std::is_base_of_v<type, shared_ptr_t<std::decay_t<T>>>
#define CK_base_of(type)      std::is_base_of_v<shared_ptr_t<std::decay_t<T>>, type>
#define CK_basic(type)        std::is_same_v<type, std::decay_t<T>>

// 一个由->set引导的update func，structure是一个shared_ptr<*>
#define UF_set(structure, var) \
    if (!structure) structure = std::make_shared<shared_ptr_t<T>>(); \
    structure->set(var);
// 一个由->value引导的update func，structure是一个shared_ptr<*>
#define UF_value(structure, var) \
    if (!structure) structure = std::make_shared<shared_ptr_t<T>>(); \
    structure->value(var);
// 一个向vector的末尾添加的update func
#define UF_append_vector(structure, var)    structure.push_back(var);
// 一个向vector的开头添加元素的update func
#define UF_unshift_vector(structure, var)   structure.insert(structure.begin(), var);
// 一个由add_attribute引导的update func
#define UF_add_attribute(structure, var)    structure->add_attribute(var);
// 一个赋值的update func
#define UF_assign(structure, var)           structure = var;

/*
Description:
依次传入class, func, member, var, UPDATEFUNC
class:当前类名
func:一个函数名，该函数作用是把var赋值给member
UPDATEFUNC:指定如何将var更新member，如果member是一个指针，那么可能使用UF_assign；如果member是一个vector，那么可能使用UF_append_vector
*/
#define CL_UPDATE_FUNC(class, func, member, UPDATEFUNC, checker, additional) \
    template<typename T, typename CHECKER = std::enable_if_t<checker>> \
    std::shared_ptr<class> func(T& var) { \
        CALL(#class, #func); \
        var->implicit_type(Node::VALUE_NODE); \
        UPDATEFUNC(this->member, var); \
        additional; \
        return std::dynamic_pointer_cast<class>(this->shared_from_this()); \
    } \
    template<typename T, typename CHECKER = std::enable_if_t<checker>> \
    std::shared_ptr<class> func(T&& var) { \
        CALL(#class, #func); \
        var->implicit_type(Node::UN_NODE); \
        UPDATEFUNC(this->member, var); \
        additional; \
        return std::dynamic_pointer_cast<class>(this->shared_from_this()); \
    }

#define CL_CLONE(class) \
    shared_ptr<Node> class::clone() { \
        CALL(#class, "clone"); \
        Clone::get()->enter(type == STRUCTURE_NODE); \
        struct CloneGuard { ~CloneGuard() {Clone::get()->exit();}} cg; \
        if (type == UN_NODE || type == STRUCTURE_NODE) \
            return make_shared<class>(*this); \
        else { \
            if (Clone::get()->structure()) { \
                if (Clone::get()->check(this) == nullptr) { \
                    Clone::get()->insert(this, make_shared<class>(*this)); \
                } \
                return Clone::get()->check(this); \
            } \
            return std::dynamic_pointer_cast<Node>(shared_from_this()); \
        } \
    }

int unknown(int code);

#define BUILD(project) \
    project->generate(0); project->out();

#endif