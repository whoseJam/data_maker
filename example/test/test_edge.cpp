#include <iostream>

#include "Data.h"
#include "Edge.h"

using std::cout;

int main() {
    Edge* e = (new Edge())
        ->add_attribute((new Attribute())->value(integer(1, 10))->name("flow"))
        ->add_attribute((new Attribute())->value(lower_character())->name("type"))
        ->add_attribute((new Attribute())->value(integer(20, 30))->name("weight"))
        ->format("$s with flow $x[flow] $t\n");
    e->__set_start_and_end(1, 5);
    e->generate();
    e->out();
    return 0;
}