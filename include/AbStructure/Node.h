#ifndef NODE_H
#define NODE_H

#include <cstddef>
#include <map>
#include <list>
#include <memory>

namespace mk {

#define GENERATE \
    NodeMap::insert(NodeMap::GENERATE_STAGE, this->origin, std::dynamic_pointer_cast<Node>(this->shared_from_this())); \
    defer(NodeMap::remove(NodeMap::GENERATE_STAGE, this->origin));
#define CLONE_FIRST 

#define BUILD(ele) \
    ele->generate(0);

#define BUILD_AND_OUT(ele) \
    ele->generate(0); ele->out();

class Node;

namespace NodeMap {
enum {
    GENERATE_STAGE,     // 存储开始generate的node的origin->value的映射关系
    CLONE_FIRST_STAGE,  // 存储clone(first = true)的那个元素的origin->value的映射关系
    CLONE_STAGE,        // 存储clone过程中，每个node的origin->value的映射关系（用于实现clone-on-first）
    TRACK_STAGE,        // 存储整个过程每一个node的origin->value的映射关系
};
auto insert(int type, void* ptr, std::shared_ptr<Node>) -> void;
auto remove(int type, void* ptr) -> void;
auto clear(int type) -> void;
auto get(int type, void* ptr) -> std::shared_ptr<Node>; 
}

class Node {
public:
    Node();
    Node(const Node& other);
    virtual ~Node();
    virtual void generate(bool re) = 0;
    virtual std::shared_ptr<Node> clone(bool first) = 0;
    
    void live_with(std::shared_ptr<Node> prt);
    static std::shared_ptr<Node> track(std::shared_ptr<Node> origin);
public:
    bool debug;
    bool generated;
    void* parent;
    void* origin;
};

template<typename T>
auto track(std::shared_ptr<Node> node) -> std::shared_ptr<T> {
    return std::dynamic_pointer_cast<T>(NodeMap::get(NodeMap::TRACK_STAGE, node->origin));
}
/*
this: parent在generate中出现.
第一次clone，之后this: parent指向clone(first=true).
clone: 上述情况都不满足.
*/


#define CL_CLONE(class) \
    auto class::clone(bool first) -> std::shared_ptr<Node> { \
        CALL(FUNCTION); \
        NodeMap::insert(NodeMap::CLONE_FIRST_STAGE, (first ? this->origin : nullptr), std::dynamic_pointer_cast<Node>(this->shared_from_this())); \
        auto parent = NodeMap::get(NodeMap::GENERATE_STAGE, this->parent); \
        if (parent) return std::dynamic_pointer_cast<Node>(this->shared_from_this()); \
        parent = NodeMap::get(NodeMap::CLONE_FIRST_STAGE, this->parent); \
        if (parent) { \
            auto first_clone = NodeMap::get(NodeMap::CLONE_STAGE, this->origin); \
            if (first_clone) return first_clone; \
            first_clone = std::make_shared<class>(*this); \
            NodeMap::insert(NodeMap::TRACK_STAGE, this->origin, first_clone); \
            NodeMap::insert(NodeMap::CLONE_STAGE, this->origin, first_clone); \
            return first_clone; \
        } \
        auto ans = std::make_shared<class>(*this); \
        NodeMap::insert(NodeMap::TRACK_STAGE, this->origin, ans); \
        return ans; \
    }

#define CL_CLONE_IN_CLASS(class) \
    virtual auto clone(bool first) -> std::shared_ptr<Node> { \
        CALL(FUNCTION); \
        NodeMap::insert(NodeMap::CLONE_FIRST_STAGE, (first ? this->origin : nullptr), std::dynamic_pointer_cast<Node>(this->shared_from_this())); \
        auto parent = NodeMap::get(NodeMap::GENERATE_STAGE, this->parent); \
        if (parent) return std::dynamic_pointer_cast<Node>(this->shared_from_this()); \
        parent = NodeMap::get(NodeMap::CLONE_FIRST_STAGE, this->parent); \
        if (parent) { \
            auto first_clone = NodeMap::get(NodeMap::CLONE_STAGE, this->origin); \
            if (first_clone) return first_clone; \
            first_clone = std::make_shared<class>(*this); \
            NodeMap::insert(NodeMap::TRACK_STAGE, this->origin, first_clone); \
            NodeMap::insert(NodeMap::CLONE_STAGE, this->origin, first_clone); \
            return first_clone; \
        } \
        auto ans = std::make_shared<class>(*this); \
        NodeMap::insert(NodeMap::TRACK_STAGE, this->origin, ans); \
        return ans; \
    }
}

#endif