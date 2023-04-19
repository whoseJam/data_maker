#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

#include "Format.h"
#include "Attribute.h"

class Tree : public Node, public Format::Formatter {
public:
    Tree();
    Tree(const Tree& other);
    Tree* vertex(int num);
    Tree* chain();
    Tree* flower();
    Tree* long_tree();
    Tree* add_attribute(Attribute* attr);
    Tree* format(const std::string& fmt);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
    virtual void parse(const std::string& specifier, ...) override;
    virtual void parse_start() override;
    virtual void parse_next() override;
    virtual bool parse_finish() override;
private:
    void check_flag();
    void gen_long_tree();
    void gen_random_tree();
    void gen_flower();
    void gen_chain();

//  define stage
    int vertex_num;
    bool is_long_tree;
    bool is_chain;
    bool is_flower;
    std::vector<Attribute*> attrs;

//  generate stage
    std::vector<int> fa;
    std::vector<
        std::vector<Attribute*>
    > attr_per_node;

//  output stage
    int cur_iter;
};

#endif