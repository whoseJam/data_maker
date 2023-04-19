#ifndef ARRAY_H
#define ARRAY_H
#include <vector>
#include <string>

#include "Node.h"
#include "Format.h"
#include "Integer.h"
#include "Character.h"
#include "IntegerWrapper.h"

class Array : public Node, public Format::Formatter {
public:
    Array();
    Array(const Array& other);
    virtual ~Array();
    Array* length(int len);
    Array* length(Integer* len);
    Array* fill(Node* ele);
    Array* format(const std::string& fmt);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
    virtual void parse(const std::string& specifier, ...) override;
    virtual void parse_start() override;
    virtual void parse_next() override;
    virtual bool parse_finish() override;
private:
//  define stage
    IntegerWrapper len;
    Node* element;

//  generate stage
    std::vector<Node*> elements;

//  output stage
    int cur_iter;
};

#endif