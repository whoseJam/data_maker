#ifndef NODE_H
#define NODE_H

#include <memory>

class Node {
public:
    Node();
    Node(const Node& other);
    virtual ~Node();
    virtual void generate(bool re, std::shared_ptr<Node> from) = 0;
    virtual std::shared_ptr<Node> clone() = 0;
    virtual void out() = 0;

    template<typename T>
    std::shared_ptr<T> from() {
        return std::dynamic_pointer_cast<T>(from_node);
    }

    void live_with(std::shared_ptr<Node> prt);
public:
    std::shared_ptr<Node> from_node;
    bool generated;
    void* parent;
};

#endif