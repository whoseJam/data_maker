#ifndef EDGE_H
#define EDGE_H

#include <memory>
#include <vector>

#include "Format.h"
#include "HashMap.h"
#include "Attribute.h"

class EdgePanel;

class Edge : 
    public Node, 
    public Hashable,
    public Formatable {
public:
    Edge();
    Edge(const Edge& other);
    CL_UPDATE_FUNC(Edge, add_attribute, attrs, UF_append_vector, CK_equal_to(Attribute), );
    std::shared_ptr<Edge> format(const std::string& fmt);
    
    int get_start();
    int get_end();
    std::shared_ptr<Attribute> get(const std::string& name);
    void set(int s, int e);

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone() override;
    virtual void out() override;
    
    virtual bool equal(std::shared_ptr<Hashable> other) override;
    virtual uint hash_code() override;

    virtual void parse(const std::string& spec, int n, ...) override;
    
    friend class Tree;
    friend class Graph;
private:
//  define stage;
    std::vector<std::shared_ptr<Attribute>> attrs;

//  generate stage
    int start;
    int end;
};

namespace mk {
    std::shared_ptr<Edge> edge();
}

#endif