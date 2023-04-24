#include <iostream>

#include "../include/Define.h"
#include "../include/Random.h"
#include "../include/Destroy.h"
#include "../include/Permutation.h"

using namespace std;

Permutation::Permutation() {
    fmt = "$x";
}

Permutation::Permutation(const Permutation& other) {
    fmt = other.fmt;
}

Permutation* Permutation::length(int len) {
    this->len->value(len);
    return this;
}

Permutation* Permutation::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

void Permutation::generate(bool re) {
    if (generated && !re) return;
    generated = true;

    CL_GENERATE(len);
    for (int i = 1; i <= len->get(); i++)
        elements.push_back(i);
    Random::rand_vector(elements);
}

Node* Permutation::clone() {
    if (type == STRUCTURE_NODE)
        return (Node*)new Permutation(*this);
    return this;
}

void Permutation::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    CL_DESTROY(len);
}

void Permutation::out() {
    CHECK_STRING_UNSET(Permutation, fmt);
    Format::parse(this, fmt, "Permutation", false);
}

bool Permutation::equal(Node* o) {
    Permutation* other = dynamic_cast<Permutation*>(o);
    if (other == nullptr) return false;
    if (!len->equal(other->len)) return false;
    for (int i = 0; i < elements.size(); i++) {
        if (elements[i] != other->elements[i]) return false;
    } return true;
}

void Permutation::parse(const string& spec, int n, ...) {
    ParseStack ps("Permutation");
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            CHECK_FUNCTION_ARGS_MATCH(Permutation, spec, n, 1);
            cout << elements[cur_iter];
        } else {
            MESSAGE_NOT_FOUND_IN_FORMAT(Permutation, spec);
        }
        va_end(valist);
    }
}

void Permutation::parse_start() {
    cur_iter = 0;
}

void Permutation::parse_next() {
    cur_iter++;
}

bool Permutation::parse_finish() {
    return cur_iter >= elements.size();
}
