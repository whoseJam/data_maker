
#include <iostream>

#include "Data.h"

using std::cout;

void test_1() {
    Integer* n = integer(3, 5);
    Array* a = (new Array())->fill(integer(1, 10000))->length(n)->format("$x ");
    Array* b = (new Array())->fill(integer(1, 10000))->length(n)->format("$x ");
    Tuple* c = tuple(n, a, b)->format("$x\n");
    c->generate();
    c->out();
}

void get_entity_or_ref(Array*& i) {
    cout << "get the ref\n";
}

void get_entity_or_ref(Array*&& i) {
    cout << "get the obj\n";
}

void test_2() {
    Integer* n = integer(3, 5);
    Array* a = new Array();
    Array* b = new Array();
    a->fill(tuple(to_obj<Array>(b)))->length(2);
    b->fill(integer(1, 10000))->length(n)->format("$x $last[\n]");

    a->generate();
    a->out();
}

int main() {
    test_2();
    return 0;
}