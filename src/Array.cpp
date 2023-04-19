#include <iostream>

#include "../include/Clone.h"
#include "../include/Array.h"
#include "../include/Format.h"
#include "../include/Define.h"
#include "../include/Destroy.h"

using namespace std;
using namespace Format;

Array::Array(): len("Array", "len") {
    element = nullptr;
    fmt = "$x";
}

Array::Array(const Array& other): len(other.len) {
    element = other.element;
    fmt = other.fmt;
}

Array::~Array() {
#ifdef OUTPUT_DELETER
    cout << "delete array\n";
#endif
}

Array* Array::length(int len) {
    this->len.set(len);
    return this;
}

Array* Array::length(Integer* len) {
    this->len.set(len);
    return this;
}

Array* Array::fill(Node* ele) {
    element = ele;
    return this;
}

Array* Array::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

void Array::generate() {
    if (generated) return;
    generated = true;
    
    CHECK_NULL(Array, element);
    len.generate();
    for (int i = 1; i <= len.get(); i++) {
        Clone::get()->prepare();
        elements.push_back(element->clone());
    }
    for (auto ele : elements) 
        ele->generate();
}

Node* Array::clone() {
    if (!Clone::get()->check(this))
        Clone::get()->insert(this, new Array(*this));
    return (Node*)Clone::get()->check(this);
}

void Array::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    element->destroy();
    for (auto ele : elements)
        ele->destroy();
}

void Array::out() {
    CHECK_STRING_UNSET(Array, fmt);
    Format::parse(fmt, this);
}

void Array::parse(const string& spec, ...) {
    va_list valist;
    va_start(valist, spec);
    IF_SPEC_IS_LAST(valist, spec, cur_iter, elements.size() - 1)
    else IF_SPEC_IS_NLAST(valist, spec, cur_iter, elements.size() - 1)
    else if (spec == "x") {
        elements[cur_iter]->out();
    } else {
        MESSAGE_NOT_FOUND_IN_FORMAT(Array, spec);
    }
    va_end(valist);
}

void Array::parse_start() {
    cur_iter = 0;
}

void Array::parse_next() {
    cur_iter++;
}

bool Array::parse_finish() {
    return cur_iter >= len.get();
}