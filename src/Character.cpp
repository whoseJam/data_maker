
#include "../include/Character.h"
#include "../include/Define.h"
#include "../include/Random.h"
#include <iostream>
using namespace std;

Character::Character() {
    gflag = UNSET;
    l = UNSET;
    r = UNSET;
}

Character::Character(const Character& other) {
    gflag = other.gflag;
    l = other.l;
    r = other.r;
}

Character* Character::lower_bound(char x) {
    if (gflag != UNSET &&
        gflag != RANDOM_FROM_RANGE) {
        cout << "Character::gflag dismatch\n";
        exit(-1);
    }
    gflag = RANDOM_FROM_RANGE;
    l = x;
    if (r != UNSET) {
        if (l > r) {
            cout << "Character::lower_bound bigger than upper_bound\n";
            exit(-1);
        }
    }
    return this;
}

Character* Character::upper_bound(char x) {
    if (gflag != UNSET && 
        gflag != RANDOM_FROM_RANGE) {
        cout << "Character::gflag dismatch\n";
        exit(-1);
    }
    gflag = RANDOM_FROM_RANGE;
    r = x;
    if (l != UNSET) {
        if (l > r) {
            cout<< "Character::upper_bound smaller than lower_bound\n";
            exit(-1);
        }
    }
    return this;
}

void Character::generate() {
    switch (gflag)
    {
    case RANDOM_FROM_RANGE:
        value = Random::rand_char(l, r);
        break;
    case RANDOM_FROM_SET:
        value = Random::rand_char('a', 'z');
        break;
    default:
        break;
    }
}

Node* Character::clone() {
    return new Character(*this);
}

void Character::out() {
    cout << value;
}