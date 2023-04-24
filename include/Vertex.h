#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <iostream>

#include "Node.h"
#include "Format.h"
#include "Attribute.h"
#include "AttributeGroup.h"

class Tree;

class Vertex : 
    public Node, 
    public Formatter {
public:
    Vertex();
    Vertex(const Vertex& other);
    virtual ~Vertex();
    CL_UPDATE_FUNC(Vertex, add_attribute, attrs, UF_add_attribute);
    Vertex* format(const std::string& fmt);
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

    Vertex* __index(int idx);
    friend class Tree;
private:
//  define stage
    int idx;
    AttributeGroup* attrs;

//  output stage
    int cur_iter;
};

#endif