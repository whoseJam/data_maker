
#include <iostream>

#include "../include/Tree.h"
#include "../include/Define.h"
#include "../include/Random.h"
#include "../include/Format.h"
#include "../include/Destroy.h"

using namespace std;
using namespace Random;
using namespace Format;

using TreeFunPtr = void(Tree::*)();
int Tree::robin_iter = 0;
TreeFunPtr Tree::gen_func[4] = {
    &Tree::gen_chain,
    &Tree::gen_flower,
    &Tree::gen_long_tree,
    &Tree::gen_random_tree
}; 

Tree::Tree() {
    panel = new TreePanel(this);
    tf = RANDOM_TREE;
    vertex_num = new IntegerWrapper();
    fmt = "UNSET";
    vertex_fmt = "UNSET";
    edge_fmt = "UNSET";
}

Tree::Tree(const Tree& other) {
    panel = new TreePanel(this);
    vertex_num = other.vertex_num;
    tf = other.tf;
    tmpl_edge = (Edge*)other.tmpl_edge->clone();
    tmpl_vertex = (Vertex*)other.tmpl_vertex->clone();
    fmt = other.fmt;
    vertex_fmt = other.vertex_fmt;
    edge_fmt = other.edge_fmt;
}

Tree* Tree::size(int num) {
    vertex_num->value(num);
    return this;
}

Tree* Tree::tree_form(TreeForm tf) {
    this->tf = tf;
    return this;
}

Tree* Tree::format(const string& fmt) {
    this->fmt = fmt;
    return this;
}

Tree* Tree::vertex_format(const string& fmt) {
    this->vertex_fmt = fmt;
    return this;
}

Tree* Tree::edge_format(const string& fmt) {
    this->edge_fmt = fmt;
    return this;
}

TreePanel* Tree::get_panel() {
    return panel;
}

void Tree::generate(bool re) {
    if (generated && !re) return;
    generated = true;

    CHECK_NULL(Tree, tmpl_edge);
    CHECK_NULL(Tree, tmpl_vertex);
    CL_GENERATE(vertex_num);

    for (int i = 1 ; i <= vertex_num->get(); i++) {
        vertices.push_back(
            ((Vertex*)tmpl_vertex->clone())
            ->__index(i)
        );
    }
    if (tf <= 3) (this->*gen_func[tf])();
    else if (tf == RANDOM_FORM) (this->*gen_func[rand_int(0, 3)])();
    else {
        (this->*gen_func[robin_iter++])();
        robin_iter %= 4;
    }
    CL_GENERATE_ITERABLE(vertices);
    CL_GENERATE_ITERABLE(edges);
    panel->build();
}

Node* Tree::clone() {
    if (type == STRUCTURE_NODE)
        return (Node*)new Tree(*this);
    return this;
}

void Tree::destroy() {
    if (destroyed) return;
    destroyed = true;

    delete panel;
    Destroy::get()->add(this);
    CL_DESTROY(tmpl_edge);
    CL_DESTROY(tmpl_vertex);
    CL_DESTROY_ITERABLE(edges);
    CL_DESTROY_ITERABLE(vertices);
}

void Tree::out() {
    CHECK_STRING_UNSET(Tree, fmt);
    stat = EMPTY_STATUS;
    out_seq.clear();
    Format::parse(this, fmt, "Tree", true);
    for (int type : out_seq) {
        stat = type;
        Format::parse(this, 
            stat == ITERATE_ON_EDGE ? edge_fmt : 
            stat == ITERATE_ON_VERTEX ? vertex_fmt : "UNSET",
            "Tree", false
        );
    }
}

bool Tree::equal(Node* o) {
    Tree* other = dynamic_cast<Tree*>(o);
    if (other == nullptr) return false;
    if (!vertex_num->equal(other->vertex_num)) return false;
    if (vertex_num->get() >= 10) return false;  //WARNING
    for (int i = 0; i < edges.size(); i++)
        if (!edges[i]->equal(other->edges[i])) return false;
    for (int i = 0; i < vertices.size(); i++)
        if (!vertices[i]->equal(other->vertices[i])) return false;
    return true;
}

void Tree::parse(const string& spec, int n, ...) {
    ParseStack("Tree");
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (stat == EMPTY_STATUS) {
            if (spec == SPEC_ITERATE_ON_EDGE) {
                CHECK_FUNCTION_ARGS_MATCH(Tree, spec, n, 0);
                out_seq.push_back(ITERATE_ON_EDGE);
            } else if (spec == SPEC_ITERATE_ON_VERTEX) {
                CHECK_FUNCTION_ARGS_MATCH(Tree, spec, n, 0);
                out_seq.push_back(ITERATE_ON_VERTEX);
            } else {
                MESSAGE_NOT_FOUND_IN_FORMAT(Tree, spec);
            }
        } else if (stat == ITERATE_ON_EDGE) {
            Edge* self = edges[cur_edge_iter];
            Vertex* fa = vertices[self->start];
            Vertex* son = vertices[self->end];
            if (spec == SPEC_FA) {                                                      // fa[?]
                CHECK_FUNCTION_ARGS_MATCH_IN_RANGE(Tree, spec, n, 0, 1);
                if (n == 0) cout << fa->idx;                                            // fa
                else if (n == 1) {                                                      // fa[attr]
                    char* attr_name = va_arg(valist, char*);
                    fa->parse(SPEC_SELF, 1, attr_name);                                 // -> x[attr]
                }
            } else if (spec == SPEC_SON) {                                              // son[?]
                CHECK_FUNCTION_ARGS_MATCH_IN_RANGE(Tree, spec, n, 0, 1);
                if (n == 0) cout << son->idx;                                           // son
                else if (n == 1) {                                                      // son[attr]
                    char* attr_name = va_arg(valist, char*);
                    son->parse(SPEC_SELF, 1, attr_name);                                // -> x[attr]
                }
            } else if (spec == SPEC_SELF) {                                             // x[attr]
                CHECK_FUNCTION_ARGS_MATCH(Tree, spec, n, 1);
                char* attr_name = va_arg(valist, char*);
                self->parse(SPEC_SELF, 1, attr_name);
            } else {
                MESSAGE_NOT_FOUND_IN_FORMAT(Tree, spec);
            }
        }
        if (stat == ITERATE_ON_VERTEX) {
            Vertex* self = vertices[cur_vertex_iter];
            if (spec == SPEC_SELF) {                                                    // x[?]
                CHECK_FUNCTION_ARGS_MATCH_IN_RANGE(Tree, spec, n, 0, 1);
                if (n == 0) cout << vertices[cur_vertex_iter]->idx;                     // x
                else if (n == 1) {                                                      // x[attr]
                    char* attr_name = va_arg(valist, char*);
                    self->parse(SPEC_SELF, 1, attr_name);                               // -> x[attr]
                }
            } else {
                MESSAGE_NOT_FOUND_IN_FORMAT(Tree, spec);
            }
        }
        va_end(valist);
    }
}

void Tree::parse_start() {
    cur_iter = 0;
    cur_edge_iter = 0;
    cur_vertex_iter = 0;
}

void Tree::parse_next() {
    if (stat == EMPTY_STATUS) cur_iter++;
    if (stat == ITERATE_ON_EDGE) cur_edge_iter++;
    if (stat == ITERATE_ON_VERTEX) cur_vertex_iter++;
}

bool Tree::parse_finish() {
    if (stat == EMPTY_STATUS) return cur_iter > 0;
    if (stat == ITERATE_ON_EDGE) return cur_edge_iter >= edges.size();
    if (stat == ITERATE_ON_VERTEX) return cur_vertex_iter >= vertices.size();
    return true;
}

bool Tree::is_last() {
    if (stat == ITERATE_ON_EDGE) return cur_edge_iter == edges.size() - 1;
    if (stat == ITERATE_ON_VERTEX) return cur_vertex_iter == vertices.size() - 1;
    return true;
}

void Tree::add_edge(int f, int s) {
    Edge* edge = (Edge*)tmpl_edge->clone();
    edge->__set_start_and_end(f, s);
    edges.push_back(edge);
}

void Tree::gen_random_tree() {
    for (int i = 1; i < vertex_num->get(); i++) {
        int parent = rand_int(0, i - 1);
        add_edge(parent, i);
    }
}

void Tree::gen_flower() {
    for (int i = 1; i < vertex_num->get(); i++) {
        add_edge(0, i);
    }
}

void Tree::gen_chain() {
    for (int i = 1; i < vertex_num->get(); i++) {
        add_edge(i - 1, i);
    }
}

void Tree::gen_long_tree() {
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

TreePanel::TreePanel(Tree* parent) {
    this->parent = parent;
}

void TreePanel::root(int idx) {
    if (cur_root != idx) {
        cur_root = idx;
        dfs(cur_root, 0);
    }
}

Attribute* TreePanel::node_attribute(int idx, const std::string& name) {
    return parent->vertices[idx]->get_panel()->get(name);
}

Attribute* TreePanel::edge_attribute(int x, int y, const std::string& name) {
    if (x > y) swap(x, y);
    pair<int, int> pa = make_pair(x, y);
    if (edges.find(pa) == edges.end()) {
        return nullptr;
    }
    return edges[pa]->get_panel()->get(name);
}

vector<Attribute*> TreePanel::subtree_attribute(int idx, const string& name) {
    vector<Attribute*> ans;
    dfs_get_attrs(idx, name, ans);
    return ans;
}

void TreePanel::build() {
    int n = parent->vertex_num->get();
    tr.resize(n); fa.resize(n);
    for (int i = 0; i < n; i++) fa[i] = -1;
    for (auto edge : parent->edges) {
        auto panel = edge->get_panel();
        tr[panel->get_start()].push_back(panel->get_end());
        tr[panel->get_end()].push_back(panel->get_start());
    }
    dfs(cur_root=0, -1);
}


void TreePanel::dfs(int x, int f) {
    fa[x] = f;
    for (int to : tr[x]) {
        if (fa[to] == -1 && to != cur_root) {
            dfs(to, x);
        }
    }
}

void TreePanel::dfs_get_attrs(int x, const string& name, vector<Attribute*>& ans) {
    ans.push_back(node_attribute(x, name));
    for (int to : tr[x]) {
        if (fa[to] == x) {
            dfs_get_attrs(to, name, ans);
        }
    }
}