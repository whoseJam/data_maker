
#include <iostream>

#include "Debug.h"
#include "Logger.h"
#include "Random.h"
#include "Integer.h"

using namespace std;
using namespace Random;

Integer::Integer() {
    CALL("Integer", "Integer");
    int_val = UNSET;
    fmt = "$x";
}

Integer::Integer(const Integer& other) :
    Node(other),
    Formatable(other) {
    CALL("Integer", "Integer");
    int_val = UNSET;
    int_val_is_set = other.int_val_is_set;
    ptr_val = other.ptr_val;
    l = other.l;
    r = other.r;
    fmt = other.fmt;
}

Integer::~Integer() {
#ifdef OUTPUT_DELETER
    cout << "delete integer\n";
#endif
}

shared_ptr<Integer> Integer::lower_bound(int x) {
    CALL("Integer", "lower_bound");
    if (!l) l = make_shared<Integer>();
    l->set(x);
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

shared_ptr<Integer> Integer::upper_bound(int x) {
    CALL("Integer", "upper_bound");
    if (!r) r = make_shared<Integer>();
    r->set(x);
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

shared_ptr<Integer> Integer::format(const string& fmt) {
    CALL("Integer", "format");
    this->fmt = fmt;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

int Integer::get() {
    CALL("Integer", "get");
    if (ptr_val) return ptr_val->get();
    if (int_val != UNSET) return int_val;
    MESSAGE("Integer", "get failed because no value is set");
}

shared_ptr<Integer> Integer::get_lower_bound() {
    CALL("Integer", "get_lower_bound");
    return l;
}

shared_ptr<Integer> Integer::get_upper_bound() {
    CALL("Integer", "get_upper_bound");
    return r;
}

shared_ptr<Integer> Integer::set(int x) {
    CALL("Integer", "set");
    int_val = x;
    int_val_is_set = true;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

void Integer::generate(bool re) {
    CALL("Integer", "generate");
    if (generated && !re) return;
    generated = true;
    
    if (ptr_val) {
        if (l) MESSAGE("Integer", CONFLICT("set", "_lower_bound"));
        if (r) MESSAGE("Integer", CONFLICT("set", "_upper_bound"));
        ptr_val->generate(re);
    } else if (int_val_is_set) {
        if (l) MESSAGE("Integer", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Integer", CONFLICT("set", "upper_bound"));
    } else {
        if (!l) MESSAGE("Integer", NEED("lower_bound"));
        if (!r) MESSAGE("Integer", NEED("upper_bound"));
        l->generate(re); r->generate(re);
        if (l->get() > r->get()) MESSAGE("Integer", ENSURE("l < r"));
        int_val = rand_int(l->get(), r->get());
    }
#ifdef DEBUG_INTEGER_FLAG
    cout << "l=" << l->get() << " r=" << r->get() << "\n";
#endif
}

shared_ptr<Node> Integer::clone() {
    CALL("Integer", "clone");
    if (type == STRUCTURE_NODE)
        return make_shared<Integer>(*this);
    return dynamic_pointer_cast<Node>(shared_from_this());
}

void Integer::out() {
    CALL("Integer", "out");
    Formatable::parse(
        enable_shared_from_this<Formatable>::shared_from_this(), fmt, "Integer");
}

bool Integer::equal(shared_ptr<Hashable> o) {
    CALL("Integer", "equal");
    shared_ptr<Integer> other;
    other = dynamic_pointer_cast<Integer>(o);
    if (other == nullptr) return false;
    return get() == other->get();
}

uint Integer::hash_code() {
    CALL("Integer", "hash_code");
    return (uint) get();
}

void Integer::parse(const string& spec, int n, ...) {
    CALL("Integer", "parse");
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            cout << get();
        } else {
            MESSAGE("Integer", FUNC_NOT_FOUND(spec));
        }
    }
}
