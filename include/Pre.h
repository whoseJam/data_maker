#ifndef PRE_H
#define PRE_H

#include "IO.h"
#include "Tuple.h"
#include "Integer.h"
#include "Character.h"

Character* lower_character();
Character* upper_character();
Integer* integer(int l, int r);
IO* standard_io();

Tuple* make_tuple(Node* a);
Tuple* make_tuple(Node* a, Node* b);
Tuple* make_tuple(Node* a, Node* b, Node* c);
Tuple* make_tuple(Node* a, Node* b, Node* c, Node* d);
Tuple* make_tuple(Node* a, Node* b, Node* c, Node* d, Node* e);
#endif