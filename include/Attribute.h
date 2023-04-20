#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>

#include "Node.h"
#include "Integer.h"
#include "Character.h"

class Attribute : public Node {
public:
    Attribute();
    Attribute(const Attribute& other);
    virtual ~Attribute();
    Attribute* name(const std::string& name);
    Attribute* value(Integer* integer);
    Attribute* value(Character* character);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override; 

    std::string __get_key();
private:
    Node* val;
    std::string key;
};

#endif