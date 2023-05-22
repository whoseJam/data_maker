
#include <iostream>

#include "Edge.h"
#include "Debug.h"
#include "Define.h"
#include "Logger.h"
#include "Node.h"

using namespace std;

namespace mk {

Edge::Edge() {
    CALL(FUNCTION);
    start_ = UNSET;
    end_ = UNSET;
    fmt = "$s $t\n";
}

Edge::Edge(const Edge& other) :
    Node(other),
    Formatable(other) {
    CALL(FUNCTION);
    for (int i = 0; i < other.attrs.size(); i++)
        attrs.push_back(
            dynamic_pointer_cast<Attribute>(
                other.attrs[i]->clone(0)));
    start_ = other.start_;
    end_ = other.end_;
    fmt = other.fmt;
}

shared_ptr<Edge> Edge::format(const string& fmt) {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Edge>(shared_from_this());
}

int Edge::start() {
    CALL(FUNCTION);
    return start_;
}

int Edge::end() {
    CALL(FUNCTION);
    return end_;
}

auto Edge::attr(const string& name) -> shared_ptr<Attribute> {
    CALL(FUNCTION);
    for (int i = 0; i < attrs.size(); i++) 
        if (attrs[i]->name() == name) return attrs[i];
    return nullptr;
}

auto Edge::start(int s) -> void {
    CALL(FUNCTION);
    start_ = s;
}

void Edge::generate(bool re) {
    CALL(FUNCTION);
    GENERATE;
    if (generated && !re) return;
    generated = true;
    
    for (int i = 0; i < attrs.size(); i++) {
        attrs[i]->generate(re);
    }
}

CL_CLONE(Edge);

void Edge::out() {
    CALL(FUNCTION);
    Formatable::parse(shared_from_this(), fmt, "Edge");
}

void Edge::parse(const std::string& spec, int n, ...) {
    CALL(FUNCTION);
    va_list valist;
    va_start(valist, n);
    if (spec == SPEC_START) {
        cout << start_;
    } else if (spec == SPEC_TO) {
        cout << end_;
    } else if (spec == SPEC_SELF) {
        if (n == 1) {
            string attr_name = va_arg(valist, char*);
            for (int i = 0; i < attrs.size(); i++) {
                if (attrs[i]->name() == attr_name) {
                    attrs[i]->out();
                }
            }
        }
    } else {
        MESSAGE("Edge", FUNC_NOT_FOUND(spec));
    }
    va_end(valist);
}

shared_ptr<Edge> edge() {
    return make_shared<Edge>();
}

}