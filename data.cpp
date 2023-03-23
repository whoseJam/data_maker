#include "include/Data.h"
#include <fstream>
using namespace std;

void test_array() {
    Array* tmp = 
    (new Array())
        ->fill(
            (new Character())
                ->lower_bound('a')
                ->upper_bound('z'))
        ->length(5)
        ->format("%x-");
    tmp->generate();
    tmp->out();
}

void test_permutation() {
    Permutation* tmp = 
    (new Permutation())
        ->length(10);
    tmp->generate();
    tmp->out();
}

void test_tree() {
    Tree* tmp = 
    (new Tree())
        ->vertex(6)
        ->add_attribute(
            (new Attribute())
                ->name("value")
                ->value(
                    (new Integer())
                        ->upper_bound(6)
                        ->lower_bound(1)
                )
        )
        ->add_attribute(
            (new Attribute())
                ->name("type")
                ->value(
                    (new Character())
                        ->lower_bound('a')
                        ->upper_bound('c')
                )
        )
        ->format("%fa->%x value=%value type=%type\n");
    tmp->generate();
    tmp->out();
}

int main() {
    return 0;
}