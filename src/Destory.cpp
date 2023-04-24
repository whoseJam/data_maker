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

void Destroy::start_service() {
    for (Node* item : items) {
        delete item;
    }
    items.clear();
}

void Destroy::stop_service() {
    for (Node* item : items) {
        delete item;
    }
    items.clear();
}