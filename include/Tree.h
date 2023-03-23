#ifndef TREE_H
#define TREE_H

#include "Attribute.h"
#include "Format.h"
#include <string>
#include <vector>

class Tree : public Node {
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
    virtual void out() override;

    friend class Format::FormatPackage;
    int __cur_iter;
private:
    void check_flag();
    void check_format_is_called();
    void gen_long_tree();
    void gen_random_tree();
    void gen_flower();
    void gen_chain();

    int vertex_num;
    bool is_long_tree;
    bool is_chain;
    bool is_flower;
    bool format_is_called;
    std::string fmt;
    std::vector<int> fa;
    std::vector<
        std::vector<Attribute*>
    > attr_per_node;
    std::vector<Attribute*> attrs;
};

#endif