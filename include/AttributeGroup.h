#ifndef ATTRIBUTEGROUP_H
#define ATTRIBUTEGROUP_H

#include <vector>
#include <iostream>

#include "Node.h"
#include "Format.h"
#include "Attribute.h"

class AttributeGroup :
    public Node, 
    public Formatter {
public:
    AttributeGroup();
    AttributeGroup(const AttributeGroup& other);
    CL_UPDATE_FUNC(AttributeGroup, add_attribute, attrs, UF_append_vector);
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
//  define stage
    std::vector<Attribute*> attrs;
};

#endif