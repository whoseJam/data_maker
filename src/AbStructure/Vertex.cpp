
#include <iostream>

#include "Debug.h"
#include "Node.h"
#include "Vertex.h"
#include "Logger.h"

using namespace std;

namespace mk {

Vertex::Vertex() {
    CALL(FUNCTION);
    idx = UNSET;
}

Vertex::Vertex(const Vertex& other) :
    Node(other),
    Formatable(other) {
    CALL(FUNCTION);
    idx = other.idx;
    for (int i = 0; i < other.attrs.size(); i++)
        attrs.push_back(
            dynamic_pointer_cast<Attribute>(
                other.attrs[i]->clone(0)));
    
    fmt = other.fmt;
}

Vertex::~Vertex() {
#ifdef OUTPUT_DELETER
    cout << "delete vertex\n";
#endif
}

shared_ptr<Vertex> Vertex::format(const string& fmt) {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Vertex>(shared_from_this());
}

int Vertex::get() {
    CALL(FUNCTION);
    return idx;
}

shared_ptr<Attribute> Vertex::get(const std::string& name) {
    CALL(FUNCTION);
    for (int i = 0; i < attrs.size(); i++)
        if (attrs[i]->name() == name)
            return attrs[i];
    return nullptr;
}

void Vertex::set(int idx) {
    CALL(FUNCTION);
    this->idx = idx;
}

void Vertex::generate(bool re) {
    CALL(FUNCTION);
    GENERATE;
    if (generated) return;
    generated = true;

    for (int i = 0; i < attrs.size(); i++)
        attrs[i]->generate(re);
}

CL_CLONE(Vertex);

void Vertex::out() {
    CALL(FUNCTION);
    Formatable::parse(shared_from_this(), fmt, "Vertex");
}

void Vertex::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            if (!IN_RANGE(0, n, 1)) MESSAGE("Vertex", FUNC_ARGS_MISMATCH(n, 0, 1));
            if (n == 0) cout << idx;
            else if (n == 1) {
                string attr_name = va_arg(valist, char*);
                for (int i = 0; i < attrs.size(); i++) {
                    if (attrs[i]->name() == attr_name) {
                        attrs[i]->out();
                    }
                }
            }
        } else {
            MESSAGE("Vertex", FUNC_NOT_FOUND(spec));
        }
        va_end(valist);
    }
}

shared_ptr<Vertex> vertex() {
    return make_shared<Vertex>();
}

}
