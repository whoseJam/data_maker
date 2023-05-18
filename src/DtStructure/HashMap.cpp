#include <iostream>

#include "HashMap.h"
#include "Integer.h"

using namespace std;

HashMap::HashMap() {
    CALL(FUNCTION);
    diff = 0;
    total = 0;
}

void HashMap::insert(shared_ptr<Hashable> obj) {
    CALL(FUNCTION);
    uint code = obj->hash_code();
    if (header.find(code) == header.end() || !header[code]) {
        shared_ptr<HashNode> new_node = make_shared<HashNode>();
        header[code] = new_node;
        new_node->cnt = 1;
        new_node->hold = obj;
        diff++; total++;
    } else {
        shared_ptr<HashNode> iter = header[code];
        while (true) {
            if (iter->hold->equal(obj)) {
                iter->cnt++;
                total++;
                return;
            }
            if (iter->next) iter = iter->next;
            else break;
        }
        iter->next = make_shared<HashNode>();
        iter->next->cnt = 1;
        iter->next->hold = obj;
        diff++; total++;
    }
}

void HashMap::remove(shared_ptr<Hashable> obj) {
    CALL(FUNCTION);
    uint code = obj->hash_code();
    if (header.find(code) == header.end() || !header[code]) return;
    shared_ptr<HashNode> iter = header[code];
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
    shared_ptr<HashNode> last = header[code];
    while (true) {
        if (iter->hold->equal(obj)) {
            iter->cnt--; total--;
            if (iter->cnt == 0) {
                last->next = iter->next;
                diff--;
            }
            return;
        }
        iter = iter->next;
        last = last->next;
        if (!iter) break;
    }
}

bool HashMap::in_hashmap(shared_ptr<Hashable> obj) {
    CALL(FUNCTION);
    uint code = obj->hash_code();
    if (header.find(code) == header.end() || !header[code]) return false;
    shared_ptr<HashNode> iter = header[code];
    while (true) {
        if (iter->hold->equal(obj))
            return true;
        iter = iter->next;
        if (!iter) break;
    }
    return false;
}

int HashMap::query_total() {
    CALL(FUNCTION);
    return total;
}

int HashMap::query_different() {
    CALL(FUNCTION);
    return diff;
}

namespace mk {
    shared_ptr<HashMap> hash_map() {
        return make_shared<HashMap>();
    }
}
