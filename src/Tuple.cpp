
#include <iostream>

#include "../include/Clone.h"
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
    for (Node* ele : other.elements) {
        elements.push_back(ele->clone());
    }
}

Tuple::~Tuple() {
#ifdef OUTPUT_DELETER
    cout << "delete tuple\n";
#endif
}

Tuple* Tuple::append(Node* ele) {
    elements.push_back(ele);
    return this;
}

Tuple* Tuple::unshift(Node* ele) {
    elements.insert(elements.begin(), ele);
    return this;
}


Tuple* Tuple::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

void Tuple::generate() {
    if (generated) return;
    generated = true;

    for (Node* ele : elements) {
        ele->generate();
    }
}

Node* Tuple::clone() {
    if (!Clone::get()->check(this))
        Clone::get()->insert(this, new Tuple(*this));
    return (Node*)Clone::get()->check(this);
}

void Tuple::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    for (Node* ele : elements) {
        ele->destroy();
    }
}

void Tuple::out() {
    CHECK_STRING_UNSET(Tuple, fmt);
    Format::parse(fmt, this);
}

void Tuple::parse(const string& spec, ...) {
    va_list valist;
    va_start(valist, spec);
    if (spec == SPEC_LAST) {
        HANDLE_SPEC_LAST(valist, cur_iter, elements.size() - 1);
    } else if (spec == SPEC_NLAST) {
        HANDLE_SPEC_NLAST(valist, cur_iter, elements.size() - 1);
    } else if (spec == SPEC_SELF) {
        elements[cur_iter]->out();
    } else {
        MESSAGE_NOT_FOUND_IN_FORMAT(Tuple, spec);
    }
    va_end(valist);
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