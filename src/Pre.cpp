#include <string>

#include "../include/Pre.h"

using namespace std;

Character* lower_character() {
    return (new Character())
        ->lower_bound('a')
        ->upper_bound('z');
}

Character* upper_character() {
    return (new Character())
        ->lower_bound('A')
        ->upper_bound('Z');
}

Integer* integer(int l, int r) {
    return (new Integer())
        ->lower_bound(l)
        ->upper_bound(r);
}

IO* standard_io() {
    return (new IO())
        ->input_prefix("input")
        ->output_prefix("output")
        ->input_suffix("txt")
        ->output_suffix("txt");
}

Tuple* make_tuple(Node* a) {
    return (new Tuple())->append(a);
}
Tuple* make_tuple(Node* a, Node* b) {
    return (new Tuple())->append(a)->append(b);
}
Tuple* make_tuple(Node* a, Node* b, Node* c) {
    return (new Tuple())
        ->append(a)
        ->append(b)
        ->append(c);
}
Tuple* make_tuple(Node* a, Node* b, Node* c, Node* d) {
    return (new Tuple())
        ->append(a)->append(b)
        ->append(c)->append(d);
}
Tuple* make_tuple(Node* a, Node* b, Node* c, Node* d, Node* e) {
    return make_tuple(a, b, c, d)->append(e);
}
