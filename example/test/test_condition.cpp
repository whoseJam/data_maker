#include "Data.h"

void test_in_range() {
    Array* a = (new Array())->fill(integer(4, 10))->satisfy([](ArrayPanel* p, int idx) -> bool {
        int sm = 0;
        for (int i = 0; i < idx; i++) {
            Integer* v = dynamic_cast<Integer*>(p->element(i));
        }
    });
}

int main() {


    return 0;
}