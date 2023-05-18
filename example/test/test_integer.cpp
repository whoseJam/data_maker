#include <iostream>
#include "Integer.h"

using namespace std;

void test_nested_integer() {
    shared_ptr<Integer> a = integer(1, 10)->format("$x hello world\n");
    a->generate(0);
    a->out();

    shared_ptr<Integer> b = integer(integer(1, 100), 100);
    b->generate(0);
    b->out(); cout << "\n";

    shared_ptr<Integer> c = integer(integer(1, 10), integer(100, 1000));
    c->generate(0);
    auto lc = c->get_lower_bound();
    auto rc = c->get_upper_bound();
    cout << "c=" << c->get() << " lc=" << lc->get() << " rc=" << rc->get() << "\n";
    cout << "lc's owner = " << lc.use_count() << "\n"; auto lc_ = c->get_lower_bound();
    cout << "lc's owner = " << lc.use_count() << "\n"; auto lc__ = lc;
    cout << "lc's owner = " << lc.use_count() << "\n";
    cout << " c's owner = " <<  c.use_count() << "\n";
}

void test_format() {
    auto a = integer(10, 100)->format("this is $x\n");
    a->generate(0); a->out();

}

int main() {
    test_nested_integer();
    test_format();
    return 0;
}