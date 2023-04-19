#include <string>
#include <iostream>

#include "../include/Clone.h"
#include "../include/Define.h"
#include "../include/Destroy.h"
#include "../include/IntegerWrapper.h"

using namespace std;

IntegerWrapper::IntegerWrapper(const string& class_name, const string& var_name) {
    int_value = UNSET;
    integer_value = nullptr;
    var = var_name;
    cls = class_name;
}

IntegerWrapper::IntegerWrapper(const IntegerWrapper& other) {
    int_value = other.int_value;
    integer_value = (Integer*)other.integer_value->clone();
    var = other.var;
    cls = other.cls;
}

int IntegerWrapper::get() {
    if (int_value == UNSET && integer_value == nullptr) {
        cout << cls << "::" << var << " is UNSET\n";
        exit(-1);
    }
    if (int_value != UNSET) return int_value;
    if (integer_value != nullptr) return integer_value->value;
    cout <<  cls << "::" << var << " Unknown Error\n";
    exit(-1);
}

void IntegerWrapper::set(int val) {
    int_value = val;
    integer_value = nullptr;
}

void IntegerWrapper::set(Integer* val) {
    integer_value = val;
    int_value = UNSET;
}

void IntegerWrapper::generate() {
    if (generated) return;
    generated = true;

    if (integer_value != nullptr) {
        integer_value->generate();
    }
}

Node* IntegerWrapper::clone() {
    if (!Clone::get()->check(this))
        Clone::get()->insert(this, new IntegerWrapper(*this));
    return (Node*)Clone::get()->check(this);
}

void IntegerWrapper::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    if (integer_value != nullptr) {
        integer_value->destroy();
    }
}

void IntegerWrapper::out() {
    cout << get();
}