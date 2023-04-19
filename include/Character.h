#ifndef CHARACTER_H
#define CHARACTER_H

#include "Node.h"

class Character : public Node {
public:
    Character();
    Character(const Character& other);
    virtual ~Character();
    Character* lower_bound(char x);
    Character* upper_bound(char x);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
private:
    char l;
    char r;
    int gflag;
    char value;
};

#endif