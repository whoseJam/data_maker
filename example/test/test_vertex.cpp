#include <iostream>

#include "Data.h"
#include "Vertex.h"

using std::cout;

void test_vertex_is_null() {
    Vertex* v = (new Vertex());
    v->generate();
    v->out();
}

void test_vertex_normal() {
    Vertex* v = (new Vertex())->add_attribute(
        (new Attribute())->name("weight")->value(integer(1, 10))
    )->add_attribute(
        (new Attribute())->name("color")->value(lower_character())
    )->index(1);
    v->generate();
    v->format("$x[weight] $x[color] $x")->out();
}

using Test = std::function<void()>;

Test test_map[] = {
    test_vertex_is_null,        // 0
    test_vertex_normal,         // 1
};

int main(int n, char** args) {
    int idx = atoi(args[1]);
    test_map[idx]();
    return 0;
}