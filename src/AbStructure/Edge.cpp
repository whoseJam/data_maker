
#include <iostream>

#include "Edge.h"
#include "Clone.h"
#include "Debug.h"
#include "Define.h"
#include "Logger.h"

using namespace std;

Edge::Edge() {
    CALL(FUNCTION);
    start = UNSET;
    end = UNSET;
    fmt = "$s $t\n";
}

Edge::Edge(const Edge& other) :
    Node(other),
    Formatable(other) {
    CALL(FUNCTION);
    for (int i = 0; i < other.attrs.size(); i++)
        attrs.push_back(
            dynamic_pointer_cast<Attribute>(
                other.attrs[i]->clone()));
    start = other.start;
    end = other.end;
    fmt = other.fmt;
}

shared_ptr<Edge> Edge::format(const string& fmt) {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Edge>(shared_from_this());
}

int Edge::get_start() {
    CALL(FUNCTION);
    return start;
}

int Edge::get_end() {
    CALL(FUNCTION);
    return end;
}

shared_ptr<Attribute> Edge::get(const string& name) {
    CALL(FUNCTION);
    for (int i = 0; i < attrs.size(); i++) 
        if (attrs[i]->get_name() == name) return attrs[i];
    return nullptr;
}

void Edge::set(int s, int e) {
    CALL(FUNCTION);
    start = s;
    end = e;
}

void Edge::generate(bool re, shared_ptr<Node> from) {
    CALL(FUNCTION);
    from_node = from;
    if (generated && !re) return;
    generated = true;
    
    for (int i = 0; i < attrs.size(); i++) {
        attrs[i]->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
    }
}

CL_CLONE(Edge);

void Edge::out() {
    CALL(FUNCTION);
    Formatable::parse(shared_from_this(), fmt, "Edge");
}

bool Edge::equal(shared_ptr<Hashable> o) {
    CALL(FUNCTION);
    shared_ptr<Edge> other = dynamic_pointer_cast<Edge>(o);
    if (!other) return false;
    if (attrs.size() != other->attrs.size()) return false;
    if (start != other->start) return false;
    if (end != other->end) return false;
    for (int i = 0; i < attrs.size(); i++) {
        shared_ptr<Hashable> a = attrs[i];
        shared_ptr<Hashable> b = other->attrs[i];
        if (!a->equal(b)) return false;
    }
    return true;
}

uint Edge::hash_code() {
    CALL(FUNCTION);
    uint ans = start * 17 + end;
    for (int i = 0; i < attrs.size(); i++) {
        ans = ans * 5 + attrs[i]->hash_code();
    } return ans;
}

void Edge::parse(const std::string& spec, int n, ...) {
    CALL(FUNCTION);
    va_list valist;
    va_start(valist, n);
    if (spec == SPEC_START) {
        cout << start;
    } else if (spec == SPEC_TO) {
        cout << end;
    } else if (spec == SPEC_SELF) {
        if (n == 1) {
            string attr_name = va_arg(valist, char*);
            for (int i = 0; i < attrs.size(); i++) {
                if (attrs[i]->get_name() == attr_name) {
                    attrs[i]->out();
                }
            }
        }
    } else {
        MESSAGE("Edge", FUNC_NOT_FOUND(spec));
    }
    va_end(valist);
}

namespace mk {
    shared_ptr<Edge> edge() {
        return make_shared<Edge>();
    }
}