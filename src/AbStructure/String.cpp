
#include <iostream>

#include "Clone.h"
#include "Debug.h"
#include "String.h"
#include "Logger.h"

using namespace std;

String::String() {
    CALL(FUNCTION);
    fmt = "$x";
}

String::String(const String& other) :
    Node(other), 
    Formatable(other) {
    CALL(FUNCTION);
    callback_when_generating_per_element = other.callback_when_generating_per_element;
    callback_after_generate = other.callback_after_generate;
    if (!other.len) MESSAGE("String", NEED("length"));
    if (!other.template_char) MESSAGE("String", NEED("fill"));
    len = dynamic_pointer_cast<Integer>(other.len->clone());
    template_char = dynamic_pointer_cast<Character>(other.template_char->clone());
    for (auto ele : other.elements)
        elements.push_back(dynamic_pointer_cast<Character>(ele->clone()));
}

String::~String() {
#ifdef OUTPUT_DELETER
    cout << "delete string\n";
#endif
}

shared_ptr<String> String::length(int len) {
    CALL(FUNCTION);
    if (!this->len) this->len = make_shared<Integer>();
    this->len->set(len);
    return dynamic_pointer_cast<String>(shared_from_this());
}

shared_ptr<String> String::format(const string& fmt) {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<String>(shared_from_this());
}

shared_ptr<String> String::when_generating_per_element(
    function<void(shared_ptr<String>, int)> callback) {
    CALL(FUNCTION);
    callback_when_generating_per_element = callback;
    return dynamic_pointer_cast<String>(shared_from_this());
}

shared_ptr<String> String::after_generate(
    function<void(shared_ptr<String>)> callback) {
    CALL(FUNCTION);
    callback_after_generate = callback;
    return dynamic_pointer_cast<String>(shared_from_this());
}

int String::get_length() {
    CALL(FUNCTION);
    if (!len) MESSAGE("String", NEED("length"));
    return len->get();
}

shared_ptr<Character> String::get(int idx) {
    CALL(FUNCTION);
    if (idx < 0 || idx >= len->get()) MESSAGE("String", INDEX_OUT_OF_BOUNDARY);
    return elements[idx];
}

void String::generate(bool re, shared_ptr<Node> from) {
    CALL(FUNCTION);
    from_node = from;
    if (generated && !re) return;
    generated = true;
    
    if (!len) MESSAGE("String", NEED("length"));
    if (!template_char) MESSAGE("String", NEED("fill"));
    len->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
    for (int i = 1; i <= len->get(); i++)
        elements.push_back(
            dynamic_pointer_cast<Character>(template_char->clone()));
    for (int i = 0; i < elements.size(); i++) {
        elements[i]->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
        if (callback_when_generating_per_element)
            callback_when_generating_per_element(
                dynamic_pointer_cast<String>(shared_from_this()), i);
    }
    if (callback_after_generate)
        callback_after_generate(
            dynamic_pointer_cast<String>(shared_from_this()));
}

CL_CLONE(String);

void String::out() {
    CALL(FUNCTION);
    Formatable::parse(
        enable_shared_from_this<Formatable>::shared_from_this(), fmt, "String");
}

bool String::equal(shared_ptr<Hashable> o) {
    CALL(FUNCTION);
    shared_ptr<String> other = dynamic_pointer_cast<String>(o);
    if (!other) return false;
    if (!len->equal(other->len)) return false;
    for (int i = 0; i < elements.size(); i++) {
        shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(elements[i]);
        shared_ptr<Hashable> b = dynamic_pointer_cast<Hashable>(other->elements[i]);
        if (!a->equal(b)) return false;
    } return true;
}

uint String::hash_code() {
    CALL(FUNCTION);
    uint ans = len->get();
    for (int i = 0; i < elements.size(); i++) {
        shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(elements[i]);
        ans = ans * 13 + a->hash_code();
    }
    return ans;
}

void String::iter_reset() {
    CALL(FUNCTION);
    cur_iter = 0;
}

void String::iter_next() {
    CALL(FUNCTION);
    cur_iter++;
}

bool String::iter_loop_finish() {
    CALL(FUNCTION);
    return cur_iter >= elements.size();
}

bool String::iter_at_last() {
    CALL(FUNCTION);
    return cur_iter == elements.size() - 1;
}

void String::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            if (!IN_RANGE(0, n, 0)) MESSAGE("String", FUNC_ARGS_MISMATCH(n, 0, 0));
            elements[cur_iter]->out();
        } else {
            MESSAGE("String", FUNC_NOT_FOUND(spec));
        }
        va_end(valist);
    }
}

namespace mk {
    shared_ptr<String> string() {
        return make_shared<String>();
    }
}