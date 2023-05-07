
#include <iostream>

#include "Clone.h"

using namespace std;

Clone Clone::instance;

Clone::Clone() {
}

Clone* Clone::get() {
    return &instance;
}

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