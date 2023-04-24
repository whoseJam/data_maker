#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>
#include <string>

#include "Node.h"
#include "Format.h"
#include "Integer.h"
#include "Character.h"
#include "IntegerWrapper.h"

class Permutation : 
    public Node, 
    public Formatter {
public:
    Permutation();
    Permutation(const Permutation& other);
    Permutation* length(int len);
    CL_UPDATE_FUNC(Permutation, length, len, UF_value);
    Permutation* format(const std::string& fmt);
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

//  generate stage
    std::vector<int> elements;

//  output stage
    int cur_iter;
};

#endif