
#include <iostream>

#include "Node.h"
#include "Debug.h"
#include "Define.h"

using namespace std;

Node::Node() {
    CALL("Node", "Node");
    generated = false;
    type = VALUE_NODE;
}

Node::Node(const Node& other) {
    CALL("Node", "Node");
    type = other.type;
}

Node::~Node() {
#ifdef OUTPUT_DELETER
    cout << "delete node\n";
#endif
}

void make_structure(shared_ptr<Node> x) {
    x->type = Node::STRUCTURE_NODE;
}

void make_value(shared_ptr<Node> x) {
    x->type = Node::VALUE_NODE;
}