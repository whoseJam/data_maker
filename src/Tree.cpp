
#include <iostream>

#include "../include/Tree.h"
#include "../include/Clone.h"
#include "../include/Define.h"
#include "../include/Random.h"
#include "../include/Format.h"
#include "../include/Destroy.h"

using namespace std;
using namespace Random;
using namespace Format;

Tree::Tree() {
    is_long_tree = false;
    is_chain = false;
    is_flower = false;
    vertex_num = UNSET;
    fmt = "UNSET";
}

Tree::Tree(const Tree& other) {
    vertex_num = other.vertex_num;
    is_long_tree = other.is_long_tree;
    is_chain = other.is_chain;
    is_flower = other.is_flower;
    for (auto attr : other.attrs) {
        attrs.push_back((Attribute*)attr->clone());
    }
    fmt = other.fmt;
}

Tree* Tree::vertex(int num) {
    vertex_num = num;
    return this;
}

Tree* Tree::chain() {
    check_flag();
    is_chain = true;
    return this;
}

Tree* Tree::flower() {
    check_flag();
    is_flower = true;
    return this;
}

Tree* Tree::long_tree() {
    check_flag();
    is_long_tree = true;
    return this;
}

Tree* Tree::add_attribute(Attribute* attr) {
    attrs.push_back(attr);
    return this;
}

Tree* Tree::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

void Tree::generate() {
    if (generated) return;
    generated = true;
    
    fa.resize(vertex_num + 5);
    attr_per_node.resize(vertex_num + 5);
    if (is_long_tree) gen_long_tree();
    else if (is_flower) gen_flower();
    else if (is_chain) gen_chain();
    else gen_random_tree();
    for (int i = 1; i <= vertex_num; i++) {
        if (fa[i] == 0) continue;
        for (auto attr : attrs) {
            Clone::get()->prepare();
            attr_per_node[i].push_back(
                (Attribute*)attr->clone()
            );
        }
        for (auto attr : attr_per_node[i])
            attr->generate();
    }
}

Node* Tree::clone() {
    if (!Clone::get()->check(this))
        Clone::get()->insert(this, new Tree(*this));
    return (Node*)Clone::get()->check(this);
}

void Tree::destroy() {
    if (destroyed) return;
    destroyed = true;

    Destroy::get()->add(this);
    for (Attribute* attr : attrs)
        attr->destroy();
    for (auto& attrs : attr_per_node) {
        for (Attribute* attr : attrs) {
            attr->destroy();
        }
    }
}

void Tree::out() {
    CHECK_STRING_UNSET(Tree, fmt);
    Format::parse(fmt, this);
}

void Tree::parse(const string& spec, ...) {
    va_list valist;
    va_start(valist, spec);
    IF_SPEC_IS_LAST(valist, spec, cur_iter, vertex_num)
    else IF_SPEC_IS_NLAST(valist, spec, cur_iter, vertex_num)
    else if (spec == "fa") {
        cout << fa[cur_iter];
    } else if (spec == "x") {
        cout << cur_iter;
    } else {
        bool specifier_matched = false;
        for (auto attr : attr_per_node[cur_iter]) {
            if (spec == attr->__get_key()) {
                attr->out();
                specifier_matched = true;
            }
        }
        if (!specifier_matched) {
            MESSAGE_NOT_FOUND_IN_FORMAT(Tree, spec);
        }
    }
    va_end(valist);
}

void Tree::parse_start() {
    cur_iter = 1;
}

void Tree::parse_next() {
    cur_iter++;
}

bool Tree::parse_finish() {
    return cur_iter > vertex_num;
}

void Tree::check_flag() {
    if (is_long_tree | 
        is_chain | 
        is_flower) {
        cout << "Tree::flag has already been set\n";
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
        pa p = RAND_AND_TAKE_OUT_FROM_VECTOR(fetch_set, pa);
        int parent = p.first;
        int depth = p.second;
        fa[i] = parent;
        fetch_set.push_back(make_pair(i, depth + 1));
        if (depth >= limit) {
            fetch_set.push_back(make_pair(i, depth + 1));
        }
    }
}
