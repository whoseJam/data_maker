
#include <iostream>
#include <functional>

#include "Integer.h"
#include "Attribute.h"

using std::cout;
using std::make_shared;
using namespace mk;

void test_attribute_val_is_null() {
    auto a = attr()->name("flow");
    BUILD(a);
}

void test_attribute_key_is_null() {
    auto a = make_shared<Attribute>()->value(integer(1, 10));
    BUILD(a);
}

void test_attribute_copy() {
    auto a = make_shared<Attribute>()->value(integer(1, 10000))->name("a");
    make_structure(a);
    auto b = std::dynamic_pointer_cast<Attribute>(a->clone());
    a->generate(0); b->generate(0);
    a->out(); cout << " and ";
    b->out(); cout << " should be diff\n";

    auto i = integer(1, 10000);
    auto c = make_shared<Attribute>()->value(i)->name("b");
    auto d = std::dynamic_pointer_cast<Attribute>(c->clone());
    c->generate(0); d->generate(0);
    c->out(); cout << " and ";
    d->out(); cout << " should be the same\n";
}

using Test = std::function<void()>;
Test test_map[] = {
    test_attribute_val_is_null,     // 0
    test_attribute_key_is_null,     // 1
    test_attribute_copy,            // 2
};

int main(int n, char** args) {
    int idx = atoi(args[1]);
    test_map[idx]();
    return 0;
}