#include "Array.h"
#include "Tuple.h"
#include "Option.h"
#include "Character.h"

using namespace std;
using namespace mk;

void test_1() {
    auto n = integer(5);
    auto a_type = mk::tuple(
        character('a'),
        integer(1, n)
    );
    auto b_type = mk::tuple(
        character('b'),
        integer(10000, 20000),
        integer(1, 10)
    );
    auto arr = mk::array(5, 
        make_shared<Option>()
        ->add_option(a_type, 80)
        ->add_option(b_type, 20)
    )->format("$x\n");

    BUILD(arr);
}

int main() {
    test_1();
    return 0;
}