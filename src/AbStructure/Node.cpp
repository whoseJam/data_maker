
#include <iostream>

#include "Node.h"
#include "Clone.h"
#include "Debug.h"
#include "Define.h"
#include "Logger.h"

using namespace std;

Node::Node() {
    CALL(FUNCTION);
    generated = false;
    parent = nullptr;
}

Node::Node(const Node& other) {
    CALL(FUNCTION);
    generated = other.generated;
    parent = other.parent;
}

Node::~Node() {
#ifdef OUTPUT_DELETER
    cout << "delete node\n";
#endif
}

void Node::live_with(shared_ptr<Node> prt) {
    parent = prt.get();
}