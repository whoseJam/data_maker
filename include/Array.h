#ifndef ARRAY_H
#define ARRAY_H

#include "Node.h"
#include "Format.h"
#include "Integer.h"
#include "Character.h"
#include <vector>
#include <string>

class Array : public Node {
public:
    Array();
    Array(const Array& other);
    Array* length(int len);
    Array* fill(Integer* ele);
    Array* fill(Character* ele);
    Array* format(const std::string& fmt);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void out() override;

    friend class Format::FormatPackage;
    int __cur_iter;
private:
    int len;
    bool is_permutation;
    Node* element;
    std::string fmt;
    std::vector<Node*> elements;
};

#endif