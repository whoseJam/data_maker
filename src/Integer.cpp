
#include "../include/Integer.h"
#include "../include/Define.h"
#include "../include/Random.h"
#include <iostream>
using namespace std;

Integer::Integer() {
    gflag = UNSET;
    l = UNSET;
    r = UNSET;
}

Integer::Integer(const Integer& other) {
    gflag = other.gflag;
    l = other.l;
    r = other.r;
}

Integer* Integer::lower_bound(int x) {
    if (gflag != UNSET &&
        gflag != RANDOM_FROM_RANGE) {
        cout << "Integer::gflag dismatch\n";
        exit(-1);
    }
    gflag = RANDOM_FROM_RANGE;
    l = x;
    if (r != UNSET) {
        if (l > r) {
            cout << "Integer::lower_bound bigger than upper_bound\n";
            exit(-1);
        }
    }
    return this;
}

Integer* Integer::upper_bound(int x) {
    if (gflag != UNSET && 
        gflag != RANDOM_FROM_RANGE) {
        cout << "Integer::gflag dismatch\n";
        exit(-1);
    }
    gflag = RANDOM_FROM_RANGE;
    r = x;
    if (l != UNSET) {
        if (l > r) {
            cout<< "Integer::upper_bound smaller than lower_bound\n";
            exit(-1);
        }
    }
    return this;
}

void Integer::generate() {
    switch (gflag)
    {
    case RANDOM_FROM_RANGE:
        value = Random::rand_int(l, r);
        break;
    case RANDOM_FROM_SET:
        value = Random::rand_int(0, 100);
        break;
    default:
        break;
    }
}

Node* Integer::clone() {
    return new Integer(*this);
}

void Integer::out() {
    cout << value;
}