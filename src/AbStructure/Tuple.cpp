
#include <iostream>

#include "Clone.h"
#include "Tuple.h"
#include "Debug.h"

using namespace std;

Tuple::Tuple() {
    CALL(FUNCTION);
    fmt = "$x ";
}

Tuple::Tuple(const Tuple& other) :
    Node(other),
    Formatable(other) {
    CALL(FUNCTION);
    callback_when_generating_per_element = other.callback_when_generating_per_element;
    callback_after_generate = other.callback_after_generate;
    for (auto ele : other.elements) 
        elements.push_back(ele->clone());
}

Tuple::~Tuple() {
#ifdef OUTPUT_DELETER
    cout << "delete tuple\n";
#endif
}

shared_ptr<Tuple> Tuple::format(const string& fmt) {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Tuple>(shared_from_this());
}

shared_ptr<Tuple> Tuple::when_generating_per_element(
    function<void(shared_ptr<Tuple>, int)> callback) {
    CALL(FUNCTION);
    callback_when_generating_per_element = callback;
    return dynamic_pointer_cast<Tuple>(shared_from_this());
}

shared_ptr<Tuple> Tuple::after_generate(
    function<void(shared_ptr<Tuple>)> callback) {
    CALL(FUNCTION);
    callback_after_generate = callback;
    return dynamic_pointer_cast<Tuple>(shared_from_this());
}

void Tuple::generate(bool re, shared_ptr<Node> from) {
    CALL(FUNCTION);
    from_node = from;
    if (generated && !re) return;
    generated = true;

    for (int i = 0; i < elements.size(); i++) {
        elements[i]->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
        if (callback_when_generating_per_element)
            callback_when_generating_per_element(
                dynamic_pointer_cast<Tuple>(shared_from_this()), i);
    }
    if (callback_after_generate)
        callback_after_generate(
            dynamic_pointer_cast<Tuple>(shared_from_this()));
}

CL_CLONE(Tuple);

bool Tuple::equal(shared_ptr<Hashable> o) {
    CALL(FUNCTION);
    shared_ptr<Tuple> other = dynamic_pointer_cast<Tuple>(o);
    if (!other) return false;
    if (elements.size() != other->elements.size()) return false;
    for (int i = 0; i < elements.size(); i++) {
        shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(elements[i]);
        shared_ptr<Hashable> b = dynamic_pointer_cast<Hashable>(other->elements[i]);
        if (!a->equal(b)) return false;
    }
    return true;
}

uint Tuple::hash_code() {
    CALL(FUNCTION);
    uint ans = elements.size();
    for (int i = 0; i < elements.size(); i++) {
        shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(elements[i]);
        ans = ans * 17 + a->hash_code();
    }
    return ans;
}

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
