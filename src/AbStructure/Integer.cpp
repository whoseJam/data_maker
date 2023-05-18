
#include <iostream>
#include <memory>

#include "Debug.h"
#include "Logger.h"
#include "Operator.h"
#include "Random.h"
#include "Integer.h"

using namespace std;
using namespace Random;

namespace mk {

Integer::Integer() {
    CALL(FUNCTION);
    status = EMPTY;
    fmt = "$x";
}

Integer::Integer(const Integer& other) :
    Node(other),
    Formatable(other) {
    CALL(FUNCTION);
    if (other.l) l = dynamic_pointer_cast<Integer>(other.l->clone());
    if (other.r) r = dynamic_pointer_cast<Integer>(other.r->clone());
    if (other.op) op = dynamic_pointer_cast<Operator<Integer>>(other.op->clone());
    status = other.status;
    int_val = other.int_val;
    if (other.ptr_val) ptr_val = dynamic_pointer_cast<Integer>(other.ptr_val->clone());
    fmt = other.fmt;
}

static int delete_cnt = 0;
Integer::~Integer() {
#ifdef DELETE_CHECK
    cout << "delete integer " << ++delete_cnt << "\n";
#endif
}

auto Integer::calculate(shared_ptr<Operator<Integer>> op) -> shared_ptr<Integer> {
    CALL(FUNCTION);
    this->op = op; status = BY_OP;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

auto Integer::lower_bound(int x) -> shared_ptr<Integer> {
    CALL(FUNCTION);
    if (!l) l = make_shared<Integer>();
    l->value(x); status = BY_LR;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

auto Integer::upper_bound(int x) -> shared_ptr<Integer> {
    CALL(FUNCTION);
    if (!r) r = make_shared<Integer>();
    r->value(x); status = BY_LR;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

auto Integer::lower_bound(shared_ptr<Integer> l) -> shared_ptr<Integer> {
    CALL(FUNCTION);
    this->l = l; status = BY_LR;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

auto Integer::upper_bound(shared_ptr<Integer> r) -> shared_ptr<Integer> {
    CALL(FUNCTION);
    this->r = r; status = BY_LR;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

auto Integer::format(const string& fmt) -> shared_ptr<Integer> {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

auto Integer::value() -> int {
    CALL(FUNCTION);
    if (status == BY_SET_PTR && ptr_val) return ptr_val->value();
    if (status == BY_LR || status == BY_SET_VAL) return int_val;
    if (status == BY_OP && op) return op->get();
    MESSAGE("Integer", "get failed because no value is set");
}

auto Integer::lower_bound() -> shared_ptr<Integer> {
    CALL(FUNCTION);
    if (!l) MESSAGE("Integer", NEED("lower_bound"));
    return l;
}

auto Integer::upper_bound() -> shared_ptr<Integer> {
    CALL(FUNCTION);
    if (!r) MESSAGE("Integer", NEED("upper_bound"));
    return r;
}

auto Integer::value(int val) -> shared_ptr<Integer> {
    CALL(FUNCTION);
    int_val = val; status = BY_SET_VAL;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

auto Integer::value(shared_ptr<Integer> val) -> shared_ptr<Integer> {
    CALL(FUNCTION);
    ptr_val = val; status = BY_SET_PTR;
    return dynamic_pointer_cast<Integer>(shared_from_this());
}

void Integer::generate(bool re) {
    CALL(FUNCTION);
    if (generated && !re) return;
    generated = true;
    
    if (status == BY_SET_PTR) {
        if (l) MESSAGE("Integer", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Integer", CONFLICT("set", "upper_bound"));
        ptr_val->generate(re);
    } else if (status == BY_SET_VAL) {
        if (l) MESSAGE("Integer", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Integer", CONFLICT("set", "upper_bound"));
    } else if (status == BY_LR){
        if (!l) MESSAGE("Integer", NEED("lower_bound"));
        if (!r) MESSAGE("Integer", NEED("upper_bound"));
        l->generate(re); 
        r->generate(re);
        if (l->value() > r->value()) MESSAGE("Integer", ENSURE("l < r"));
        int_val = rand_int(l->value(), r->value());
    } else if (status == BY_OP) {
        if (!op) MESSAGE("Integer", NEED("calculate"));
        op->generate(re);
    } else {
        MESSAGE("Integer", NEED("any initialize"));
    }
}

CL_CLONE(Integer);

void Integer::out() {
    CALL(FUNCTION);
    Formatable::parse(
        shared_from_this(), fmt, "Integer");
}

void Integer::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            cout << value();
        } else {
            MESSAGE("Integer", FUNC_NOT_FOUND(spec));
        }
    }
}

shared_ptr<Integer> integer(int x) { return make_shared<Integer>()->value(x); }
}
