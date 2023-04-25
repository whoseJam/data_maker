#ifndef ATTRIBUTEGROUP_H
#define ATTRIBUTEGROUP_H

#include <vector>
#include <iostream>

#include "Node.h"
#include "Format.h"
#include "Attribute.h"

class AttributeGroupPanel;

class AttributeGroup :
    public Node, 
    public Formatter {
public:
    AttributeGroup();
    AttributeGroup(const AttributeGroup& other);
    CL_UPDATE_FUNC(AttributeGroup, add_attribute, attrs, UF_append_vector);
    AttributeGroupPanel* get_panel();
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

    friend class AttributeGroupPanel;
private:
//  inner helper
    AttributeGroupPanel* panel;

//  define stage
    std::vector<Attribute*> attrs;
};

class AttributeGroupPanel {
public:
    AttributeGroupPanel(AttributeGroup* parent);
    Attribute* get(const std::string& name);
private:
    AttributeGroup* parent;
};

#endif