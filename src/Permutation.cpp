#include <iostream>

#include "../include/Clone.h"
#include "../include/Define.h"
#include "../include/Random.h"
#include "../include/Destroy.h"
#include "../include/Permutation.h"

using namespace std;

Permutation::Permutation() {
    len = UNSET;
}

Permutation::Permutation(const Permutation& other) {
    len = other.len;
}

Permutation* Permutation::length(int len) {
    this->len = len;
    return this;
}

void Permutation::generate() {
    if (generated) return;
    generated = true;

    CHECK_UNSET(Permutation, len);
    for (int i = 1; i <= len; i++)
        elements.push_back(i);
    Random::rand_vector(elements);
}

Node* Permutation::clone() {
    if (!Clone::get()->check(this))
        Clone::get()->insert(this, new Permutation(*this));
    return (Node*)Clone::get()->check(this);
}

void Permutation::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
}

void Permutation::out() {
    for (auto ele : elements) {
        cout << ele << " ";
    }
}