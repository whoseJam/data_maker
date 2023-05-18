
#include <iostream>

#include "Clone.h"
#include "Debug.h"
#include "Vertex.h"
#include "Logger.h"

using namespace std;

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
                other.attrs[i]->clone()));
    
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
        if (attrs[i]->get_name() == name)
            return attrs[i];
    return nullptr;
}

void Vertex::set(int idx) {
    CALL(FUNCTION);
    this->idx = idx;
}

void Vertex::generate(bool re, shared_ptr<Node> from) {
    CALL(FUNCTION);
    from_node = from;
    if (generated) return;
    generated = true;

    for (int i = 0; i < attrs.size(); i++)
        attrs[i]->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
}

CL_CLONE(Vertex);

void Vertex::out() {
    CALL(FUNCTION);
    Formatable::parse(shared_from_this(), fmt, "Vertex");
}

bool Vertex::equal(shared_ptr<Hashable> o) {
    CALL(FUNCTION);
    shared_ptr<Vertex> other = dynamic_pointer_cast<Vertex>(o);
    if (!other) return false;
    if (attrs.size() != other->attrs.size()) return false;
    if (idx != other->idx) return false;
    for (int i = 0; i < attrs.size(); i++) {
        shared_ptr<Hashable> a = attrs[i];
        shared_ptr<Hashable> b = other->attrs[i];
        if (!a->equal(b)) return false;
    }
    return true;
}

uint Vertex::hash_code() {
    CALL(FUNCTION);
    uint ans = idx;
    for (int i = 0; i < attrs.size(); i++) {
        ans = ans * 17 + attrs[i]->hash_code();
    } return ans;
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
                    if (attrs[i]->get_name() == attr_name) {
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

namespace mk {
    shared_ptr<Vertex> vertex() {
        return make_shared<Vertex>();
    }
}
