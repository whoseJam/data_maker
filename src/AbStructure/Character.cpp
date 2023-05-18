
#include <iostream>
#include <memory>

#include "Debug.h"
#include "Logger.h"
#include "Operator.h"
#include "Random.h"
#include "Character.h"

using namespace std;
using namespace Random;

namespace mk {

Character::Character() {
    CALL(FUNCTION);
    status = EMPTY;
    fmt = "$x";
}

Character::Character(const Character& other) :
    Node(other),
    Formatable(other) {
    CALL(FUNCTION);
    if (other.l) l = dynamic_pointer_cast<Character>(other.l->clone());
    if (other.r) r = dynamic_pointer_cast<Character>(other.r->clone());
    if (other.op) op = dynamic_pointer_cast<Operator<Character>>(other.op->clone());
    status = other.status;
    char_val = other.char_val;
    if (other.ptr_val) ptr_val = dynamic_pointer_cast<Character>(other.ptr_val->clone());
    fmt = other.fmt;
}

Character::~Character() {
#ifdef OUTPUT_DELETER
    cout << "delete character\n";
#endif
}

auto Character::calculate(shared_ptr<Operator<Character>> op) -> shared_ptr<Character> {
    CALL(FUNCTION);
    this->op = op; status = BY_OP;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

auto Character::lower_bound(char x) -> shared_ptr<Character> {
    CALL(FUNCTION);
    if (!l) l = make_shared<Character>();
    l->value(x); status = BY_LR;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

auto Character::upper_bound(char x) -> shared_ptr<Character> {
    CALL(FUNCTION);
    if (!r) r = make_shared<Character>();
    r->value(x); status = BY_LR;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

auto Character::lower_bound(shared_ptr<Character> l) -> shared_ptr<Character> {
    CALL(FUNCTION);
    this->l = l; status = BY_LR;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

auto Character::upper_bound(shared_ptr<Character> r) -> shared_ptr<Character> {
    CALL(FUNCTION);
    this->r = r; status = BY_LR;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

auto Character::format(const string& fmt) -> shared_ptr<Character> {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

auto Character::value() -> char {
    CALL(FUNCTION);
    if (status == BY_SET_PTR && ptr_val) return ptr_val->value();
    if (status == BY_LR || status == BY_SET_VAL) return char_val;
    if (status == BY_OP && op) return op->get();
    MESSAGE("Character", "get failed because no value is set");
}

auto Character::lower_bound() -> shared_ptr<Character> {
    CALL(FUNCTION);
    if (!l) MESSAGE("Character", NEED("lower_bound"));
    return l;
}

auto Character::upper_bound() -> shared_ptr<Character> {
    CALL(FUNCTION);
    if (!r) MESSAGE("Character", NEED("upper_bound"));
    return r;
}

auto Character::value(char x) -> shared_ptr<Character> {
    CALL(FUNCTION);
    char_val = x; status = BY_SET_VAL;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

auto Character::value(shared_ptr<Character> val) -> shared_ptr<Character> {
    CALL(FUNCTION);
    ptr_val = val; status = BY_SET_PTR;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

void Character::generate(bool re) {
    CALL(FUNCTION);
    if (generated && !re) return;
    generated = true;
    
    if (status == BY_SET_PTR) {
        if (l) MESSAGE("Character", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Character", CONFLICT("set", "upper_bound"));
        ptr_val->generate(re);
    } else if (status == BY_SET_VAL) {
        if (l) MESSAGE("Character", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Character", CONFLICT("set", "upper_bound"));
    } else if (status == BY_LR){
        if (!l) MESSAGE("Character", NEED("lower_bound"));
        if (!r) MESSAGE("Character", NEED("upper_bound"));
        l->generate(re); 
        r->generate(re);
        if (l->value() > r->value()) MESSAGE("Character", ENSURE("l < r"));
        char_val = rand_int(l->value(), r->value());
    } else if (status == BY_OP) {
        if (!op) MESSAGE("Character", NEED("calculate"));
        op->generate(re);
    } else {
        MESSAGE("Integer", NEED("any initialize"));
    }
}

CL_CLONE(Character);

void Character::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            cout << value();
        } else {
            MESSAGE("Character", FUNC_NOT_FOUND(spec));
        }
    }
}

void Character::out() {
    CALL(FUNCTION);
    Formatable::parse(
        shared_from_this(), fmt, "Character");
}

shared_ptr<Character> character(char x) {
    return make_shared<Character>()->value(x);
}

}