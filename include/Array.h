#ifndef ARRAY_H
#define ARRAY_H
#include <vector>
#include <string>

#include "Node.h"
#include "Format.h"
#include "Integer.h"
#include "Character.h"
#include "IntegerWrapper.h"

class Array : 
    public Node, 
    public Formatter {
public:
    Array();
    Array(const Array& other);
    virtual ~Array();
    Array* length(int len);
    CL_UPDATE_FUNC(Array, length, len, UF_value);
    CL_UPDATE_FUNC(Array, fill, tmpl_ele, UF_assign);
    Array* format(const std::string& fmt);
    Array* unique();
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
    IntegerWrapper* len;
    Node* tmpl_ele;
    bool unique_flag;

//  generate stage
    std::vector<Node*> elements;

//  output stage
    int cur_iter;
};

#endif