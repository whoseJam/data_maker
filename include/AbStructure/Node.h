#ifndef NODE_H
#define NODE_H

#include <map>
#include <list>
#include <memory>

namespace mk {

#define BUILD(ele) \
    ele->generate(0);

#define BUILD_AND_OUT(ele) \
    ele->generate(0); ele->out();

class Node;

class Tracker {
public:
    static Tracker* get();
    void track_on(void* origin, std::shared_ptr<Node> cur);
    std::shared_ptr<Node> find(void* origin);
private:
    Tracker() = default;
    static Tracker instance;
    std::map<void*, std::shared_ptr<Node>> memory;
};

class Clone {
public:
    static Clone* get();
    void enter(std::shared_ptr<Node> ptr);
    void exit();
    bool check_stay_with(void* ptr);
    std::shared_ptr<Node> check(void* ptr);
    void insert(void*, std::shared_ptr<Node> ptr);
private:
    Clone() = default;
    
    static Clone instance;
    std::map<void*, std::shared_ptr<Node>> helper;
    std::map<void*, int> cnt;
    std::list<void*> stack;
};

class Node {
public:
    Node();
    Node(const Node& other);
    virtual ~Node();
    virtual void generate(bool re) = 0;
    virtual std::shared_ptr<Node> clone() = 0;
    
    void keep_track();
    void live_with(std::shared_ptr<Node> prt);
    static std::shared_ptr<Node> track(std::shared_ptr<Node> origin);
public:
    bool generated;
    void* parent;
    void* origin;
};

#define CL_CLONE(class) \
    auto class::clone() -> std::shared_ptr<Node> { \
        CALL(FUNCTION); \
        Clone::get()->enter(std::dynamic_pointer_cast<Node>(this->shared_from_this())); \
        struct CloneGuard { ~CloneGuard() {Clone::get()->exit();}} cg; \
        auto only_clone_once = Clone::get()->check_stay_with(this->parent); \
        if (only_clone_once && Clone::get()->check(this)) return Clone::get()->check(this); \
        auto ans = std::make_shared<class>(*this); \
        if (only_clone_once) Clone::get()->insert(this, ans); \
        if (this->origin) Tracker::get()->track_on(this->origin, ans); \
        return ans; \
    }

#define CL_CLONE_IN_CLASS(class) \
    virtual auto clone() -> std::shared_ptr<Node> { \
        CALL(FUNCTION); \
        Clone::get()->enter(std::dynamic_pointer_cast<Node>(this->shared_from_this())); \
        struct CloneGuard { ~CloneGuard() {Clone::get()->exit();}} cg; \
        auto only_clone_once = Clone::get()->check_stay_with(this->parent); \
        if (only_clone_once && Clone::get()->check(this)) return Clone::get()->check(this); \
        auto ans = std::make_shared<class>(*this); \
        if (only_clone_once) Clone::get()->insert(this, ans); \
        if (this->origin) Tracker::get()->track_on(this->origin, ans); \
        return ans; \
    }
}

#endif