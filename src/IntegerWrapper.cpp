#include <string>
#include <iostream>

#include "../include/Define.h"
#include "../include/Format.h"
#include "../include/Destroy.h"
#include "../include/Integer.h"
#include "../include/IntegerWrapper.h"

using namespace std;
using namespace Format;

IntegerWrapper::IntegerWrapper() {
    int_value = UNSET;
    integer_value = nullptr;
}

IntegerWrapper::IntegerWrapper(const IntegerWrapper& other) {
    int_value = other.int_value;
    integer_value = (Integer*)other.integer_value->clone();
}

int IntegerWrapper::get() {
    if (int_value == UNSET && integer_value == nullptr) {
        cout << "IntegerWrapper" << "::" << "value" << " is UNSET\n";
        exit(-1);
    }
    if (int_value != UNSET) return int_value;
    if (integer_value != nullptr) return integer_value->value;
    cout << "IntegerWrapper" << "::" << " Unknown Error\n";
    exit(-1);
}

IntegerWrapper* IntegerWrapper::value(int val) {
    int_value = val;
    integer_value = nullptr;
    return this;
}

void IntegerWrapper::generate(bool re) {
    if (generated && !re) return;
    generated = true;

    CL_GENERATE(integer_value);
}

Node* IntegerWrapper::clone() {
    if (type == STRUCTURE_NODE)
        return (Node*)new IntegerWrapper(*this);
    return this;
}

void IntegerWrapper::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    CL_DESTROY(integer_value);
}

void IntegerWrapper::out() {
    cout << get();
}

bool IntegerWrapper::equal(Node* o) {
    IntegerWrapper* other = dynamic_cast<IntegerWrapper*>(o);
    if (other == nullptr) return false;
    return get() == other->get();
}