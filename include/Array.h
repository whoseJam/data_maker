#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <string>
#include <functional>

#include "Node.h"
#include "Format.h"
#include "Integer.h"
#include "Character.h"
#include "IntegerWrapper.h"

class ArrayPanel;

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
    Array* satisfy(std::function<bool(ArrayPanel*, int idx)>);
    ArrayPanel* get_panel();
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

    friend class ArrayPanel;
private:
//  inner helper
    ArrayPanel* panel;

//  define stage
    IntegerWrapper* len;
    Node* tmpl_ele;
    bool unique_flag;
    std::function<bool(ArrayPanel*, int)> checker;

//  generate stage
    std::vector<Node*> elements;

//  output stage
    int cur_iter;
};

class ArrayPanel {
public:
    ArrayPanel(Array* parent);
    template<typename T, typename P>
    P* element(int idx) {
        return dynamic_cast<T*>(parent->elements[idx])->get_panel();
    }
private:
    Array* parent;
};

#endif