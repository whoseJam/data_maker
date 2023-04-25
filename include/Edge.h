#ifndef EDGE_H
#define EDGE_H

#include <vector>

#include "Node.h"
#include "Format.h"
#include "Attribute.h"
#include "AttributeGroup.h"

class EdgePanel;

class Edge : 
    public Node, 
    public Formatter {
public:
    Edge();
    Edge(const Edge& other);
    CL_UPDATE_FUNC(Edge, add_attribute, attrs, UF_add_attribute);
    Edge* format(const std::string& fmt);
    EdgePanel* get_panel();
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
    friend class EdgePanel;
private:
//  inner helper
    EdgePanel* panel;

//  define stage;
    AttributeGroup* attrs;

//  generate stage
    int start;
    int end;

//  output stage
    int cur_iter;
};

class EdgePanel {
public:
    EdgePanel(Edge* parent);
    Attribute* get(const std::string& name);
    int get_start();
    int get_end();
private:
    Edge* parent;
};

#endif