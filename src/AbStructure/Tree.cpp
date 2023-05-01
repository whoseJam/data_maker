
#include <iostream>

#include "Tree.h"
#include "Debug.h"
#include "Logger.h"
#include "Random.h"

using namespace std;
using namespace Random;

using TreeFunPtr = void(Tree::*)();
int Tree::robin_iter = 0;
TreeFunPtr Tree::gen_func[4] = {
    &Tree::gen_chain,
    &Tree::gen_flower,
    &Tree::gen_long_tree,
    &Tree::gen_random_tree
}; 

Tree::Tree() {
    CALL("Tree", "Tree");
    tf = RANDOM_TREE;
}

Tree::Tree(const Tree& other) :
    Node(other), 
    Formatable(other) {
    CALL("Tree", "Tree");
    vertex_num = other.vertex_num;
    tf = other.tf;
    template_edge = dynamic_pointer_cast<Edge>(other.template_edge->clone());
    template_vertex = dynamic_pointer_cast<Vertex>(other.template_vertex->clone());
    fmt = other.fmt;
}

shared_ptr<Tree> Tree::size(int num) {
    CALL("Tree", "size");
    if (!vertex_num) vertex_num = make_shared<Integer>();
    vertex_num->set(num);
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

shared_ptr<Tree> Tree::tree_form(TreeForm tf) {
    CALL("Tree", "tree_form");
    this->tf = tf;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

shared_ptr<Tree> Tree::format(const string& fmt) {
    CALL("Tree", "format");
    this->fmt = fmt;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

shared_ptr<Vertex> Tree::get(int u) {
    if (u < 0 || u > vertices.size()) MESSAGE("Tree", INDEX_OUT_OF_BOUNDARY);
    return vertices[u];
}

const vector<int> Tree::get_adjoint(int u) {
    return links[u];
}

int Tree::get_size() {
    return vertex_num->get();
}

void Tree::generate(bool re) {
    CALL("Tree", "generate");
    if (generated && !re) return;
    generated = true;

    if (!template_vertex) MESSAGE("Tree", NEED("vertex"));
    if (!template_edge)   MESSAGE("Tree", NEED("edge"));
    if (!vertex_num)      MESSAGE("Tree", NEED("size"));
    
    vertex_num->generate(re);
    links.resize(vertex_num->get());
    for (int i = 1; i <= vertex_num->get(); i++) {
        shared_ptr<Vertex> ver = dynamic_pointer_cast<Vertex>(template_vertex->clone());
        ver->set(i);
        vertices.push_back(ver);
    }
    if (tf <= 3) (this->*gen_func[tf])();
    else if (tf == RANDOM_FORM) (this->*gen_func[rand_int(0, 3)])();
    else {
        (this->*gen_func[robin_iter++])();
        robin_iter %= 4;
    }
    for (int i = 0; i < vertices.size(); i++)
        vertices[i]->generate(re);
    for (int i = 0; i < edges.size(); i++)
        edges[i]->generate(re);
}

shared_ptr<Node> Tree::clone() {
    CALL("Tree", "clone");
    if (type == STRUCTURE_NODE)
        return make_shared<Tree>(*this);
    return dynamic_pointer_cast<Node>(shared_from_this());
}

void Tree::out() {
    CALL("Tree", "out");
    stat = EMPTY_STATUS;
    empty_stat_called = false;
    Formatable::parse(
        enable_shared_from_this<Formatable>::shared_from_this(), fmt, "Tree");
}

bool Tree::equal(shared_ptr<Hashable> o) {
    CALL("Tree", "equal");
    shared_ptr<Tree> other = dynamic_pointer_cast<Tree>(o);
    if (other == nullptr) return false;
    if (!vertex_num->equal(other->vertex_num)) return false;
    if (vertex_num->get() >= 10) return false;  //WARNING
    for (int i = 0; i < edges.size(); i++)
        if (!edges[i]->equal(other->edges[i])) return false;
    for (int i = 0; i < vertices.size(); i++)
        if (!vertices[i]->equal(other->vertices[i])) return false;
    return true;
}

uint Tree::hash_code() {
    CALL("Tree", "hash_code");
    uint ans = 0;
    for (int i = 0; i < vertices.size(); i++)
        ans = ans * 23 + vertices[i]->hash_code();
    for (int i = 0; i < edges.size(); i++)
        ans = ans * 29 + edges[i]->hash_code();
    return ans;
}

void Tree::iter_reset() {
    CALL("Tree", "iter_reset");
    cur_iter = 0;
}

void Tree::iter_next() {
    CALL("Tree", "iter_next");
    cur_iter++;
}

bool Tree::iter_loop_finish() {
    CALL("Tree", "iter_loop_finish");
    if (stat == ITERATE_ON_VERTEX) {
        if (cur_iter >= vertices.size()) stat = EMPTY_STATUS;
        return cur_iter >= vertices.size();
    }
    if (stat == ITERATE_ON_EDGE) {
        if (cur_iter >= edges.size()) stat = EMPTY_STATUS;
        return cur_iter >= edges.size();
    }
    if (empty_stat_called) return true;
    empty_stat_called = true;
    return false;
}

bool Tree::iter_at_last() {
    CALL("Tree", "iter_at_last");
    if (stat == ITERATE_ON_VERTEX) return cur_iter == vertices.size() - 1;
    if (stat == ITERATE_ON_EDGE) return cur_iter == edges.size() - 1;
    return false;
}

void Tree::parse(const string& spec, int n, ...) {
    CALL("Tree", "parse");
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (stat == EMPTY_STATUS) {
            if (spec == SPEC_ITERATE_ON_EDGE) {
                if (!IN_RANGE(1, n, 1)) MESSAGE("Tree", FUNC_ARGS_MISMATCH(n, 1, 1));
                stat = ITERATE_ON_EDGE;
                string fmt = va_arg(valist, char*);
                Formatable::parse(
                    enable_shared_from_this<Formatable>::shared_from_this(), fmt, "Tree");
            } else if (spec == SPEC_ITERATE_ON_VERTEX) {
                if (!IN_RANGE(1, n, 1)) MESSAGE("Tree", FUNC_ARGS_MISMATCH(n, 1, 1));
                stat = ITERATE_ON_VERTEX;
                string fmt = va_arg(valist, char*);
                Formatable::parse(
                    enable_shared_from_this<Formatable>::shared_from_this(), fmt, "Tree");
            } else {
                MESSAGE("Tree", FUNC_NOT_FOUND(spec));
            }
        } else if (stat == ITERATE_ON_EDGE) {
            shared_ptr<Edge> self = edges[cur_iter];
            shared_ptr<Vertex> fa = vertices[self->start];
            shared_ptr<Vertex> son = vertices[self->end];
            if (spec == SPEC_FA) {                                                      // fa[?]
                if (!IN_RANGE(0, n, 1)) MESSAGE("Tree", FUNC_ARGS_MISMATCH(n, 0, 1));
                if (n == 0) cout << fa->get();                                          // fa
                else if (n == 1) {                                                      // fa[attr]
                    char* attr_name = va_arg(valist, char*);
                    fa->parse(SPEC_SELF, 1, attr_name);                                 // -> x[attr]
                }
            } else if (spec == SPEC_SON) {                                              // son[?]
                if (!IN_RANGE(0, n, 1)) MESSAGE("Tree", FUNC_ARGS_MISMATCH(n, 0, 1));
                if (n == 0) cout << son->get();                                         // son
                else if (n == 1) {                                                      // son[attr]
                    char* attr_name = va_arg(valist, char*);
                    son->parse(SPEC_SELF, 1, attr_name);                                // -> x[attr]
                }
            } else if (spec == SPEC_SELF) {                                             // x[attr]
                if (!IN_RANGE(0, n, 1)) MESSAGE("Tree", FUNC_ARGS_MISMATCH(n, 0, 1));
                char* attr_name = va_arg(valist, char*);
                self->parse(SPEC_SELF, 1, attr_name);
            } else {
                MESSAGE("Tree", FUNC_NOT_FOUND(spec));
            }
        }
        if (stat == ITERATE_ON_VERTEX) {
            shared_ptr<Vertex> self = vertices[cur_iter];
            if (spec == SPEC_SELF) {                                                    // x[?]
                if (!IN_RANGE(0, n, 1)) MESSAGE("Tree", FUNC_ARGS_MISMATCH(n, 0, 1));
                if (n == 0) cout << self->get();                                        // x
                else if (n == 1) {                                                      // x[attr]
                    char* attr_name = va_arg(valist, char*);
                    self->parse(SPEC_SELF, 1, attr_name);                               // -> x[attr]
                }
            } else {
                MESSAGE("Tree", FUNC_NOT_FOUND(spec));
            }
        }
        va_end(valist);
    }
}

void Tree::gen_random_tree() {
    CALL("Tree", "gen_random_tree");
    for (int i = 1; i < vertex_num->get(); i++) {
        int parent = rand_int(0, i - 1);
        add_edge(parent, i);
    }
}

void Tree::gen_flower() {
    CALL("Tree", "gen_flower");
    for (int i = 1; i < vertex_num->get(); i++) {
        add_edge(0, i);
    }
}

void Tree::gen_chain() {
    CALL("Tree", "gen_chain");
    for (int i = 1; i < vertex_num->get(); i++) {
        add_edge(i - 1, i);
    }
}

void Tree::gen_long_tree() {
    CALL("Tree", "gen_long_tree");
    typedef pair<int,int> pa;
    vector<pa> fetch_set;
    fetch_set.push_back(make_pair(0, 1));
    int limit = vertex_num->get() / 4;
    for (int i = 1; i < vertex_num->get(); i++) {
        pa p = RAND_AND_TAKE_OUT_FROM_VECTOR(fetch_set, pa);
        int parent = p.first;
        int depth = p.second;
        add_edge(parent, i);
        fetch_set.push_back(make_pair(i, depth + 1));
        if (depth >= limit) {
            fetch_set.push_back(make_pair(i, depth + 1));
        }
    }
}

void Tree::add_edge(int f, int s) {
    CALL("Tree", "add_edge");
    links[f].push_back(s);
    links[s].push_back(f);
    shared_ptr<Edge> e = dynamic_pointer_cast<Edge>(template_edge->clone());
    e->set(f, s);
    edges.push_back(e);
}

namespace mk {
    shared_ptr<Tree> tree() {
        return make_shared<Tree>();
    }
}
