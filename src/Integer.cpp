
#include <iostream>

#include "../include/Define.h"
#include "../include/Random.h"
#include "../include/Integer.h"
#include "../include/Destroy.h"
#include "../include/IntegerWrapper.h"

using namespace std;
using namespace Random;
using namespace Format;

Integer::Integer() {
    panel = new IntegerPanel(this);
    l = new IntegerWrapper();
    r = new IntegerWrapper();
    fmt = "$x";
}

Integer::Integer(const Integer& other) {
    panel = new IntegerPanel(this);
    l = other.l;
    r = other.r;
    fmt = other.fmt;
}

Integer::~Integer() {
#ifdef OUTPUT_DELETER
    cout << "delete integer\n";
#endif
}

Integer* Integer::lower_bound(int x) {
    l->value(x);
    return this;
}

Integer* Integer::upper_bound(int x) {
    r->value(x);
    return this;
}

Integer* Integer::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

IntegerPanel* Integer::get_panel() {
    return panel;
}

void Integer::generate(bool re) {
    if (generated && !re) return;
    generated = true;
    
    CL_GENERATE(l);
    CL_GENERATE(r);
#ifdef DEBUG_INTEGER_FLAG
    cout << "l=" << l->get() << " r=" << r->get() << "\n";
#endif
    value = rand_int(l->get(), r->get());
}

Node* Integer::clone() {
    if (type == STRUCTURE_NODE)
        return (Node*)new Integer(*this);
    return this;
}

void Integer::destroy() {
    if (destroyed) return;
    destroyed = true;

    delete panel;
    Destroy::get()->add(this);
    CL_DESTROY(l);
    CL_DESTROY(r);
}

void Integer::out() {
    CHECK_STRING_UNSET(Integer, fmt);
    Format::parse(this, fmt, "Integer", false);
}

bool Integer::equal(Node* o) {
    Integer* other = dynamic_cast<Integer*>(o);
    if (other == nullptr) return false;
    return value == other->value;
}

void Integer::parse(const string& spec, int n, ...) {
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            cout << value;
        } else {
            MESSAGE_NOT_FOUND_IN_FORMAT(Integer, spec);
        }
    }
}

void Integer::parse_start() {
    cur_iter = 0;
}

void Integer::parse_next() {
    cur_iter++;
}

bool Integer::parse_finish() {
    return cur_iter > 0;
}

bool Integer::is_last() {
    return true;
}

IntegerPanel::IntegerPanel(Integer* parent) {
    this->parent = parent;
}

int IntegerPanel::get() {
    return parent->value;
}

void IntegerPanel::set(int x) {
    parent->value = x;
}

bool IntegerPanel::equal(IntegerPanel* other) {
    return parent->equal(other->parent);
}
