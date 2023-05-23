
#include <functional>
#include <iostream>
#include <memory>

#include "Node.h"
#include "Tuple.h"
#include "Debug.h"

using namespace std;

namespace mk {

int COUNT_TUPLE = 0;

Tuple::Tuple() {
    COUNT_TUPLE++;
    fmt = "$x ";
}

Tuple::Tuple(const Tuple& other) :
    Node(other),
    Formatable(other) {
    COUNT_TUPLE++;
    callback_before_generate = other.callback_before_generate;
    callback_when_generating = other.callback_when_generating;
    callback_after_generate = other.callback_after_generate;
    for (auto ele : other.elements) 
        elements.push_back(ele->clone(0));
}

Tuple::~Tuple() {
    COUNT_TUPLE--;
}

auto Tuple::append(shared_ptr<Node> ele) -> shared_ptr<Tuple> {
    CALL(FUNCTION);
    elements.push_back(ele);
    return dynamic_pointer_cast<Tuple>(shared_from_this());
}

auto Tuple::before_generate(
    function<void(shared_ptr<Tuple>)> callback) -> shared_ptr<Tuple> {
    CALL(FUNCTION);
    callback_before_generate = callback;
    return dynamic_pointer_cast<Tuple>(shared_from_this());
}

auto Tuple::when_generating(
    function<void(shared_ptr<Tuple>, int)> callback) -> shared_ptr<Tuple> {
    CALL(FUNCTION);
    callback_when_generating = callback;
    return dynamic_pointer_cast<Tuple>(shared_from_this());
}

auto Tuple::after_generate(
    function<void(shared_ptr<Tuple>)> callback) -> shared_ptr<Tuple> {
    CALL(FUNCTION);
    callback_after_generate = callback;
    return dynamic_pointer_cast<Tuple>(shared_from_this());
}

auto Tuple::format(const char* fmt) -> shared_ptr<Tuple> {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Tuple>(shared_from_this());
}

extern bool gdebug;
auto Tuple::generate(bool re) -> void {
    CALL(FUNCTION);
    gdebug = true;
    GENERATE;
    gdebug = false;
    if (generated && !re) return;
    generated = true;

    if (callback_before_generate)
        callback_before_generate(
            dynamic_pointer_cast<Tuple>(shared_from_this()));
    for (int i = 0; i < elements.size(); i++) {
        elements[i]->generate(re);
        if (callback_when_generating)
            callback_when_generating(
                dynamic_pointer_cast<Tuple>(shared_from_this()), i);
    }
    if (callback_after_generate)
        callback_after_generate(
            dynamic_pointer_cast<Tuple>(shared_from_this()));
}

CL_CLONE(Tuple);

void Tuple::iter_reset() {
    CALL(FUNCTION);
    cur_iter = 0;
}

void Tuple::iter_next() {
    CALL(FUNCTION);
    cur_iter++;
}

bool Tuple::iter_loop_finish() {
    CALL(FUNCTION);
    return cur_iter >= elements.size();
}

bool Tuple::iter_at_last() {
    CALL(FUNCTION);
    return cur_iter == elements.size() - 1;
}

void Tuple::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            if (!IN_RANGE(0, n, 0)) MESSAGE("Tuple", FUNC_ARGS_MISMATCH(n, 0, 0));
            auto ele = dynamic_pointer_cast<Formatable>(elements[cur_iter]);
            if (!ele) MESSAGE("Tuple", UNABLE("Formatable"));
            ele->out();
        } else {
            MESSAGE("Tuple", FUNC_NOT_FOUND(spec));
        }
        va_end(valist);
    }
}

void Tuple::out() {
    CALL(FUNCTION);
    Formatable::parse(shared_from_this(), fmt, "Tuple");
}

auto tuple() -> shared_ptr<Tuple> {
    return make_shared<Tuple>();
}

auto tuple(shared_ptr<Node> a) -> shared_ptr<Tuple> {
    return tuple()->append(a);
}

auto tuple(shared_ptr<Node> a, shared_ptr<Node> b) -> shared_ptr<Tuple> {
    return tuple(a)->append(b);
}

auto tuple(shared_ptr<Node> a, shared_ptr<Node> b, shared_ptr<Node> c) -> shared_ptr<Tuple> {
    return tuple(a, b)->append(c);
}

auto tuple(shared_ptr<Node> a, shared_ptr<Node> b, shared_ptr<Node> c, shared_ptr<Node> d) -> shared_ptr<Tuple> {
    return tuple(a, b, c)->append(d);
}

auto tuple(shared_ptr<Node> a, shared_ptr<Node> b, shared_ptr<Node> c, shared_ptr<Node> d, shared_ptr<Node> e) -> shared_ptr<Tuple> {
    return tuple(a, b, c, d)->append(e);
}

}