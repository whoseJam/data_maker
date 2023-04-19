
#include "../include/Clone.h"

Clone Clone::instance;

Clone* Clone::get() {
    return &instance;
}

void Clone::prepare() {
    rel.clear();
}

void Clone::insert(void* from, void* to) {
    rel[from] = to;
}

void* Clone::check(void* from) {
    if (rel.find(from) == rel.end())
        return nullptr;
    return rel[from];
}
