#ifndef CLONE_H
#define CLONE_H

#include <map>
#include <list>
#include <memory>

#include "Node.h"

class Clone {
public:
    static Clone* get();
    void enter(std::shared_ptr<Node> ptr);
    void exit();
    bool check_stay_with(void* ptr);
    std::shared_ptr<Node> check(void* ptr);
    void insert(void*, std::shared_ptr<Node> ptr);
private:
    Clone();
    
    static Clone instance;
    std::map<void*, std::shared_ptr<Node>> helper;
    std::map<void*, int> cnt;
    std::list<void*> stack;
};

#endif