
#include <iostream>

#include "../include/Node.h"
#include "../include/Define.h"

using namespace std;

Node::Node() {
    generated = false;
    destroyed = false;
    type = STRUCTURE_NODE;
}

Node::~Node() {
#ifdef OUTPUT_DELETER
    cout << "delete node\n";
#endif
}