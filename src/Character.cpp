
#include <iostream>

#include "../include/Define.h"
#include "../include/Random.h"
#include "../include/Destroy.h"
#include "../include/Character.h"

using namespace std;

Character::Character() {
    l = UNSET;
    r = UNSET;
}

Character::Character(const Character& other) {
    l = other.l;
    r = other.r;
}

Character::~Character() {
#ifdef OUTPUT_DELETER
    cout << "delete character\n";
#endif
}

Character* Character::lower_bound(char x) {
    l = x;
    return this;
}

Character* Character::upper_bound(char x) {
    r = x;
    return this;
}

void Character::generate(bool re) {
    if (generated && !re) return;
    generated = true;
    
    value = Random::rand_char(l, r);
}

Node* Character::clone() {
    if (type == STRUCTURE_NODE)
        return (Node*)new Character(*this);
    return this;
}

void Character::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
}

void Character::out() {
    cout << value;
}

bool Character::equal(Node* o) {
    Character* other = dynamic_cast<Character*>(o);
    if (other == nullptr) return false;
    return value == other->value;
}