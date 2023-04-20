
#include <iostream>

#include "../include/Edge.h"
#include "../include/Clone.h"
#include "../include/Define.h"
#include "../include/Destroy.h"

using namespace std;

Edge::Edge() {
    start = UNSET;
    end = UNSET;
    fmt = "$s $t";
}

Edge::Edge(const Edge& other) {
    for (auto attr : other.attrs) {
        attrs.push_back((Attribute*)attr->clone());
    }
}

Edge* Edge::add_attribute(Attribute* attr) {
    attrs.push_back(attr);
    return this;
}

Edge* Edge::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

void Edge::__set_start_and_end(int start, int end) {
    this->start = start;
    this->end = end;
}

void Edge::generate() {
    if (generated) return;
    generated = true;
    
    for (auto attr : attrs) {
        attr->generate();
    }
}

Node* Edge::clone() {
    if (!Clone::get()->check(this))
        Clone::get()->insert(this, new Edge(*this));
    return (Node*)Clone::get()->check(this);
}

void Edge::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    for (Attribute* attr : attrs) {
        attr->destroy();
    }
}

void Edge::out() {
    CHECK_STRING_UNSET(Edge, fmt);
    Format::parse(fmt, this);
}

void Edge::parse(const std::string& spec, ...) {
    va_list valist;
    va_start(valist, spec);
    if (spec == SPEC_START) {
        cout << start;
    } else if (spec == SPEC_TO) {
        cout << end;
    } else if (spec == SPEC_ATTR) {
        HANDLE_SPEC_A(valist, attrs);
    } else {
        MESSAGE_NOT_FOUND_IN_FORMAT(Edge, spec);
    }
    va_end(valist);
}

void Edge::parse_start() {
    cur_iter = 0;
}

void Edge::parse_next() {
    cur_iter++;
}

bool Edge::parse_finish() {
    return cur_iter > 0;
}