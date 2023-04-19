#include <iostream>

#include "../include/Destroy.h"
#include "../include/Node.h"

using namespace std;

Destroy Destroy::instance;

Destroy* Destroy::get() {
    return &instance;
}

void Destroy::add(Node* item) {
    items.push_back(item);
}

void Destroy::destroy() {
    cout<<"len="<<items.size()<<"\n";
    for (Node* item : items) {
        cout<<"delete="<<item<<"\n";
        delete item;
    }
    items.clear();
}