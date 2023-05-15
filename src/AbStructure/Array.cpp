#include <utility>
#include <iostream>

#include "Debug.h"
#include "Array.h"
#include "Clone.h"
#include "Logger.h"

using namespace std;

Array::Array() {
    CALL("Array", "Array");
    fmt = "$x ";
}

Array::Array(const Array& other) : 
    Node(other), 
    Formatable(other) {
    CALL("Array", "Array");
    callback_when_generating_per_element = other.callback_when_generating_per_element;
    callback_after_generate = other.callback_after_generate;
    if (!other.len) MESSAGE("Array", NEED("length"));
    if (!other.template_ele) MESSAGE("Array", NEED("fill"));
    len = dynamic_pointer_cast<Integer>(other.len->clone());
    template_ele = other.template_ele->clone();
    for (auto ele : other.elements) 
        elements.push_back(ele->clone());
}

Array::~Array() {
#ifdef OUTPUT_DELETER
    cout << "delete array\n";
#endif
}

shared_ptr<Array> Array::length(int len) {
    CALL("Array", "length");
    if (!this->len) this->len = make_shared<Integer>();
    this->len->set(len);
    return dynamic_pointer_cast<Array>(shared_from_this());
}

shared_ptr<Array> Array::length(shared_ptr<Integer> len) {
    CALL("Array", "length");
    this->len = len;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

shared_ptr<Array> Array::fill(shared_ptr<Node> ele) {
    CALL("Array", "fill");
    this->template_ele = ele;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

shared_ptr<Array> Array::format(const string& fmt) {
    CALL("Array", "format");
    this->fmt = fmt;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

shared_ptr<Array> Array::when_generating_per_element(
    function<void(shared_ptr<Array>, int)> callback) {
    CALL("Array", "when_generating_per_element");
    callback_when_generating_per_element = callback;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

shared_ptr<Array> Array::after_generate(
    function<void(shared_ptr<Array>)> callback) {
    CALL("Array", "after_generate");
    callback_after_generate = callback;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

int Array::get_length() {
    CALL("Array", "get_length");
    if (!len) MESSAGE("Array", NEED("length"));
    return len->get();
}

void Array::generate(bool re, std::shared_ptr<Node> from) {
    CALL("Array", "generate");
    from_node = from;
    if (generated && !re) return;
    generated = true;
    
    if (!len) MESSAGE("Array", NEED("length"));
    if (!template_ele) MESSAGE("Array", NEED("fill"));
    len->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
    for (int i = 1; i <= len->get(); i++)
        elements.push_back(template_ele->clone());
    for (int i = 0; i < elements.size(); i++) {
        elements[i]->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
        if (callback_when_generating_per_element)
            callback_when_generating_per_element(
                dynamic_pointer_cast<Array>(shared_from_this()), i);
    }
    if (callback_after_generate)
        callback_after_generate(
            dynamic_pointer_cast<Array>(shared_from_this()));
}

CL_CLONE(Array);

bool Array::equal(shared_ptr<Hashable> o) {
    CALL("Array", "equal");
    shared_ptr<Array> other = dynamic_pointer_cast<Array>(o);
    if (!other) return false;
    if (!len->equal(other->len)) return false;
    for (int i = 0; i < elements.size(); i++) {
        shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(elements[i]);
        shared_ptr<Hashable> b = dynamic_pointer_cast<Hashable>(other->elements[i]);
        if (!a->equal(b)) return false;
    } return true;
}

uint Array::hash_code() {
    CALL("Array", "hash_code");
    uint ans = len->get();
    for (int i = 0; i < elements.size(); i++) {
        shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(elements[i]);
        ans = ans * 13 + a->hash_code();
    }
    return ans;
}

void Array::iter_reset() {
    CALL("Array", "iter_reset");
    cur_iter = 0;
}

void Array::iter_next() {
    CALL("Array", "iter_next");
    cur_iter++;
}

bool Array::iter_loop_finish() {
    CALL("Array", "iter_loop_finish");
    return cur_iter >= elements.size();
}

bool Array::iter_at_last() {
    CALL("Array", "iter_at_last");
    return cur_iter == elements.size() - 1;
}

void Array::parse(const string& spec, int n, ...) {
    CALL("Array", "parse");
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            if (!IN_RANGE(0, n, 0)) MESSAGE("Array", FUNC_ARGS_MISMATCH(n, 0, 0));
            auto ele = get<Formatable>(cur_iter);
            if (!ele) MESSAGE("Array", UNABLE("Formatable"));
            ele->out();
        } else {
            MESSAGE("Array", FUNC_NOT_FOUND(spec));
        }
        va_end(valist);
    }
}

void Array::out() {
    CALL("Array", "out");
    Formatable::parse(shared_from_this(), fmt, "Array");
}

namespace mk {
    shared_ptr<Array> array() {
        return make_shared<Array>();
    }

    shared_ptr<Array> array(int n) {
        return make_shared<Array>()->length(n);
    }
}
