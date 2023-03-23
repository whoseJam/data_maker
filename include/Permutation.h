#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>
#include "Node.h"
#include "Integer.h"
#include "Character.h"

class Permutation : public Node {
public:
    Permutation();
    Permutation(const Permutation& other);
    Permutation* length(int len);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void out() override;
private:
    int len;
    std::vector<int> elements;
};

#endif