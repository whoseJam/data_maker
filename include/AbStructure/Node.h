#ifndef NODE_H
#define NODE_H

#include <memory>

class Node {
public:
    enum {
        VALUE_NODE,
        STRUCTURE_NODE
    };
public:
    Node();
    Node(const Node& other);
    virtual ~Node();
    virtual void generate(bool re) = 0;
    virtual std::shared_ptr<Node> clone() = 0;
    virtual void out() = 0;

    friend void make_structure(std::shared_ptr<Node>);
    friend void make_value(std::shared_ptr<Node>);
    int type;
protected:
    bool generated;
};

void make_structure(std::shared_ptr<Node>);
void make_value(std::shared_ptr<Node>);

#endif