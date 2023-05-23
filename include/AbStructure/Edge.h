#ifndef EDGE_H
#define EDGE_H

#include <memory>
#include <vector>

#include "Format.h"
#include "HashMap.h"
#include "Attribute.h"

namespace mk {

extern int COUNT_EDGE;

class Edge : 
    public Node, 
    public Formatable {
public:
    Edge();
    Edge(const Edge& other);
    ~Edge();
    auto add_attribute(std::shared_ptr<Attribute> attr) -> std::shared_ptr<Edge>;
    auto format(const char* fmt) -> std::shared_ptr<Edge>;
    
    auto start() -> int;
    auto end() -> int;
    auto attr(const std::string& name) -> std::shared_ptr<Attribute>;
    auto start(int s) -> void;
    auto end(int e) -> void;

    virtual auto generate(bool re) -> void override;
    virtual auto clone(bool first) -> std::shared_ptr<Node> override;
    
    virtual void parse(const std::string& spec, int n, ...) override;
    virtual void out() override;
    
    friend class Tree;
    friend class Graph;
private:
    std::vector<std::shared_ptr<Attribute>> attrs;
    int start_;
    int end_;
};

std::shared_ptr<Edge> edge();
}
#endif