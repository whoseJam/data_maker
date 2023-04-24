#ifndef PRE_H
#define PRE_H

#include "IO.h"
#include "Tuple.h"
#include "Integer.h"
#include "Attribute.h"
#include "Character.h"

template<typename T>
T* value_node(T* o) {
    return o;
}

template<typename T>
T& structure_node(T&& o) {
    return o;
}

template<typename T>
Attribute* attr(const std::string& key, T&& val) {
    return (new Attribute())->name(key)->value(std::forward<T>(val));
}

template<typename T>
Tuple* tuple(T&& arg) {
    return (new Tuple())->append(std::forward<T>(arg));
}
template<typename T, typename ...Args>
Tuple* tuple(T&& arg, Args&&... args) {
    return tuple(std::forward<Args>(args)...)->unshift(std::forward<T>(arg));
}

Character* lower_character();
Character* upper_character();
Integer* integer(int l, int r);
IO* standard_io();

/*
Tuple* make_tuple(Node* a);
Tuple* make_tuple(Node* a, Node* b);
Tuple* make_tuple(Node* a, Node* b, Node* c);
Tuple* make_tuple(Node* a, Node* b, Node* c, Node* d);
Tuple* make_tuple(Node* a, Node* b, Node* c, Node* d, Node* e);
*/
#endif