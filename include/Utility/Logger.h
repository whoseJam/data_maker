#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include "Debug.h"

#define MESSAGE(class, msg) \
    do { \
        std::cout << class << " :: " << msg << "\n"; \
        std::cout << "from : \n"; \
        CallStack::get()->print(); \
        exit(-1); \
    } while(0);

/*
Description:
有两个不能同时被设置的属性被同时设置了
*/
#define CONFLICT(a, b) (a " conflict with " b)

/*
Description:
当配置函数a需要被设置，而没有被设置时
*/
#define NEED(a) (a " need to be called")

/*
Description:
当某些配置需要满足它们之间的一些关系时
*/
#define ENSURE(cond) (cond " should be ensured")

#define FUNC_ARGS_MISMATCH(got, expect_l, expect_r) \
    "we expect num of args in range [" << \
    expect_l << ", " << expect_r << "], but we got " << got

#define IN_RANGE(l, x, r) (l <= x && x <= r)

#define FUNC_NOT_FOUND(func) func << " not found"

#define TYPE_ERROR "type error"

#define INDEX_OUT_OF_BOUNDARY "index out of boundary"
#define INVALID_VERSION(ver) ver << " is a invalid version"
#define UNABLE(ability) "is not " << ability
#define SPLAY_HANDLE_NOT_FOUND "splay handle not found"
#define VERTEX_NOT_FOUND "vertex not found"

#define UNEXCEPT __FILE__ << " " << __LINE__ << " unknown error happend"
#define UNKNOWN UNEXCEPT

#endif