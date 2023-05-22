#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <memory>
#include <iostream>

#include "Node.h"
#include "Format.h"
#include "HashMap.h"
#include "Attribute.h"

namespace mk {

class Vertex : 
    public Node, 
    public Formatable {
public:
    Vertex();
    Vertex(const Vertex& other);
    virtual ~Vertex();
    auto add_attribute(std::shared_ptr<Attribute> attr) -> std::shared_ptr<Vertex>;
    std::shared_ptr<Vertex> format(const std::string& fmt);
    
    int get();
    std::shared_ptr<Attribute> get(const std::string& name);
    void set(int idx);

    virtual void generate(bool re) override;
    virtual auto clone(bool first) -> std::shared_ptr<Node> override;

    virtual void parse(const std::string& spec, int n, ...) override;
    virtual void out() override;
private:
//  define stage
    std::vector<std::shared_ptr<Attribute>> attrs;
    int idx;
};

std::shared_ptr<Vertex> vertex();

}

#endif