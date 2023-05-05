#ifndef CLONE_H
#define CLONE_H

#include <map>
#include <list>
#include <memory>

#include "Node.h"

class Clone {
public:
    static Clone* get();
    void enter(bool flag);
    void exit();
    bool structure();
    void insert(void* ptr, std::shared_ptr<Node> val);
    std::shared_ptr<Node> check(void* ptr);
private:
    Clone();
    
    static Clone instance;
    std::list<bool> has_structure;
    std::map<void*, std::shared_ptr<Node>> helper;
    int cnt;
};

#endif