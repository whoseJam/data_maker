
#include <iostream>

#include "Node.h"
#include "Debug.h"
#include "Define.h"
#include "Logger.h"

using namespace std;

namespace mk {

Tracker Tracker::instance;
Tracker* Tracker::get() { return &instance; }

void Tracker::track_on(void* origin, std::shared_ptr<Node> cur) {
    memory[origin] = cur;
}

auto Tracker::find(void* origin) -> shared_ptr<Node> {
    return memory[origin];
}

Clone Clone::instance;
Clone* Clone::get() { return &instance; }

void Clone::enter(shared_ptr<Node> ptr) {
    void* pptr = ptr.get();
    cnt[pptr]++;
    stack.push_back(pptr);
}

void Clone::exit() {
    void* lst = stack.back();
    stack.pop_back();
    cnt[lst]--;
    if (stack.size() == 0) {
        cnt.clear();
        helper.clear();
    }
}

bool Clone::check_stay_with(void* ptr) {
    return cnt[ptr] > 0;
}

shared_ptr<Node> Clone::check(void* ptr) {
    return helper[ptr];
}

void Clone::insert(void* pos, std::shared_ptr<Node> ptr) {
    helper[pos] = ptr;
}

Node::Node() {
    CALL(FUNCTION);
    generated = false;
    parent = nullptr;
    origin = nullptr;
}

Node::Node(const Node& other) {
    CALL(FUNCTION);
    generated = other.generated;
    parent = other.parent;
    origin = other.origin;
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

void Node::keep_track() {
    CALL(FUNCTION);
    origin = this;
}

}