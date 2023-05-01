#include <iostream>
#include "Integer.h"
#include "Array.h"

using std::cout;

int main() {
    auto a = array(5)->fill(integer(1, 10));
    a->generate(0);
    for (int i = 0; i < a->get_length(); i++) {
        auto e = a->get<Integer>(i);
        cout << e->get() << " ";
    } cout << "\n";
    return 0;
}