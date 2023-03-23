#ifndef NODE_H
#define NODE_H

#include "IExportable.h"

class Node : public IExportable {
public:
    virtual void generate() = 0;
    virtual Node* clone() = 0;
};

#endif