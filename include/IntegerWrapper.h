
#ifndef INTEGERWRAPPER_H
#define INTEGERWRAPPER_H

#include <string>

#include "Node.h"
#include "Define.h"

class Integer;

class IntegerWrapper : public Node {
public:
    IntegerWrapper();
    IntegerWrapper(const IntegerWrapper& other);
    int get();
    IntegerWrapper* value(int val);
    CL_UPDATE_FUNC(IntegerWrapper, value, integer_value, UF_assign);
    virtual void generate(bool re) override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
    virtual bool equal(Node* other) override;
private:
    Integer* integer_value;
    int int_value;
};

#endif