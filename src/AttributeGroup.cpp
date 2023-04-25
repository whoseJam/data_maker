
#include <iostream>

#include "../include/Destroy.h"
#include "../include/AttributeGroup.h"

using namespace std;
using namespace Format;

AttributeGroup::AttributeGroup() {
    panel = new AttributeGroupPanel(this);
    fmt = "UNSET";
}

AttributeGroup::AttributeGroup(const AttributeGroup& other) {
    panel = new AttributeGroupPanel(this);
    fmt = other.fmt;
    for (auto attr : other.attrs)
        attrs.push_back((Attribute*)attr->clone());
}

AttributeGroupPanel* AttributeGroup::get_panel() {
    return panel;
}

void AttributeGroup::generate(bool re) {
    if (generated && !re) return;
    generated = true;

    CL_GENERATE_ITERABLE(attrs);
}

Node* AttributeGroup::clone() {
    if (type == STRUCTURE_NODE)
        return (AttributeGroup*)new AttributeGroup(*this);
    return this;
}

void AttributeGroup::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    CL_DESTROY_ITERABLE(attrs);
}

void AttributeGroup::out() {
    MESSAGE_FUNCTION_FORBIDDEN(AttributeGroup, "out");
}

bool AttributeGroup::equal(Node* o) { // TODO
    AttributeGroup* other = dynamic_cast<AttributeGroup*>(o);
    if (other == nullptr) return false;
    if (attrs.size() != other->attrs.size()) return false;
    for (int i = 0; i < attrs.size(); i++) {
        if (!attrs[i]->equal(other->attrs[i]))
            return false;
    }
    return true;
}

void AttributeGroup::parse(const string& spec, int n, ...) {
    ParseStack ps("AttributeGroup");
    va_list valist;
    va_start(valist, n);
    if (spec == SPEC_ATTR) {
        bool attr_found = false;
        string attr_name = va_arg(valist, char*);
        for (Attribute* attr : attrs) {
            if (attr_name == attr->key) {
                attr->out();
                attr_found = true;
                break;
            }
        }
        if (!attr_found) {
            MESSAGE_ATTR_NOT_FOUND_IN_FORMAT(AttributeGroup, spec);
        }
    }
    va_end(valist);
}

void AttributeGroup::parse_start() {
    MESSAGE_FUNCTION_FORBIDDEN(AttributeGroup, "parse_start");
}

void AttributeGroup::parse_next() {
    MESSAGE_FUNCTION_FORBIDDEN(AttributeGroup, "parse_next");
}

bool AttributeGroup::parse_finish() {
    MESSAGE_FUNCTION_FORBIDDEN(AttributeGroup, "parse_finish");
    return true;
}

bool AttributeGroup::is_last() {
    return true;
}

AttributeGroupPanel::AttributeGroupPanel(AttributeGroup* parent) {
    this->parent = parent;
}

Attribute* AttributeGroupPanel::get(const string& name) {
    for (Attribute* attr : parent->attrs) {
        if (attr->key == name) {
            return attr;
        }
    }
    return nullptr;
}