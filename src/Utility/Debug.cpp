
#include <iostream>

#include "Debug.h"

using namespace std;

Call::Call(const string& cls, const string& fun) {
    CallStack::get()->enter(cls, fun);
}

Call::~Call() {
    CallStack::get()->exit();
}

CallStack CallStack::instance;

CallStack* CallStack::get() {
    return &instance;
}

void CallStack::enter(const string& cls, const string& fun) {
    this->cls.push_back(cls);
    this->func.push_back(fun);
}

void CallStack::exit() {
    this->cls.pop_back();
    this->func.pop_back();
}

void CallStack::print() {
    while (cls.size() > 0) {
        cout << cls.front() << " :: " << func.front() << "\n";
        cls.pop_front();
        func.pop_front();
    }
}