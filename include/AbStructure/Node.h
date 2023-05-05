#ifndef NODE_H
#define NODE_H

#include <memory>

class Node {
public:
    enum {
        UN_NODE,
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
    int type;
public:
    void implicit_type(int type);
    void explicit_type(int type);
    bool explicit_flag;
public:
    bool generated;
};

namespace mk {
    template<typename T>
    T structure(T&& ele) {
        ele->explicit_type(Node::STRUCTURE_NODE);
        return ele;
    }

    template<typename T>
    T value(T&& ele) {
        ele->explicit_type(Node::VALUE_NODE);
        return ele;
    }

    template<typename T>
    T un(T&& ele) {
        ele->explicit_type(Node::UN_NODE);
        return ele;
    }
}

#endif