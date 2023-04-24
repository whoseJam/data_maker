#ifndef EDGE_H
#define EDGE_H

#include <vector>

#include "Node.h"
#include "Format.h"
#include "Attribute.h"
#include "AttributeGroup.h"

class Edge : 
    public Node, 
    public Formatter {
public:
    Edge();
    Edge(const Edge& other);
    CL_UPDATE_FUNC(Edge, add_attribute, attrs, UF_add_attribute);
    Edge* format(const std::string& fmt);
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
    
    Edge* __set_start_and_end(int start, int end);

    friend class Tree;
    friend class Graph;
private:
//  define stage;
    AttributeGroup* attrs;

//  generate stage
    int start;
    int end;

//  output stage
    int cur_iter;
};

#endif