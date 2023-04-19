
#include <iostream>

#include "../include/Clone.h"
#include "../include/Define.h"
#include "../include/Destroy.h"
#include "../include/Attribute.h"

using namespace std;

Attribute::Attribute() {
    key = "UNSET";
    val = nullptr;
}

Attribute::Attribute(const Attribute& other) {
    key = other.key;
    val = other.val->clone();
}

Attribute::~Attribute() {
#ifdef OUTPUT_DELETER
    cout << "delete attribute\n";
#endif
}

Attribute* Attribute::name(const std::string& name) {
    key = name;
    return this;
}

Attribute* Attribute::value(Integer* integer) {
    val = integer;
    return this;
}

Attribute* Attribute::value(Character* character) {
    val = character;
    return this;
}

void Attribute::generate() {
    if (generated) return;
    generated = true;
    
    val->generate();
}

Node* Attribute::clone() {
    if (!Clone::get()->check(this))
        Clone::get()->insert(this, new Attribute(*this));
    return (Node*)Clone::get()->check(this);
}

void Attribute::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    val->destroy();
}

void Attribute::out() {
    val->out();
}

std::string Attribute::__get_key() {
    return key;
}