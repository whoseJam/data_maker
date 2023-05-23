
#include <cstddef>
#include <iostream>

#include "Node.h"
#include "Debug.h"
#include "Define.h"
#include "Logger.h"

using namespace std;

namespace mk {

bool gdebug = false;
namespace NodeMap {
map<void*, shared_ptr<Node>> node_map[4];
auto insert(int type, void* ptr, shared_ptr<Node> item) -> void {
    if (!ptr) return;
    node_map[type][ptr] = item;
}
auto remove(int type, void* ptr) -> void {
    if (!ptr) return;
    node_map[type].erase(ptr);
}
auto clear(int type) -> void {
    node_map[type].clear();
}
auto get(int type, void* ptr) -> shared_ptr<Node> {
    if (!ptr) return nullptr;
    return node_map[type][ptr];
}
auto size(int type) -> int {
    return node_map[type].size();
}
}

Node::Node() {
    CALL(FUNCTION);
    generated = false;
    parent = nullptr;
    origin = this;
    debug = false;
}

Node::Node(const Node& other) {
    CALL(FUNCTION);
    generated = false;
    parent = other.parent;
    origin = other.origin;
    debug = other.debug;
}

Node::~Node() {
#ifdef OUTPUT_DELETER
    cout << "delete node\n";
#endif
}

void Node::live_with(shared_ptr<Node> prt) {
    CALL(FUNCTION);
    parent = prt.get();
}

}