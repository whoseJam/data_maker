#include "../include/Format.h"
#include "../include/Define.h"
#include "../include/Array.h"
#include <iostream>
using namespace std;
using namespace Format;

Array::Array() {
    len = UNSET;
    element = nullptr;
}

Array::Array(const Array& other) {
    len = other.len;
    element = other.element;
}

Array* Array::length(int len) {
    this->len = len;
    return this;
}

Array* Array::fill(Integer* ele) {
    element = ele;
    return this;
}

Array* Array::fill(Character* ele) {
    element = ele;
    return this;
}

Array* Array::format(const string& fmt) {
    for (int i = 0; i < fmt.length(); i++) {
        if (fmt[i] == '%') {
            string tmp = get_specifier(fmt, i);
            bool flag = false;
            if (tmp == "x" || is_general_specifier(tmp))
                flag = true;
            if (!flag) {
                MESSAGE_NOT_FOUND_IN_FORMAT(Array, tmp);
                exit(-1);
            }
        }
    }
    this->fmt = fmt;
    return this;
}

void Array::generate() {
    CHECK_NULL(Array, element);
    CHECK_UNSET(Array, len);
    for (int i = 1; i <= len; i++) 
        elements.push_back(element->clone());
    for (auto ele : elements) 
        ele->generate();
}

Node* Array::clone() {
    return new Array(*this);
}

void Array::out() {
    CHECK_STRING_UNSET(Array, fmt);
    for (int i = 0; i < elements.size(); i++) {
        __cur_iter = i;
        Node* ele = elements[i];
        for (int j = 0; j < fmt.length(); j++) {
            if (fmt[j] == '%') {
                string tmp = get_specifier(fmt, j);
                if (tmp == "x") ele->out();
                else out_general_specifier(fmt, j, FormatPackage(this));
                j += tmp.length();
            } else {
                cout << fmt[j];
            }
        }
    }
}