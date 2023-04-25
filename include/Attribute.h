#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <iostream>

#include "Node.h"
#include "Define.h"
#include "Integer.h"
#include "Character.h"

class AttributePanel;

class Attribute : public Node {
public:
    Attribute();
    Attribute(const Attribute& other);
    virtual ~Attribute();
    Attribute* name(const std::string& name);
    CL_UPDATE_FUNC(Attribute, value, val, UF_assign);
    AttributePanel* get_panel();
    virtual void generate(bool re) override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
    virtual bool equal(Node* other) override;

    friend class AttributePanel;
    friend class AttributeGroup;
    friend class AttributeGroupPanel;
private:
    AttributePanel* panel;

    Node* val;
    std::string key;
};

class AttributePanel {
public:
    AttributePanel(Attribute* parent);
    Node* get();
private:
    Attribute* parent;
};

#endif