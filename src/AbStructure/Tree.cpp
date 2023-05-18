
#include <iostream>

#include "Tree.h"
#include "Clone.h"
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
    CALL(FUNCTION);
    tf = RANDOM_TREE;
}

Tree::Tree(const Tree& other) :
    Node(other), 
    Formatable(other) {
    CALL(FUNCTION);
    if (!other.vertex_num) MESSAGE("Tree", NEED("size"));
    if (!other.template_edge) MESSAGE("Tree", NEED("edge"));
    if (!other.template_vertex) MESSAGE("Tree", NEED("vertex"));

    vertex_num = dynamic_pointer_cast<Integer>(other.vertex_num->clone());
    tf = other.tf;
    template_edge = dynamic_pointer_cast<Edge>(other.template_edge->clone());
    template_vertex = dynamic_pointer_cast<Vertex>(other.template_vertex->clone());
    for (auto edge : other.edges)
        edges.push_back(dynamic_pointer_cast<Edge>(edge->clone()));
    for (auto vertex : other.vertices)
        vertices.push_back(dynamic_pointer_cast<Vertex>(vertex->clone()));
    fmt = other.fmt;
}

shared_ptr<Tree> Tree::size(int num) {
    CALL(FUNCTION);
    if (!vertex_num) vertex_num = make_shared<Integer>();
    vertex_num->set(num);
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::tree_form(TreeForm tf) -> shared_ptr<Tree> {
    CALL(FUNCTION);
    this->tf = tf;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::format(const string& fmt) -> shared_ptr<Tree> {
    CALL(FUNCTION);
    this->fmt = fmt;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::vertex(int u) -> std::shared_ptr<Vertex> {
    if (u < 0 || u > vertices.size()) MESSAGE("Tree", INDEX_OUT_OF_BOUNDARY);
    return vertices[u];
}

auto Tree::vertex_set() -> vector<shared_ptr<Vertex>>& {
    return vertices;
}

auto Tree::edge_set() -> vector<shared_ptr<Edge>>& {
    return edges;
}

auto Tree::size() -> int {
    return vertex_num->get();
}

void Tree::generate(bool re, shared_ptr<Node> from) {
    CALL(FUNCTION);
    from_node = from;
    if (generated && !re) return;
    generated = true;

    if (!template_vertex) MESSAGE("Tree", NEED("vertex"));
    if (!template_edge) MESSAGE("Tree", NEED("edge"));
    if (!vertex_num) MESSAGE("Tree", NEED("size"));
    
    vertex_num->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
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
        vertices[i]->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
    for (int i = 0; i < edges.size(); i++)
        edges[i]->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
}

CL_CLONE(Tree);

void Tree::out() {
    CALL(FUNCTION);
    stat = EMPTY_STATUS;
    empty_stat_called = false;
    Formatable::parse(
        enable_shared_from_this<Formatable>::shared_from_this(), fmt, "Tree");
}

bool Tree::equal(shared_ptr<Hashable> o) {
    CALL(FUNCTION);
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
    CALL(FUNCTION);
    uint ans = 0;
    for (int i = 0; i < vertices.size(); i++)
        ans = ans * 23 + vertices[i]->hash_code();
    for (int i = 0; i < edges.size(); i++)
        ans = ans * 29 + edges[i]->hash_code();
    return ans;
}

void Tree::iter_reset() {
    CALL(FUNCTION);
    cur_iter = 0;
}

void Tree::iter_next() {
    CALL(FUNCTION);
    cur_iter++;
}

bool Tree::iter_loop_finish() {
    CALL(FUNCTION);
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
    CALL(FUNCTION);
    if (stat == ITERATE_ON_VERTEX) return cur_iter == vertices.size() - 1;
    if (stat == ITERATE_ON_EDGE) return cur_iter == edges.size() - 1;
    return false;
}

void Tree::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
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
    CALL(FUNCTION);
    for (int i = 1; i < vertex_num->get(); i++) {
        int parent = rand_int(0, i - 1);
        add_edge(parent, i);
    }
}

void Tree::gen_flower() {
    CALL(FUNCTION);
    for (int i = 1; i < vertex_num->get(); i++) {
        add_edge(0, i);
    }
}

auto Tree::gen_chain() -> void {
    CALL(FUNCTION);
    for (int i = 1; i < vertex_num->get(); i++) {
        add_edge(i - 1, i);
    }
}

auto Tree::gen_long_tree() -> void {
    CALL(FUNCTION);
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

auto Tree::add_edge(int f, int s) -> void {
    CALL(FUNCTION);
    shared_ptr<Edge> e = dynamic_pointer_cast<Edge>(template_edge->clone());
    e->set(f, s);
    edges.push_back(e);
}

namespace mk {
    shared_ptr<Tree> tree() {
        return make_shared<Tree>();
    }
}
