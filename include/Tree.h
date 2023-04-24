#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <functional>

#include "Edge.h"
#include "Vertex.h"
#include "Format.h"
#include "Attribute.h"

class Tree : 
    public Node, 
    public Formatter {
public:
    enum TreeForm { CHAIN = 0, FLOWER = 1, LONG_TREE = 2, RANDOM_TREE = 3, RANDOM_FORM, ROBIN_RANDOM_FORM };

    Tree();
    Tree(const Tree& other);
    Tree* size(int num);
    CL_UPDATE_FUNC(Tree, size, vertex_num, UF_value);
    Tree* tree_form(TreeForm tf);
    CL_UPDATE_FUNC(Tree, edge, tmpl_edge, UF_assign);
    CL_UPDATE_FUNC(Tree, vertex, tmpl_vertex, UF_assign);
    Tree* format(const std::string& fmt);
    Tree* vertex_format(const std::string& fmt);
    Tree* edge_format(const std::string& fmt);
    virtual void generate(bool re) override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
    virtual bool equal(Node* other) override;
    virtual void parse(const std::string& specifier, int n, ...) override;
    virtual void parse_start() override;
    virtual void parse_next() override;
    virtual bool parse_finish() override;
    virtual bool is_last() override;
private:
    void gen_long_tree();
    void gen_random_tree();
    void gen_flower();
    void gen_chain();
    void add_edge(int x, int y);

//  static helper
    using TreeFunPtr = void(Tree::*)();
    static int robin_iter;
    static TreeFunPtr gen_func[4];

//  define stage
    IntegerWrapper* vertex_num;
    TreeForm tf;
    Edge* tmpl_edge;
    Vertex* tmpl_vertex;
    std::string vertex_fmt;
    std::string edge_fmt;

//  generate stage
    std::vector<Edge*> edges;
    std::vector<Vertex*> vertices;

//  output stage
    std::vector<int> out_seq;
    int stat;
    int cur_iter;
    int cur_vertex_iter;
    int cur_edge_iter;
};

#endif