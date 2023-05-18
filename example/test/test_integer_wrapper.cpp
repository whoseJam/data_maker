
#include <iostream>

#include "Data.h"

using std::cout;

void test_integer_wrapper_is_null() {
    IntegerWrapper* a = new IntegerWrapper();
    a->generate();
    a->out();
    Destroy::get()->start_service();
    a->destroy();
    Destroy::get()->stop_service();
}

void test_integer_wrapper_is_int() {
    IntegerWrapper* a = (new IntegerWrapper())->value(5);
    a->generate();
    a->out();
    Destroy::get()->start_service();
    a->destroy();
    Destroy::get()->stop_service();
}

void test_integer_wrapper_is_integer() {
    IntegerWrapper* a = (new IntegerWrapper())->value(integer(1, 10));
    a->generate();
    a->out();
    Destroy::get()->start_service();
    a->destroy();
    Destroy::get()->stop_service();
}

void test_integer_wrapper_copy() {
    IntegerWrapper* a = (new IntegerWrapper())->value(integer(1, 10000));
    IntegerWrapper* b = (new IntegerWrapper(*a));
    a->generate();
    b->generate();
    a->out(); cout << " and ";
    b->out(); cout << " should be diff\n";
    
    Integer* i = integer(1, 10000);
    IntegerWrapper* c = (new IntegerWrapper())->value(i);
    IntegerWrapper* d = (IntegerWrapper*)c->clone();
    c->generate();
    d->generate();
    c->out(); cout << " and ";
    d->out(); cout << " should be the same\n";
}

using Test = std::function<void()>;

Test test_map[] = {
    test_integer_wrapper_is_null,   // 0
    test_integer_wrapper_is_int,    // 1
    test_integer_wrapper_is_integer,// 2
    test_integer_wrapper_copy,      // 3
};

int main() {
    test_map[3]();
    return 0;
}