#include "../include/Permutation.h"
#include "../include/Define.h"
#include "../include/Random.h"
#include <iostream>
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
    CHECK_UNSET(Permutation, len);
    for (int i = 1; i <= len; i++)
        elements.push_back(i);
    Random::rand_vector(elements);
}

Node* Permutation::clone() {
    return new Permutation(*this);
}

void Permutation::out() {
    for (auto ele : elements) {
        cout << ele << " ";
    }
}