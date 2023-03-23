
#include "../include/Edge.h"
#include "../include/Define.h"
#include <iostream>
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
    for (auto attr : attrs) {
        attr->generate();
    }
}

Node* Edge::clone() {
    return new Edge(*this);
}

void Edge::out() {
    cout << start << " " << end << " ";
    for (auto attr : attrs) {
        attr->out();
        cout << " ";
    }
    cout << "\n";
}