
#include "../include/Attribute.h"

Attribute::Attribute() {
    key = "UNSET";
    val = nullptr;
}

Attribute::Attribute(const Attribute& other) {
    key = other.key;
    val = other.val->clone();
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
    val->generate();
}

Node* Attribute::clone() {
    return new Attribute(*this);
}

void Attribute::out() {
    val->out();
}

std::string Attribute::__get_key() {
    return key;
}