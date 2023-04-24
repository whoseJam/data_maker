#ifndef DESTROY_H
#define DESTROY_H

#include <vector>

class Node;

class Destroy {
public:
    static Destroy* get();
    void add(Node* item);
    void start_service();
    void stop_service();
private:
    static Destroy instance;
    std::vector<Node*> items;
};

#endif