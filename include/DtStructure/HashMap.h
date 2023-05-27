#ifndef HASHMAP_H
#define HASHMAP_H

#include <map>
#include <memory>

#include "Dtstructure.h"

namespace mk {

namespace Hidden {
template<typename T>
struct HashNode {
    int cnt = 0;
    T* hold;
    std::shared_ptr<HashNode> next;
};

}

template<typename T>
class HashMap {
public:
    HashMap();
    void insert(const T& obj);
    void insert(T&& obj);
    void remove(const T& obj);
    bool contain(const T& obj);
    int query_total();
    int query_different();
private:
    using MyHashNode = Hidden::HashNode<T>;
    std::map<uint, std::shared_ptr<MyHashNode>> header;
    int diff;
    int total;
};

template<typename T>
HashMap<T>::HashMap() {
    diff = 0;
    total = 0;
}

template<typename T>
void HashMap<T>::insert(const T& obj_) {
    auto obj = const_cast<T&>(obj_);
    uint code = obj.hash_code();
    if (header.find(code) == header.end() || !header[code]) {
        auto new_node = std::make_shared<MyHashNode>();
        header[code] = new_node;
        new_node->cnt = 1;
        new_node->hold = new T(obj);
        diff++; total++;
    } else {
        auto iter = header[code];
        while (true) {
            if (iter->hold->equal(obj)) {
                iter->cnt++;
                total++;
                return;
            }
            if (iter->next) iter = iter->next;
            else break;
        }
        iter->next = std::make_shared<MyHashNode>();
        iter->next->cnt = 1;
        iter->next->hold = new T(obj);
        diff++; total++;
    }
}

template<typename T>
void HashMap<T>::insert(T&& obj) {
    uint code = obj.hash_code();
    if (header.find(code) == header.end() || !header[code]) {
        auto new_node = std::make_shared<MyHashNode>();
        header[code] = new_node;
        new_node->cnt = 1;
        new_node->hold = new T(std::move(obj));
        diff++; total++;
    } else {
        auto iter = header[code];
        while (true) {
            if (iter->hold->equal(obj)) {
                iter->cnt++;
                total++;
                return;
            }
            if (iter->next) iter = iter->next;
            else break;
        }
        iter->next = std::make_shared<MyHashNode>();
        iter->next->cnt = 1;
        iter->next->hold = new T(std::move(obj));
        diff++; total++;
    }
}

template<typename T>
void HashMap<T>::remove(const T& obj_) {
    T& obj = const_cast<T&>(obj_);
    uint code = obj.hash_code();
    if (header.find(code) == header.end() || !header[code]) return;
    auto iter = header[code];
    if (iter->hold->equal(obj)) {
        iter->cnt--; total--;
        if (iter->cnt == 0) {
            header[code] = iter->next;
            diff--;
        }
        return;
    }
    iter = iter->next;
    if (!iter) return;
    auto last = header[code];
    while (true) {
        if (iter->hold->equal(obj)) {
            iter->cnt--; total--;
            if (iter->cnt == 0) {
                last->next = iter->next;
                delete iter->hold;
                diff--;
            }
            return;
        }
        iter = iter->next;
        last = last->next;
        if (!iter) break;
    }
}

template<typename T>
bool HashMap<T>::contain(const T& obj_) {
    T& obj = const_cast<T&>(obj_);
    uint code = obj.hash_code();
    if (header.find(code) == header.end() || !header[code]) return false;
    auto iter = header[code];
    while (true) {
        if (iter->hold->equal(obj))
            return true;
        iter = iter->next;
        if (!iter) break;
    }
    return false;
}

template<typename T>
int HashMap<T>::query_total() {
    return total;
}

template<typename T>
int HashMap<T>::query_different() {
    return diff;
}

}

#endif