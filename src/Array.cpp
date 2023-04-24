#include <iostream>

#include "../include/Array.h"
#include "../include/Format.h"
#include "../include/Define.h"
#include "../include/Destroy.h"

using namespace std;
using namespace Format;

Array::Array() {
    len = new IntegerWrapper();
    tmpl_ele = nullptr;
    fmt = "$x";
}

Array::Array(const Array& other) {
    len = (IntegerWrapper*)other.len->clone();
    tmpl_ele = other.tmpl_ele->clone();
    fmt = other.fmt;
}

Array::~Array() {
#ifdef OUTPUT_DELETER
    cout << "delete array\n";
#endif
}

Array* Array::length(int len) {
    this->len->value(len);
    return this;
}

Array* Array::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

Array* Array::unique() {
    unique_flag = true;
    return this;
}

void Array::generate(bool re) {
    if (generated && !re) return;
    generated = true;
    
    CHECK_NULL(Array, tmpl_ele);
    CL_GENERATE(len);
    for (int i = 1; i <= len->get(); i++)
        elements.push_back(tmpl_ele->clone());
    for (int i = 0; i < elements.size(); i++) {
        elements[i]->generate(re);
        if (unique_flag) {
            int cnt = 0;
            auto check_continue = [&]() {
                for (int j = 0; j < i; j++) {
                    if (elements[j]->equal(elements[i])) return true;
                } return false;
            };
            while(check_continue()) {
                elements[i]->generate(true); cnt++;
                if (cnt >= MAX_RETRY) MESSAGE_MAX_RETRY_EXCEED(Array);
            }
        }
    }
    CL_GENERATE_ITERABLE(elements);
}

Node* Array::clone() {
    if (type == STRUCTURE_NODE)
        return (Node*)new Array(*this);
    return this;
}

void Array::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    len->destroy();
    CL_DESTROY(len);
    CL_DESTROY(tmpl_ele);
    CL_DESTROY_ITERABLE(elements);
}

void Array::out() {
    CHECK_STRING_UNSET(Array, fmt);
    Format::parse(this, fmt, "Array", false);
}

bool Array::equal(Node* o) {
    Array* other = dynamic_cast<Array*>(o);
    if (other == nullptr) return false;
    if (!len->equal(other->len)) return false;
    for (int i = 0; i < elements.size(); i++) {
        if (!elements[i]->equal(other->elements[i])) return false;
    } return true;
}

void Array::parse(const string& spec, int n, ...) {
    ParseStack("Array");
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            CHECK_FUNCTION_ARGS_MATCH(Array, spec, n, 0);
            elements[cur_iter]->out();
        } else {
            MESSAGE_NOT_FOUND_IN_FORMAT(Array, spec);
        }
        va_end(valist);
    }
}

void Array::parse_start() {
    cur_iter = 0;
}

void Array::parse_next() {
    cur_iter++;
}

bool Array::parse_finish() {
    return cur_iter >= len->get();
}

bool Array::is_last() {
    return cur_iter == len->get() - 1;
}