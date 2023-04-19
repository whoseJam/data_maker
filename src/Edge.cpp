
#include <iostream>

#include "../include/Edge.h"
#include "../include/Clone.h"
#include "../include/Define.h"
#include "../include/Destroy.h"

using namespace std;

Edge::Edge() {
    start = UNSET;
    end = UNSET;
}

Edge::Edge(const Edge& other) {
    for (auto attr : other.attrs) {
        attrs.push_back((Attribute*)attr->clone());
    }
}

Edge* Edge::add_attribute(Attribute* attr) {
    attrs.push_back(attr);
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
    cout << start << " " << end << " ";
    for (auto attr : attrs) {
        attr->out();
        cout << " ";
    }
    cout << "\n";
}