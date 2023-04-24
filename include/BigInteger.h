#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include "Node.h"

class BigInteger : public Node {
public:
    BigInteger();
    BigInteger(const BigInteger& other);
    BigInteger* lower_bound(long long x);
    BigInteger* upper_bound(long long x);
    virtual void generate(bool re) override;
    virtual Node* clone() override;
    virtual void out() override;
private:
    long long l;
    long long r;
    int gflag;
    long long value;
};

#endif