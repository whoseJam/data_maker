
#include <iostream>

#include "Clone.h"
#include "Debug.h"
#include "Logger.h"
#include "Random.h"
#include "Integer.h"

using namespace std;
using namespace Random;

Integer::Integer() {
    CALL("Integer", "Integer");
    status = EMPTY;
    fmt = "$x";
}

Integer::Integer(const Integer& other) :
    Node(other),
    Formatable(other) {
    CALL("Integer", "Integer");
    if (other.l) l = dynamic_pointer_cast<Integer>(other.l->clone());
    if (other.r) r = dynamic_pointer_cast<Integer>(other.r->clone());
    if (other.op) op = dynamic_pointer_cast<Operator<Integer>>(other.op->clone());
    status = other.status;
    int_val = other.int_val;
    if (other.ptr_val) ptr_val = dynamic_pointer_cast<Integer>(other.ptr_val->clone());
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
    l->set(x); status = BY_LR;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

shared_ptr<Integer> Integer::upper_bound(int x) {
    CALL("Integer", "upper_bound");
    if (!r) r = make_shared<Integer>();
    r->set(x); status = BY_LR;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

shared_ptr<Integer> Integer::format(const string& fmt) {
    CALL("Integer", "format");
    this->fmt = fmt;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

int Integer::get() {
    CALL("Integer", "get");
    if (status == BY_SET_PTR && ptr_val) return ptr_val->get();
    if (status == BY_LR || status == BY_SET_VAL) return int_val;
    if (status == BY_OP && op) return op->get();
    MESSAGE("Integer", "get failed because no value is set");
}

shared_ptr<Integer> Integer::get_lower_bound() {
    CALL("Integer", "get_lower_bound");
    if (!l) MESSAGE("Integer", NEED("lower_bound"));
    return l;
}

shared_ptr<Integer> Integer::get_upper_bound() {
    CALL("Integer", "get_upper_bound");
    if (!r) MESSAGE("Integer", NEED("upper_bound"));
    return r;
}

shared_ptr<Integer> Integer::set(int x) {
    CALL("Integer", "set");
    int_val = x; status = BY_SET_VAL;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

void Integer::generate(bool re, shared_ptr<Node> from) {
    CALL("Integer", "generate");
    from_node = from;
    if (generated && !re) return;
    generated = true;
    
    if (status == BY_SET_PTR) {
        if (l) MESSAGE("Integer", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Integer", CONFLICT("set", "upper_bound"));
        ptr_val->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
    } else if (status == BY_SET_VAL) {
        if (l) MESSAGE("Integer", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Integer", CONFLICT("set", "upper_bound"));
    } else if (status == BY_LR){
        if (!l) MESSAGE("Integer", NEED("lower_bound"));
        if (!r) MESSAGE("Integer", NEED("upper_bound"));
        l->generate(re, dynamic_pointer_cast<Node>(shared_from_this())); 
        r->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
        if (l->get() > r->get()) MESSAGE("Integer", ENSURE("l < r"));
        int_val = rand_int(l->get(), r->get());
    } else if (status == BY_OP) {
        if (!op) MESSAGE("Integer", NEED("calculate"));
        op->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
    } else {
        MESSAGE("Integer", NEED("any initialize"));
    }
}

CL_CLONE(Integer);

void Integer::out() {
    CALL("Integer", "out");
    Formatable::parse(
        shared_from_this(), fmt, "Integer");
}

bool Integer::equal(shared_ptr<Hashable> o) {
    CALL("Integer", "equal");
    shared_ptr<Integer> other = dynamic_pointer_cast<Integer>(o);
    if (!other) return false;
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

namespace mk {
    shared_ptr<Integer> integer(int x) {
        return make_shared<Integer>()->set(x);
    }
}
