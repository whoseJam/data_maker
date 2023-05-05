#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <memory>
#include <iostream>

#include "Node.h"
#include "Format.h"
#include "HashMap.h"
#include "Attribute.h"

class Vertex : 
    public Node, 
    public Hashable,
    public Formatable {
public:
    Vertex();
    Vertex(const Vertex& other);
    virtual ~Vertex();
    CL_UPDATE_FUNC(Vertex, add_attribute, attrs, UF_append_vector, CK_equal_to(Attribute), );
    std::shared_ptr<Vertex> format(const std::string& fmt);
    
    int get();
    std::shared_ptr<Attribute> get(const std::string& name);
    void set(int idx);

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone() override;
    virtual void out() override;
    
    virtual bool equal(std::shared_ptr<Hashable> other) override;
    virtual uint hash_code() override;

    virtual void parse(const std::string& spec, int n, ...) override;
private:
//  define stage
    std::vector<std::shared_ptr<Attribute>> attrs;
    int idx;
};

namespace mk {
    std::shared_ptr<Vertex> vertex();
}

#endif