#ifndef INTEGER_H
#define INTEGER_H

#include "Node.h"

class Integer : public Node {
public:
    Integer();
    Integer(const Integer& other);
    Integer* lower_bound(int x);
    Integer* upper_bound(int x);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void out() override;
private:
    int l;
    int r;
    int gflag;
    int value;
};

#endif