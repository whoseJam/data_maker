
#include <iostream>

#include "Clone.h"
#include "Logger.h"
#include "Random.h"
#include "Character.h"

using namespace std;
using namespace Random;

Character::Character() {
    CALL("Character", "Character");
    char_val_is_set = false;
    fmt = "$x";
}

Character::Character(const Character& other) :
    Node(other),
    Formatable(other) {
    CALL("Character", "Character");
    char_val = other.char_val;
    char_val_is_set = other.char_val_is_set;
    if (other.ptr_val)
        ptr_val = dynamic_pointer_cast<Character>(other.ptr_val->clone());
    if (other.l) l = dynamic_pointer_cast<Character>(other.l->clone());
    if (other.r) r = dynamic_pointer_cast<Character>(other.r->clone());
    fmt = other.fmt;
}

Character::~Character() {
#ifdef OUTPUT_DELETER
    cout << "delete character\n";
#endif
}

shared_ptr<Character> Character::lower_bound(char x) {
    CALL("Character", "lower_bound");
    if (!l) l = make_shared<Character>();
    l->set(x);
    return dynamic_pointer_cast<Character>(shared_from_this());
}

shared_ptr<Character> Character::upper_bound(char x) {
    CALL("Character", "upper_bound");
    if (!r) r = make_shared<Character>();
    r->set(x);
    return dynamic_pointer_cast<Character>(shared_from_this());
}

shared_ptr<Character> Character::format(const string& fmt) {
    CALL("Character", "format");
    this->fmt = fmt;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

char Character::get() {
    CALL("Character", "get");
    if (ptr_val) return ptr_val->get();
    if (char_val_is_set || generated) return char_val;
    MESSAGE("Character", "get failed because no value is set");
}

shared_ptr<Character> Character::get_lower_bound() {
    CALL("Character", "get_lower_bound");
    if (!l) MESSAGE("Character", NEED("lower_bound"));
    return l;
}

shared_ptr<Character> Character::get_upper_bound() {
    CALL("Character", "get_upper_bound");
    if (!r) MESSAGE("Character", NEED("upper_bound"));
    return r;
}

shared_ptr<Character> Character::set(char x) {
    CALL("Character", "set");
    char_val = x;
    char_val_is_set = true;
    return dynamic_pointer_cast<Character>(shared_from_this());
}

void Character::generate(bool re) {
    CALL("Character", "generate");
    if (generated && !re) return;
    generated = true;
    
    if (ptr_val) {
        if (l) MESSAGE("Character", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Character", CONFLICT("set", "upper_bound"));
        ptr_val->generate(re);
    } else if (char_val_is_set) {
        if (l) MESSAGE("Character", CONFLICT("set", "lower_bound"));
        if (r) MESSAGE("Character", CONFLICT("set", "upper_bound"));
    } else {
        if (!l) MESSAGE("Character", NEED("lower_bound"));
        if (!r) MESSAGE("Character", NEED("upper_bound"));
        l->generate(re); r->generate(re);
        if (l->get() > r->get()) MESSAGE("Character", ENSURE("l < r"));
        char_val = rand_char(l->get(), r->get());
    }
}

CL_CLONE(Character);

void Character::out() {
    CALL("Character", "out");
    Formatable::parse(
        shared_from_this(), fmt, "Character");
}

bool Character::equal(shared_ptr<Hashable> o) {
    CALL("Character", "equal");
    shared_ptr<Character> other = dynamic_pointer_cast<Character>(o);
    if (!other) return false;
    return get() == other->get();
}

uint Character::hash_code() {
    CALL("Character", "hash_code");
    return (uint) get();
}

void Character::parse(const string& spec, int n, ...) {
    CALL("Character", "parse");
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