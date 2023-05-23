
#include <iostream>
#include <memory>

#include "Integer.h"
#include "Node.h"
#include "Tree.h"
#include "Debug.h"
#include "Logger.h"
#include "Random.h"
#include "Vertex.h"

using namespace std;
using namespace Random;

namespace mk {

int COUNT_TREE = 0;

using TreeFunPtr = void(Tree::*)();
int Tree::robin_iter = 0;
TreeFunPtr Tree::gen_func[4] = {
    &Tree::gen_chain,
    &Tree::gen_flower,
    &Tree::gen_long_tree,
    &Tree::gen_random_tree
}; 

Tree::Tree() {
    COUNT_TREE++;
    tf = RANDOM_TREE;
    template_edge = make_shared<Edge>();
    template_vertex = make_shared<Vertex>();
}

Tree::Tree(const Tree& other) :
    Node(other), 
    Formatable(other) {
    COUNT_TREE++;
    if (!other.vertex_num) MESSAGE("Tree", NEED("size"));
    if (!other.template_edge) MESSAGE("Tree", NEED("edge"));
    if (!other.template_vertex) MESSAGE("Tree", NEED("vertex"));

    vertex_num = dynamic_pointer_cast<Integer>(other.vertex_num->clone(0));
    tf = other.tf;
    callback_before_generate = other.callback_before_generate;
    callback_after_generate = other.callback_after_generate;
    template_edge = dynamic_pointer_cast<Edge>(other.template_edge->clone(0));
    template_vertex = dynamic_pointer_cast<Vertex>(other.template_vertex->clone(0));
    for (auto edge : other.edges)
        edges.push_back(dynamic_pointer_cast<Edge>(edge->clone(0)));
    for (auto vertex : other.vertices)
        vertices.push_back(dynamic_pointer_cast<Vertex>(vertex->clone(0)));
    fmt = other.fmt;
}

Tree::~Tree() {
    COUNT_TREE--;
}

auto Tree::size(int num) -> shared_ptr<Tree> {
    CALL(FUNCTION);
    if (!vertex_num) vertex_num = make_shared<Integer>();
    vertex_num->value(num);
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::size(shared_ptr<Integer> num) -> shared_ptr<Tree> {
    CALL(FUNCTION);
    vertex_num = num;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::tree_form(TreeForm tf) -> shared_ptr<Tree> {
    CALL(FUNCTION);
    this->tf = tf;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::edge(shared_ptr<Edge> e) -> shared_ptr<Tree> {
    CALL(FUNCTION);
    template_edge = e;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::vertex(shared_ptr<Vertex> v) -> shared_ptr<Tree> {
    CALL(FUNCTION);
    template_vertex = v;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::before_generate(function<void(shared_ptr<Tree>)> callback) -> shared_ptr<Tree> {
    CALL(FUNCTION);
    callback_before_generate = callback;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::after_generate(function<void(shared_ptr<Tree>)> callback) -> shared_ptr<Tree> {
    CALL(FUNCTION);
    callback_after_generate = callback;
    return dynamic_pointer_cast<Tree>(shared_from_this());
}

auto Tree::format(const char* fmt) -> shared_ptr<Tree> {
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
    return vertex_num->value();
}

void Tree::generate(bool re) {
    CALL(FUNCTION);
    GENERATE;
    if (generated && !re) return;
    generated = true;

    if (!template_vertex) MESSAGE("Tree", NEED("vertex"));
    if (!template_edge) MESSAGE("Tree", NEED("edge"));
    if (!vertex_num) MESSAGE("Tree", NEED("size"));
    
    if (callback_before_generate)
        callback_before_generate(dynamic_pointer_cast<Tree>(shared_from_this()));

    vertex_num->generate(re);
    for (int i = 1; i <= vertex_num->value(); i++) {
        shared_ptr<Vertex> ver = dynamic_pointer_cast<Vertex>(template_vertex->clone(1));
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

    if (callback_after_generate)
        callback_after_generate(dynamic_pointer_cast<Tree>(shared_from_this()));
}

CL_CLONE(Tree);

void Tree::out() {
    CALL(FUNCTION);
    stat = EMPTY_STATUS;
    empty_stat_called = false;
    Formatable::parse(
        enable_shared_from_this<Formatable>::shared_from_this(), fmt, "Tree");
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
            shared_ptr<Vertex> fa = vertices[self->start_];
            shared_ptr<Vertex> son = vertices[self->end_];
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
    for (int i = 1; i < vertex_num->value(); i++) {
        int parent = rand_int(0, i - 1);
        add_edge(parent, i);
    }
}

void Tree::gen_flower() {
    CALL(FUNCTION);
    for (int i = 1; i < vertex_num->value(); i++) {
        add_edge(0, i);
    }
}

auto Tree::gen_chain() -> void {
    CALL(FUNCTION);
    for (int i = 1; i < vertex_num->value(); i++) {
        add_edge(i - 1, i);
    }
}

auto Tree::gen_long_tree() -> void {
    CALL(FUNCTION);
    typedef pair<int,int> pa;
    vector<pa> fetch_set;
    fetch_set.push_back(make_pair(0, 1));
    int limit = vertex_num->value() / 4;
    for (int i = 1; i < vertex_num->value(); i++) {
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
    shared_ptr<Edge> e = dynamic_pointer_cast<Edge>(template_edge->clone(1));
    e->start(f);
    e->end(s);
    edges.push_back(e);
}

shared_ptr<Tree> tree() {
    return make_shared<Tree>();
}

}
