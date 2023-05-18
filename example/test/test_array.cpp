
#include <iostream>

#include "Array.h"

using std::cout;
using std::make_shared;
using std::shared_ptr;
using namespace mk;

void test_array_is_null() {
    auto a = array();
    a->generate(0); a->out();
}

void test_array_have_the_same_length_1() {
    shared_ptr<Integer> l = integer(5, 10);
    auto a = array(l)->fill(integer(1, 10));
    auto b = array(l)->fill(integer(1, 10));
    a->generate(0); 
    b->generate(0);
    a->format("$x $last[\n]")->out();
    b->format("$x $last[\n]")->out();
}

void test_array_have_the_same_length_2() {
    auto l = integer(5, 10);
    auto a = array(l, integer(1, 10));
    auto b = array(l, l);
    a->generate(0); b->generate(0);
    a->format("$x $last[\n]")->out();
    b->format("$x $last[\n]")->out();
}

void test_array_have_the_same_element() {
    auto e = integer(5, 10);
    auto a = array(integer(5, 10), e)->format("$x $last[\n]");
    auto b = array(integer(5, 10), e)->format("$x $last[\n]");
    auto c = array(integer(5, 10), integer(5, 10))->format("$x $last[\n]");
    a->generate(0); a->out();
    b->generate(0); b->out();
    c->generate(0); c->out();
}

void test_nested_array_1() {
    auto a = array(2);
    auto b = array(5, integer(2, 5))->format("$x $last[\n]");
    a->fill(b);
    a->generate(0);
    a->format("$x")->out();
}

void test_nested_array_2() {
    auto l = integer(3, 10);
    auto a = make_shared<Array>()->length(2)->fill(
        make_shared<Array>()
        ->length(l)
        ->fill(integer(2, 5))
        ->format("$x $last[\n]")
    );
    a->generate(0);
    a->format("$x")->out();
}

void test_nested_array_3() {
    auto a = make_shared<Array>()->length(3)->fill(integer(1, 3));
    auto c = make_shared<Array>()
        ->fill(make_shared<Array>()
            ->fill(a)
            ->length(integer(1, 3)))
        ->length(5)
        ->format("$x\n");
    c->generate(0);
    c->out();
}

using Test = std::function<void()>;
Test test_map[] = {
    test_array_is_null,                 // 0
    test_array_have_the_same_length_1,  // 1
    test_array_have_the_same_length_2,  // 2
    test_array_have_the_same_element,   // 3
    test_nested_array_1,                // 4
    test_nested_array_2,                // 5
    test_nested_array_3                 // 6
};

int main(int n, char** args) {
    int idx = atoi(args[1]);
    test_map[idx]();
    return 0;
}