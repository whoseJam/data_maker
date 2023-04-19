#ifndef NODE_H
#define NODE_H

class Node {
public:
    Node();
    virtual ~Node();
    virtual void generate() = 0;
    virtual Node* clone() = 0;
    virtual void destroy() = 0;
    virtual void out() = 0;
protected:
    bool generated;
    bool destroyed;
};

#endif