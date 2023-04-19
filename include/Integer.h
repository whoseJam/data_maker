#ifndef INTEGER_H
#define INTEGER_H

#include "Node.h"

class Integer : public Node {
public:
    Integer();
    Integer(const Integer& other);
    virtual ~Integer();
    Integer* lower_bound(int x);
    Integer* upper_bound(int x);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
    
    friend class IntegerWrapper;
private:
    int l;
    int r;
    int gflag;
    int value;
};

#endif