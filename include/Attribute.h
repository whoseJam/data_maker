#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "Node.h"
#include "Integer.h"
#include "Character.h"
#include <string>

class Attribute : public Node {
public:
    Attribute();
    Attribute(const Attribute& other);
    Attribute* name(const std::string& name);
    Attribute* value(Integer* integer);
    Attribute* value(Character* character);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void out() override; 

    std::string __get_key();
private:
    Node* val;
    std::string key;
};

#endif