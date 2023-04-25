
#include <stdarg.h>

// #define DEBUG_INTEGER_FLAG
#define UNSET -123
#define EMPTY_STATUS 0
#define RANDOM_FROM_RANGE 1
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

/*
Description:
检测var是否是UNSET，其中var是一个整型变量
Post-Condition:
如果var是UNSET，exit(-1)
*/
#define CHECK_UNSET(class, var) \
    do { \
        if ((var) == UNSET) { \
            cout << #class << "::" << #var << " is UNSET\n"; \
            exit(-1); \
        } \
    } while(0);

/*
Description:
检测var是否是"UNSET"，其中var是一个字符串
Post-Condition:
如果var是"UNSET"，exit(-1)
*/
#define CHECK_STRING_UNSET(class, var) \
    do { \
        if ((var) == "UNSET") { \
            cout << #class << "::" << #var << " is UNSET\n"; \
            exit(-1); \
        } \
    } while(0);

/*
Description:
检测var是否是nullptr，其中var是一个指针
Post-Condition:
如果var是nullptr，exit(-1)
*/
#define CHECK_NULL(class, var) \
    do { \
        if ((var) == nullptr) { \
            cout << #class << "::" << #var << " is NULL\n"; \
            exit(-1); \
        } \
    } while(0);

/*
Description:
通知在解析class的format的时候，没有找到关键字/函数func
*/
#define MESSAGE_NOT_FOUND_IN_FORMAT(class, func) \
    do { \
        cout << #class << "::[" << (func) << "] in format not found\n"; \
        exit(-1); \
    } while(0);

/*
Description:
通知在解析class的format的时候，遇到了一个属性attr，但attr并没有被找到
*/
#define MESSAGE_ATTR_NOT_FOUND_IN_FORMAT(class, attr) \
    do { \
        cout << #class << "::[" << (attr) << "](attribute) in format not found\n"; \
        exit(-1); \
    } while(0);

/*
Description:
通知class的function已经被禁用了，但被错误地调用了
*/
#define MESSAGE_FUNCTION_FORBIDDEN(class, func) \
    do { \
        cout << #class << "::[" << (func) << "](function) is forbidden\n"; \
        exit(-1); \
    } while(0);

/*
Description:
通知class的func期望得到expect个参数，但得到了got个参数
*/
#define MESSAGE_FUNCTION_MISMATCH_ARGS(class, func, got, expect) \
    do { \
        cout << #class << "::[" << (func) << "](function) args dismatch, got " \
             << (got) << ", expect "<< (expect) << "\n"; \
        exit(-1); \
    } while(0);

/*
Description:
通知class的func期望得到exp_l到exp_r个参数，但得到了got个参数
*/
#define MESSAGE_FUNCTION_MISMATCH_ARGS_IN_RANGE(class, func, got, exp_l, exp_r) \
    do { \
        cout << #class << "::[" << (func) << "](function) args dismatch, got " \
             << (got) << ", expect" << (exp_l) << " ~ " << (exp_r) << "\n"; \
        exit(-1); \
    } while(0);

#define MESSAGE_MAX_RETRY_EXCEED(class) \
    do { \
        cout << #class << "::" << "max retry exceed\n"; \
        exit(-1); \
    } while(0);

/*
Description:
检查class的func的参数个数是否合法（恰好expect个参数）
*/
#define CHECK_FUNCTION_ARGS_MATCH(class, func, got, expect) \
    do { \
        if ((got) != (expect)) { \
            MESSAGE_FUNCTION_MISMATCH_ARGS(class, func, got, expect); \
        } \
    } while(0);

/*
Description:
检查class的func的参数个数是否合法（参数个数在exp_l到exp_r之间）
*/
#define CHECK_FUNCTION_ARGS_MATCH_IN_RANGE(class, func, got, exp_l, exp_r) \
    do { \
        if (!((exp_l) <= (got) && (got) <= (exp_r))) { \
            MESSAGE_FUNCTION_MISMATCH_ARGS_IN_RANGE(class, func, got, exp_l, exp_r); \
        } \
    } while(0);

/*
Description:
调用Formatter的parse方法，通用函数（last,nlast...）被放在这个方法中
*/
#define CALL_FORMATTER(spec, n) \
    do { \
        va_list args; \
        va_start(args, n); \
        if (n == 0) Formatter::parse(spec, 0); \
        else if (n == 1) { \
            char* arg0 = va_arg(args, char*); \
            Formatter::parse(spec, 1, arg0); \
        } else if (n == 2) { \
            char* arg0 = va_arg(args, char*); \
            char* arg1 = va_arg(args, char*); \
            Formatter::parse(spec, 2, arg0, arg1); \
        } else if (n == 3) { \
            char* arg0 = va_arg(args, char*); \
            char* arg1 = va_arg(args, char*); \
            char* arg2 = va_arg(args, char*); \
            Formatter::parse(spec, 3, arg0, arg1, arg2); \
        } \
        va_end(args); \
    } while(0);


// 一个由->set引导的update func
#define UF_set(structure, var)              structure->set(var);
// 一个由->value引导的update func
#define UF_value(structure, var)            structure->value(var);
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
#define CL_UPDATE_FUNC(class, func, member, UPDATEFUNC) \
    template<typename T> \
    class* func(T& var) { \
        var->type = Node::VALUE_NODE; \
        UPDATEFUNC(this->member, std::forward<T>(var)); \
        return this; \
    } \
    template<typename T> \
    class* func(T&& var) { \
        var->type = Node::STRUCTURE_NODE; \
        UPDATEFUNC(this->member, std::forward<T>(var)); \
        return this; \
    }

// 以for-loop的形式克隆，适用于用容器存储Node的情况
#define IF_pair_for
// 以非loop的形式克隆，适用于持有单个Node的情况
#define IF_no_loop

#define BEGIN_IF_no_loop(type, member) \
    do { \
        bool by_ref = other.member.second; \
        type* optr = other.member.first;
#define END_IF_no_loop } while(0);
#define BEGIN_IF_pair_for(type ,member) \
    for (pair<type*, bool> iter : other.member) { \
        bool by_ref = iter.second; \
        type* optr = iter.first;
#define END_IF_pair_for }

/*
Description:
对member进行销毁操作，member是单个Node
*/
#define CL_DESTROY(member) \
    do { \
        if (this->member != nullptr) \
            this->member->destroy(); \
    } while(0);

/*
Description:
对member进行销毁操作，member是一个存放Node的可迭代容器
*/
#define CL_DESTROY_ITERABLE(member) \
    do { \
        for (auto iter : this->member) \
            if (iter != nullptr) \
                iter->destroy(); \
    } while(0);

/*
Description:
对member进行生成操作，member是单个Node
*/
#define CL_GENERATE(member) \
    do { \
        if (this->member != nullptr) \
            this->member->generate(re); \
    } while(0);

/*
Description:
对member进行生成操作，member是一个存放Node的可迭代容器
*/
#define CL_GENERATE_ITERABLE(member) \
    do { \
        for (auto iter : this->member) \
            if (iter != nullptr) \
                iter->generate(re); \
    } while(0);

int unknown(int code);