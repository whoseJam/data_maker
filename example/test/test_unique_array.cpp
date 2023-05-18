#include <iostream>

#include "HashMap.h"
#include "Array.h"

using std::shared_ptr;
using std::cout;

int main() {
    auto h = std::make_shared<HashMap>();
    auto ar = std::make_shared<Array>();
    auto t = ar->length(100)->fill(integer(1, 120));
    ar->when_generating_per_element([&](shared_ptr<Array> This, int idx) -> void{
        shared_ptr<Integer> cur = This->get<Integer>(idx);
        while(true) {
            if (h->in_hashmap(cur)) {
                cur->generate(1);
            } else break;
        } h->insert(cur);
    });
    ar->generate(0); ar->out();
    return 0;
}