#include "Array.h"
#include "Tuple.h"

using namespace mk;
using namespace std;

void test1() {
    auto n = integer(1, 4);
    auto m = integer(1, 5);
    auto col = mk::array(m, integer(4, 9));
    auto row = mk::array(n, col)->format("$x\n");
    auto mat = mk::tuple(
        n->format("$x "),
        m->format("$x\n"),
        row
    )->format("$x");
    n->live_with(mat);
    m->live_with(mat);

    auto multi = mk::array(3, mat)->format("$x--------\n");
    BUILD(multi);
}

void test2() {
    auto n = integer(3, 4);
    auto k = n - integer(1);
    auto arr1 = mk::array(n, integer(1, 100));
    auto arr2 = mk::array(n, integer(1, 100));
    auto case_ = mk::tuple(
        n->format("$x "),
        k->format("$x\n"),
        arr1->format("$x $last[\n]"),
        arr2->format("$x $last[\n-------\n]")
    )->format("$x");
    k->live_with(case_);
    n->live_with(case_);

    auto multi = mk::array(3, case_)->format("$x");
    BUILD(multi);
}

int main() {
    test2();
    return 0;
}