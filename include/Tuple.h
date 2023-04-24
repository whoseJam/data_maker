#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <vector>
#include <iostream>

#include "Node.h"
#include "Define.h"
#include "Format.h"

class Tuple : 
    public Node, 
    public Formatter {
public:
    Tuple();
    Tuple(const Tuple& other);
    virtual ~Tuple();
    CL_UPDATE_FUNC(Tuple, append, elements, UF_append_vector);
    CL_UPDATE_FUNC(Tuple, unshift, elements, UF_unshift_vector);
    Tuple* format(const std::string& fmt);
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
    int len;
    std::vector<Node*> elements;

//  output stage
    int cur_iter;
};

#endif