#ifndef RANDOM_H
#define RANDOM_H

#include <vector>
#include <cstdlib>
#include <iostream>

#define RAND_AND_TAKE_OUT_FROM_VECTOR(vec, type) \
    ({ \
        if (vec.size() == 0) { \
            std::cout << "Random::vec in randAndTakeOutFromVector has size = 0\n"; \
            exit(-1); \
        } \
        int idx = rand_int(0, vec.size() - 1); \
        type val = vec[idx]; \
        vec.erase(vec.begin() + idx); \
        val; \
    })

#define RAND_FROM_VECTOR(vec, type) \
    ({ \
        int idx = rand_int(0, vec.size() - 1); \
        type val = vec[idx]; \
        val; \
    })

namespace Random {
    int rand_int(int l, int r);
    char rand_char(char l, char r);
    void rand_vector(std::vector<int>& vec);
}

#endif