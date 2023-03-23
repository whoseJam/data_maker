
#include "../include/Tree.h"
#include "../include/Define.h"
#include "../include/Random.h"
#include "../include/Format.h"
#include <iostream>
using namespace std;
using namespace Random;
using namespace Format;

Tree::Tree() {
    is_long_tree = false;
    is_chain = false;
    is_flower = false;
    format_is_called = false;
    vertex_num = UNSET;
}

Tree::Tree(const Tree& other) {
    is_long_tree = other.is_long_tree;
    is_chain = other.is_chain;
    is_flower = other.is_flower;
    for (auto attr : other.attrs) {
        attrs.push_back((Attribute*)attr->clone());
    }
}

Tree* Tree::vertex(int num) {
    check_format_is_called();
    vertex_num = num;
    return this;
}

Tree* Tree::chain() {
    check_flag();
    check_format_is_called();
    is_chain = true;
    return this;
}

Tree* Tree::flower() {
    check_flag();
    check_format_is_called();
    is_flower = true;
    return this;
}

Tree* Tree::long_tree() {
    check_flag();
    check_format_is_called();
    is_long_tree = true;
    return this;
}

Tree* Tree::add_attribute(Attribute* attr) {
    check_format_is_called();
    attrs.push_back(attr);
    return this;
}

Tree* Tree::format(const string& fmt) {
    format_is_called = true;
    for (int i = 0; i < fmt.length(); i++) {
        if (fmt[i] == '%') {
            string tmp = get_specifier(fmt, i);
            bool flag = false;
            if (tmp == "fa" || tmp == "x")
                flag = true;
            for (auto attr : attrs) {
                if (tmp == attr->__get_key()) { 
                    flag = true;
                }
            }
            if (is_general_specifier(tmp)) flag = true;
            if (!flag) {
                MESSAGE_NOT_FOUND_IN_FORMAT(Tree, tmp);
                exit(-1);
            }
        }
    }
    this->fmt = fmt;
    return this;
}

void Tree::generate() {
    fa.resize(vertex_num + 5);
    attr_per_node.resize(vertex_num + 5);
    if (is_long_tree) gen_long_tree();
    else if (is_flower) gen_flower();
    else if (is_chain) gen_chain();
    else gen_random_tree();
    for (int i = 1; i <= vertex_num; i++) {
        if (fa[i] == 0) continue;
        for (auto attr : attrs)
            attr_per_node[i].push_back(
                (Attribute*)attr->clone()
            );
        for (auto attr : attr_per_node[i])
            attr->generate();
    }
}

Node* Tree::clone() {
    return new Tree(*this);
}

void Tree::out() {
    for (int i = 1; i <= vertex_num; i++) {
        if (!fa[i]) continue;
        __cur_iter = i;
        for (int j = 0; j < fmt.length(); j++) {
            if (fmt[j] == '%') {
                string tmp = get_specifier(fmt, j);
                if (tmp == "fa") cout << fa[i];
                else if (tmp == "x") cout << i;
                else {
                    bool specifier_matched = false;
                    for (auto attr : attr_per_node[i]) {
                        if (tmp == attr->__get_key()) {
                            attr->out();
                            specifier_matched = true;
                        }
                    }
                    if (!specifier_matched) {
                        out_general_specifier(fmt, j, FormatPackage(this));
                    }
                }
                j += tmp.length();
            } else {
                cout << fmt[j];
            }
        }
    }
}

void Tree::check_flag() {
    if (is_long_tree | 
        is_chain | 
        is_flower) {
        cout << "Tree::flag has already been set\n";
        exit(-1);
    }
}

void Tree::check_format_is_called() {
    if (format_is_called) {
        cout << "Tree::format() should be called after all attributes have been set\n";
        exit(-1);
    }
}

void Tree::gen_random_tree() {
    fa[1] = 0;
    for (int i = 2; i <= vertex_num; i++) {
        int parent = rand_int(1, i - 1);
        fa[i] = parent;
    }
}

void Tree::gen_flower() {
    fa[1] = 0;
    for (int i = 2; i <= vertex_num; i++) {
        fa[i] = 1;
    }
}

void Tree::gen_chain() {
    fa[1] = 0;
    for (int i = 2; i <= vertex_num; i++) {
        fa[i] = i - 1;
    }
}

void Tree::gen_long_tree() {
    fa[1] = 0;
    typedef pair<int,int> pa;
    vector<pa> fetch_set;
    fetch_set.push_back(make_pair(1, 1));
    int limit = vertex_num / 4;
    for (int i = 2; i <= vertex_num; i++) {
        pa p = rand_and_take_out_from_vector<pa>(fetch_set);
        int parent = p.first;
        int depth = p.second;
        fa[i] = parent;
        fetch_set.push_back(make_pair(i, depth + 1));
        if (depth >= limit) {
            fetch_set.push_back(make_pair(i, depth + 1));
        }
    }
}
