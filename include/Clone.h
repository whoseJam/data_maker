#ifndef CLONE_H
#define CLONE_H

#include <map>

class Clone {
public:
    static Clone* get();
    void prepare();
    void insert(void* from, void* to);
    void* check(void* from);
private:
    std::map<void*, void*> rel;
    static Clone instance;
};

#endif