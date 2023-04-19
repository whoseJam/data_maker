#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <vector>

#include "Node.h"
#include "Format.h"

class Tuple : public Node, public Format::Formatter {
public:
    Tuple();
    Tuple(const Tuple& other);
    virtual ~Tuple();
    Tuple* append(Node* ele);
    Tuple* unshift(Node* ele);
    Tuple* format(const std::string& fmt);
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
    int len;
    std::vector<Node*> elements;

//  output stage
    int cur_iter;
};

#endif