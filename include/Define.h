
#include <stdarg.h>

#define UNSET -123
#define RANDOM_FROM_RANGE 1
#define RANDOM_FROM_SET 2

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

#define IF_SPEC_IS_LAST(args, spec, cur_len, len) \
    if ((spec) == "last") { \
        IF_COND_THEN_OUTPUT_STRING(args, \
            (cur_len) == (len) \
        ) \
    }

#define IF_SPEC_IS_NLAST(args, spec, cur_len, len) \
    if ((spec) == "nlast") { \
        IF_COND_THEN_OUTPUT_STRING(args, \
            (cur_len) < (len)\
        ) \
    }
