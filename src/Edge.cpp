
#include <iostream>

#include "../include/Edge.h"
#include "../include/Define.h"
#include "../include/Destroy.h"

using namespace std;

Edge::Edge() {
    panel = new EdgePanel(this);
    start = UNSET;
    end = UNSET;
    attrs = new AttributeGroup();
    fmt = "$s $t";
}

Edge::Edge(const Edge& other) {
    panel = new EdgePanel(this);
    attrs = (AttributeGroup*)other.attrs->clone();
    fmt = other.fmt;
}

Edge* Edge::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

EdgePanel* Edge::get_panel() {
    return panel;
}

Edge* Edge::__set_start_and_end(int start, int end) {
    this->start = start;
    this->end = end;
    return this;
}

void Edge::generate(bool re) {
    if (generated && !re) return;
    generated = true;
    
    CL_GENERATE(attrs);
}

Node* Edge::clone() {
    if (type == STRUCTURE_NODE)
        return (Node*)new Edge(*this);
    return this;
}

void Edge::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    CL_DESTROY(attrs);
}

void Edge::out() {
    CHECK_STRING_UNSET(Edge, fmt);
    Format::parse(this, fmt, "Edge", false);
}

bool Edge::equal(Node* o) {
    Edge* other = dynamic_cast<Edge*>(o);
    if (other == nullptr) return false;
    return (start == other->start && end == other->end && attrs->equal(other->attrs));
}

void Edge::parse(const std::string& spec, int n, ...) {
    ParseStack("Edge");
    va_list valist;
    va_start(valist, n);
    if (spec == SPEC_START) {
        cout << start;
    } else if (spec == SPEC_TO) {
        cout << end;
    } else if (spec == SPEC_SELF) {
        if (n == 1) {
            char* attr_name = va_arg(valist, char*);
            attrs->parse(SPEC_ATTR, 1, attr_name);
        }
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

bool Edge::is_last() {
    return true;
}

EdgePanel::EdgePanel(Edge* parent) {
    this->parent = parent;
}

Attribute* EdgePanel::get(const string& name) {
    return parent->attrs->get_panel()->get(name);
}

int EdgePanel::get_start() {
    return parent->start;
}

int EdgePanel::get_end() {
    return parent->end;
}