
#include <stdarg.h>

#define UNSET -123
#define RANDOM_FROM_RANGE 1
#define RANDOM_FROM_SET 2
#define SPEC_LAST "last"
#define SPEC_NLAST "nlast"
#define SPEC_SELF "x"
#define SPEC_ATTR "a"
#define SPEC_START "s"
#define SPEC_TO "t"
#define SPEC_FA "fa"

#define CHECK_UNSET(class, var) \
    do { \
        if ((var) == UNSET) { \
            cout << #class << "::" << #var << " is UNSET\n"; \
            exit(-1); \
        } \
    } while(0);

#define CHECK_STRING_UNSET(class, var) \
    do { \
        if ((var) == "UNSET") { \
            cout << #class << "::" << #var << " is UNSET\n"; \
            exit(-1); \
        } \
    } while(0);

#define CHECK_NULL(class, var) \
    do { \
        if ((var) == nullptr) { \
            cout << #class << "::" << #var << " is NULL\n"; \
            exit(-1); \
        } \
    } while(0);

#define MESSAGE_NOT_FOUND_IN_FORMAT(class, str_var) \
    do { \
        cout << #class << "::[" << (str_var) << "] in format not found\n"; \
        exit(-1); \
    } while(0);

#define MESSAGE_ATTR_NOT_FOUND_IN_FORMAT(class, str_var) \
    do { \
        cout << #class << "::[" << (str_var) << "](attribute) in format not found\n"; \
    } while(0);

#define IF_COND_THEN_OUTPUT_STRING(args, cond) \
    if (cond) { \
        char* str = va_arg(args, char*); \
        cout << str; \
    }

#define HANDLE_SPEC_LAST(args, cur_len, len) \
    do { \
        IF_COND_THEN_OUTPUT_STRING(args, \
            (cur_len) == (len) \
        ) \
    } while(0);

#define HANDLE_SPEC_NLAST(args, cur_len, len) \
    do { \
        IF_COND_THEN_OUTPUT_STRING(args, \
            (cur_len) < (len)\
        ) \
    } while(0);

#define HANDLE_SPEC_A(args, attrs) \
    do { \
        bool attr_found = false; \
        string attr_name = va_arg((args), char*); \
        for (Attribute* attr : attrs) { \
            if (attr_name == attr->__get_key()) { \
                attr->out(); \
                attr_found = true; \
                break; \
            } \
        } \
        if (!attr_found) { \
            MESSAGE_ATTR_NOT_FOUND_IN_FORMAT(Vertex, attr_name); \
        } \
    } while(0);
