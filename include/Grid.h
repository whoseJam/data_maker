#ifndef GRID_H
#define GRID_H

#include "Node.h"
#include "Format.h"
#include "IntegerWrapper.h"

class Grid :
    public Node,
    public Formatter {
public:
    Grid();
    Grid(const Grid& other);

private:
    IntegerWrapper* size_n;
    IntegerWrapper* size_m;
}

#endif