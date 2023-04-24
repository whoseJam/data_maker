#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <iostream>

#include "Node.h"
#include "Define.h"
#include "Integer.h"
#include "Character.h"

class Attribute : public Node {
public:
    Attribute();
    Attribute(const Attribute& other);
    virtual ~Attribute();
    Attribute* name(const std::string& name);
    CL_UPDATE_FUNC(Attribute, value, val, UF_assign);
    virtual void generate(bool re) override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
    virtual bool equal(Node* other) override;

    std::string __get_key();
private:
    Node* val;
    std::string key;
};

#endif