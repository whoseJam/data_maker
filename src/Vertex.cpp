
#include <iostream>

#include "../include/Define.h"
#include "../include/Vertex.h"
#include "../include/Destroy.h"

using namespace std;

Vertex::Vertex() {
    panel = new VertexPanel(this);
    idx = UNSET;
    attrs = new AttributeGroup();
    fmt = "$x";
}

Vertex::Vertex(const Vertex& other) {
    panel = new VertexPanel(this);
    idx = other.idx;
    attrs = (AttributeGroup*)other.attrs->clone();
    fmt = other.fmt;
}

Vertex::~Vertex() {
#ifdef OUTPUT_DELETER
    cout << "delete vertex\n";
#endif
}

Vertex* Vertex::__index(int idx) {
    this->idx = idx;
    return this;
}

Vertex* Vertex::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

VertexPanel* Vertex::get_panel() {
    return panel;
}

void Vertex::generate(bool re) {
    if (generated) return;
    generated = true;

    CHECK_UNSET(Vertex, idx);
    CL_GENERATE(attrs);
}

Node* Vertex::clone() {
    if (type == STRUCTURE_NODE)
        return (Node*)new Vertex(*this);
    return this;
}

void Vertex::destroy() {
    if (destroyed) return;
    destroyed = true;

    delete panel;
    Destroy::get()->add(this);
    attrs->destroy();
}

void Vertex::out() {
    CHECK_STRING_UNSET(Vertex, fmt);
    Format::parse(this, fmt, "Vertex", false);
}

bool Vertex::equal(Node* o) {
    Vertex* other = dynamic_cast<Vertex*>(o);
    if (other == nullptr) return false;
    return (idx == other->idx && attrs->equal(other->attrs));
}

void Vertex::parse(const string& spec, int n, ...) {
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            CHECK_FUNCTION_ARGS_MATCH_IN_RANGE(Vertex, spec, n, 0, 1);
            if (n == 0) cout << idx;
            else if (n == 1) {
                char* attr_name = va_arg(valist, char*);
                attrs->parse(SPEC_ATTR, 1, attr_name);
            }
        } else {
            MESSAGE_NOT_FOUND_IN_FORMAT(Vertex, spec);
        }
        va_end(valist);
    }
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

bool Vertex::is_last() {
    return true;
}

VertexPanel::VertexPanel(Vertex* parent) {
    this->parent = parent;
}

Attribute* VertexPanel::get(const string& name) {
    return parent->attrs->get_panel()->get(name);
}