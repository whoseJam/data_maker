
#include <iostream>

#include "Clone.h"
#include "Logger.h"
#include "Random.h"
#include "Character.h"

using namespace std;
using namespace Random;

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

shared_ptr<Character> Character::lower_bound(char x) {
    CALL(FUNCTION);
    if (!l) l = make_shared<Character>();
    l->set(x); status = BY_LR;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

shared_ptr<Character> Character::upper_bound(char x) {
    CALL(FUNCTION);
    if (!r) r = make_shared<Character>();
    r->set(x); status = BY_LR;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

shared_ptr<Character> Character::format(const string& fmt) {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

char Character::get() {
    CALL(FUNCTION);
    if (status == BY_SET_PTR && ptr_val) return ptr_val->get();
    if (status == BY_LR || status == BY_SET_VAL) return char_val;
    if (status == BY_OP && op) return op->get();
    MESSAGE("Character", "get failed because no value is set");
}

shared_ptr<Character> Character::get_lower_bound() {
    CALL(FUNCTION);
    if (!l) MESSAGE("Character", NEED("lower_bound"));
    return l;
}

shared_ptr<Character> Character::get_upper_bound() {
    CALL(FUNCTION);
    if (!r) MESSAGE("Character", NEED("upper_bound"));
    return r;
}

shared_ptr<Character> Character::set(char x) {
    CALL(FUNCTION);
    char_val = x; status = BY_SET_VAL;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

void Character::generate(bool re, shared_ptr<Node> from) {
    CALL(FUNCTION);
    from_node = from;
    if (generated && !re) return;
    generated = true;
    
    if (status == BY_SET_PTR) {
        if (l) MESSAGE("Character", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Character", CONFLICT("set", "upper_bound"));
        ptr_val->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
    } else if (status == BY_SET_VAL) {
        if (l) MESSAGE("Character", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Character", CONFLICT("set", "upper_bound"));
    } else if (status == BY_LR){
        if (!l) MESSAGE("Character", NEED("lower_bound"));
        if (!r) MESSAGE("Character", NEED("upper_bound"));
        l->generate(re, dynamic_pointer_cast<Node>(shared_from_this())); 
        r->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
        if (l->get() > r->get()) MESSAGE("Character", ENSURE("l < r"));
        char_val = rand_int(l->get(), r->get());
    } else if (status == BY_OP) {
        if (!op) MESSAGE("Character", NEED("calculate"));
        op->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
    } else {
        MESSAGE("Integer", NEED("any initialize"));
    }
}

CL_CLONE(Character);

bool Character::equal(shared_ptr<Hashable> o) {
    CALL(FUNCTION);
    shared_ptr<Character> other = dynamic_pointer_cast<Character>(o);
    if (!other) return false;
    return get() == other->get();
}

uint Character::hash_code() {
    CALL(FUNCTION);
    return (uint) get();
}

void Character::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            cout << get();
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

namespace mk {
    shared_ptr<Character> character(char x) {
        return make_shared<Character>()->set(x);
    }
}