
#include <iostream>

#include "../include/Define.h"
#include "../include/Destroy.h"
#include "../include/Attribute.h"

using namespace std;

Attribute::Attribute() {
    panel = new AttributePanel(this);
    key = "UNSET";
    val = nullptr;
}

Attribute::Attribute(const Attribute& other) {
    panel = new AttributePanel(this);
    key = other.key;
    val = other.val->clone();
}

Attribute::~Attribute() {
#ifdef OUTPUT_DELETER
    cout << "delete attribute\n";
#endif
}

Attribute* Attribute::name(const std::string& name) {
    key = name;
    return this;
}

AttributePanel* Attribute::get_panel() {
    return panel;
}

void Attribute::generate(bool re) {
    if (generated && !re) return;
    generated = true;
    
    CHECK_NULL(Attribute, val);
    CHECK_STRING_UNSET(Attribute, key);
    CL_GENERATE(val);
}

Node* Attribute::clone() {
    if (type == STRUCTURE_NODE)
        return (Node*)new Attribute(*this);
    return this;
}

void Attribute::destroy() {
    if (destroyed) return;
    destroyed = true;

    delete panel;
    Destroy::get()->add(this);
    CL_DESTROY(val);
}

void Attribute::out() {
    val->out();
}

bool Attribute::equal(Node* o) {
    Attribute* other = dynamic_cast<Attribute*>(o);
    if (other == nullptr) return false;
    return val->equal(other->val);
}

AttributePanel::AttributePanel(Attribute* parent) {
    this->parent = parent;
}

Node* AttributePanel::get() {
    return parent->val;
}