#ifndef INTEGER_H
#define INTEGER_H

#include "Node.h"
#include "Define.h"
#include "Format.h"
#include "IntegerWrapper.h"

class IntegerPanel;

class Integer : 
    public Node,
    public Formatter {
public:
    Integer();
    Integer(const Integer& other);
    virtual ~Integer();
    Integer* lower_bound(int x);
    Integer* upper_bound(int x);
    CL_UPDATE_FUNC(Integer, lower_bound, l, UF_value);
    CL_UPDATE_FUNC(Integer, upper_bound, r, UF_value);
    Integer* format(const std::string& fmt);
    IntegerPanel* get_panel();
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
    
    friend class IntegerWrapper;
    friend class IntegerPanel;
private:
//  inner helper
    IntegerPanel* panel;

//  define stage
    IntegerWrapper* l;
    IntegerWrapper* r;

//  generate stage
    int value;

//  output stage
    int cur_iter;
};

class IntegerPanel {
public:
    IntegerPanel(Integer* parent);
    int get();
    void set(int x);
    bool equal(IntegerPanel* other);
private:
    Integer* parent;
};

#endif