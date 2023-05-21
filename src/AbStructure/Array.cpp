#include <functional>
#include <memory>
#include <utility>
#include <iostream>

#include "Debug.h"
#include "Array.h"
#include "Logger.h"
#include "Node.h"

using namespace std;

namespace mk {

Array::Array() {
    CALL(FUNCTION);
    fmt = "$x ";
}

Array::Array(const Array& other) : 
    Node(other), 
    Formatable(other) {
    CALL(FUNCTION);
    callback_before_generate = other.callback_before_generate;
    callback_when_generating = other.callback_when_generating;
    callback_after_generate = other.callback_after_generate;
    if (!other.len) MESSAGE("Array", NEED("length"));
    if (!other.template_ele) MESSAGE("Array", NEED("fill"));
    len = dynamic_pointer_cast<Integer>(other.len->clone(0));
    template_ele = other.template_ele->clone(0);
    for (auto ele : other.elements) 
        elements.push_back(ele->clone(0));
}

static int array_cnt = 0;
Array::~Array() {
#ifdef DELETE_CHECK
    cout << "delete array " << ++array_cnt << "\n";
#endif
}

auto Array::length(int len) -> shared_ptr<Array> {
    CALL(FUNCTION);
    if (!this->len) this->len = make_shared<Integer>();
    this->len->value(len);
    return dynamic_pointer_cast<Array>(shared_from_this());
}

auto Array::length(shared_ptr<Integer> len) -> shared_ptr<Array> {
    CALL(FUNCTION);
    this->len = len;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

auto Array::fill(shared_ptr<Node> ele) -> shared_ptr<Array> {
    CALL(FUNCTION);
    this->template_ele = ele;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

auto Array::format(const string& fmt) -> shared_ptr<Array> {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

auto Array::before_generate(
    function<void(shared_ptr<Array>)> callback) -> shared_ptr<Array> {
    CALL(FUNCTION);
    callback_before_generate = callback;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

auto Array::when_generating(
    function<void(shared_ptr<Array>, int)> callback) -> shared_ptr<Array> {
    CALL(FUNCTION);
    callback_when_generating = callback;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

auto Array::after_generate(
    function<void(shared_ptr<Array>)> callback) -> shared_ptr<Array> {
    CALL(FUNCTION);
    callback_after_generate = callback;
    return dynamic_pointer_cast<Array>(shared_from_this());
}

auto Array::length() -> int {
    CALL(FUNCTION);
    if (!len) MESSAGE("Array", NEED("length"));
    return len->value();
}

auto Array::generate(bool re) -> void {
    CALL(FUNCTION);
    GENERATE;
    if (generated && !re) return;
    generated = true;
    
    if (!len) MESSAGE("Array", NEED("length"));
    if (!template_ele) MESSAGE("Array", NEED("fill"));

    if (callback_before_generate)
        callback_before_generate(
            dynamic_pointer_cast<Array>(shared_from_this()));
    len->generate(re);
    for (int i = 0; i < len->value(); i++) {
        NodeMap::clear(NodeMap::CLONE_STAGE);
        elements.push_back(template_ele->clone(1));
        elements[i]->generate(re);
        if (callback_when_generating)
            callback_when_generating(
                dynamic_pointer_cast<Array>(shared_from_this()), i);
    }
    if (callback_after_generate)
        callback_after_generate(
            dynamic_pointer_cast<Array>(shared_from_this()));
}

CL_CLONE(Array);

void Array::iter_reset() {
    CALL(FUNCTION);
    cur_iter = 0;
}

void Array::iter_next() {
    CALL(FUNCTION);
    cur_iter++;
}

bool Array::iter_loop_finish() {
    CALL(FUNCTION);
    return cur_iter >= elements.size();
}

bool Array::iter_at_last() {
    CALL(FUNCTION);
    return cur_iter == elements.size() - 1;
}

void Array::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
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
    CALL(FUNCTION);
    Formatable::parse(shared_from_this(), fmt, "Array");
}

auto array() -> shared_ptr<Array> { return make_shared<Array>(); }
}
