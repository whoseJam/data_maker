
#include <iostream>

#include "../include/Clone.h"
#include "../include/Define.h"
#include "../include/Vertex.h"
#include "../include/Destroy.h"

using namespace std;

Vertex::Vertex(int idx) {
    this->idx = idx;
}

Vertex::Vertex(const Vertex& other) {
    idx = other.idx;
    for (Attribute* attr : other.attrs) {
        attrs.push_back((Attribute*)attr->clone());
    }
}

Vertex::~Vertex() {
#ifdef OUTPUT_DELETER
    cout << "delete vertex\n";
#endif
}

Vertex* Vertex::add_attribute(Attribute* attr) {
    attrs.push_back(attr);
    return this;
}

Vertex* Vertex::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

void Vertex::generate() {
    if (generated) return;
    generated = true;

    for (Attribute* attr : attrs)
        attr->generate();
}

Node* Vertex::clone() {
    if (!Clone::get()->check(this))
        Clone::get()->insert(this, new Vertex(*this));
    return (Node*)Clone::get()->check(this);
}

void Vertex::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    for (Attribute* attr : attrs)
        attr->destroy();
}

void Vertex::out() {
    CHECK_STRING_UNSET(Vertex, fmt);
    Format::parse(fmt, this);
}

void Vertex::parse(const string& spec, ...) {
    va_list valist;
    va_start(valist, spec);
    if (spec == "x") {
        cout << idx;
    } else if (spec == "a") {
        bool attr_found = false;
        string attr_name = va_arg(valist, char*);
        for (Attribute* attr : attrs) {
            if (attr_name == attr->__get_key()) {
                attr->out();
                attr_found = true;
                break;
            }
        }
        if (!attr_found) {
            MESSAGE_ATTR_NOT_FOUND_IN_FORMAT(Vertex, attr_name);
        }
    } else {
        MESSAGE_NOT_FOUND_IN_FORMAT(Vertex, spec);
    }
    va_end(valist);
}

void Vertex::parse_start() {
    cur_iter = 0;
}

void Vertex::parse_next() {
    cur_iter++;
}

bool Vertex::parse_finish() {
    return cur_iter > 0;
}