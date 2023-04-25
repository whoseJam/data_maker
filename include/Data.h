#ifndef DATA_H
#define DATA_H

#include "AttributeGroup.h"
#include "IntegerWrapper.h"
#include "Permutation.h"
#include "Destroy.h"
#include "Define.h"
#include "Random.h"
#include "Vertex.h"
#include "Array.h"
#include "Graph.h"
#include "Tuple.h"
#include "Tree.h"
#include "Node.h"
#include "Edge.h"
#include "Pre.h"
#include "IO.h"

#define BUILD(ele) \
    ele->generate(0); \
    ele->out(); \
    Destroy::get()->start_service(); \
    ele->destroy(); \
    Destroy::get()->stop_service();

#define T(x) x, x##Panel

template<typename T>
int count_different_element(const std::vector<T*> elements) {
    std::vector<T*> diff;
    for (int i = 0; i < elements.size(); i++) {
        bool flag = true;
        for (int j = 0; j < diff.size(); j++) {
            if (diff[j]->equal(elements[i])) {
                flag = false; break;
            }
        } 
        if (flag) diff.push_back(elements[i]);
    }
    return diff.size();
}

#endif