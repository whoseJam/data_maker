
#include <iostream>

#include "../include/Tuple.h"
#include "../include/Define.h"
#include "../include/Format.h"
#include "../include/Destroy.h"

using namespace std;
using namespace Format;

Tuple::Tuple() {
    len = 0;
    fmt = "$x";
}

Tuple::Tuple(const Tuple& other) {
    len = other.len;
    fmt = other.fmt;
    for (Node* ele : other.elements) 
        elements.push_back(ele->clone());
}

Tuple::~Tuple() {
#ifdef OUTPUT_DELETER
    cout << "delete tuple\n";
#endif
}

Tuple* Tuple::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

void Tuple::generate(bool re) {
    if (generated && !re) return;
    generated = true;

    CL_GENERATE_ITERABLE(elements);
}

Node* Tuple::clone() {
    return (Node*)new Tuple(*this);
}

void Tuple::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    CL_DESTROY_ITERABLE(elements);
}

void Tuple::out() {
    CHECK_STRING_UNSET(Tuple, fmt);
    Format::parse(this, fmt, "Tuple", false);
}

bool Tuple::equal(Node* o) {
    Tuple* other = dynamic_cast<Tuple*>(o);
    if (other == nullptr) return false;
    if (elements.size() != other->elements.size()) return false;
    for (int i = 0; i < elements.size(); i++)
        if (!elements[i]->equal(other->elements[i])) return false;
    return true;
}

void Tuple::parse(const string& spec, int n, ...) {
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            CHECK_FUNCTION_ARGS_MATCH(Tuple, spec, n, 0);
            elements[cur_iter]->out();
        } else {
            MESSAGE_NOT_FOUND_IN_FORMAT(Tuple, spec);
        }
        va_end(valist);
    }
}

void Tuple::parse_start() {
    cur_iter = 0;
}

void Tuple::parse_next() {
    cur_iter++;
}

bool Tuple::parse_finish() {
    return cur_iter >= elements.size();
}

bool Tuple::is_last() {
    return cur_iter == elements.size() - 1;
}