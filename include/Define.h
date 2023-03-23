
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
        cout << #class << "::[" << (str_var) <<"] in format not found\n"; \
        exit(-1); \
    } while(0);

