#ifndef EDGE_H
#define EDGE_H

#include "Node.h"
#include "Attribute.h"
#include <vector>

class Edge : public Node {
public:
    Edge();
    Edge(const Edge& other);
    Edge* add_attribute(Attribute* attr);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void out() override;

    void __set_start_and_end(int start, int end);
private:
    int start;
    int end;
    std::vector<Attribute*> attrs;
};

#endif