
#include <iostream>

#include "Node.h"
#include "Clone.h"
#include "Debug.h"
#include "Define.h"
#include "Logger.h"

using namespace std;

Node::Node() {
    CALL("Node", "Node");
    generated = false;
    type = UN_NODE;
    explicit_flag = false;
}

Node::Node(const Node& other) {
    CALL("Node", "Node");
    // if (other.generated) MESSAGE("Node", ENSURE("other should never be generated"));
    generated = other.generated;
    type = other.type;
    explicit_flag = other.explicit_flag;
}

Node::~Node() {
#ifdef OUTPUT_DELETER
    cout << "delete node\n";
#endif
}

void Node::implicit_type(int type) {
    if (!explicit_flag) this->type = type;
}

void Node::explicit_type(int type) {
    if (!explicit_flag) {
        this->type = type;
        explicit_flag = true;
    } else if (this->type != type) {
        MESSAGE("Node", "multi explicit type is set");
    }
}