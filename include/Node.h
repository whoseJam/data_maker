#ifndef NODE_H
#define NODE_H

class Node {
public:
    enum {
        VALUE_NODE,
        STRUCTURE_NODE
    };
    Node();
    virtual ~Node();
    virtual void generate(bool re) = 0;
    virtual Node* clone() = 0;
    virtual void destroy() = 0;
    virtual void out() = 0;
    virtual bool equal(Node* other) = 0;
    int type;
protected:
    bool generated;
    bool destroyed;
};

#endif