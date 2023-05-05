
#include <iostream>

#include "Clone.h"

using namespace std;

Clone Clone::instance;

Clone::Clone() {
    cnt = 0;
}

Clone* Clone::get() {
    return &instance;
}

void Clone::enter(bool flag) {
    if (flag) cnt++;
    // cout<<"enter flag="<<flag<<"\n";
    has_structure.push_back(flag);
}

void Clone::exit() {
    if (has_structure.back()) cnt--;
    has_structure.pop_back();
    if (has_structure.size() == 0)
        helper.clear();
}

bool Clone::structure() {
    // cout<<"check structure="<<(cnt>0)<<"\n";
    return cnt > 0;
}

void Clone::insert(void* ptr, shared_ptr<Node> val) {
    // cout<<"insert at="<<ptr<<" val="<<val<<"\n";
    helper[ptr] = val;
}

shared_ptr<Node> Clone::check(void* ptr) {
    // cout<<"check ptr="<<ptr<<"\n";
    if (helper.find(ptr) == helper.end())
        return nullptr;
    return helper[ptr];
}