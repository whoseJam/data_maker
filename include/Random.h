#ifndef RANDOM_H
#define RANDOM_H

#include <vector>
#include <cstdlib>
#include <iostream>

namespace Random {
    int rand_int(int l, int r);
    char rand_char(char l, char r);
    void rand_vector(std::vector<int>& vec);

    // 从vector中随机选择一个元素，并将其取出后返回
    template<typename T>
    T rand_and_take_out_from_vector(std::vector<T>& vec) {
        if (vec.size() == 0) {
            std::cout << "Random::vec in randAndTakeOutFromVector has size = 0\n";
            exit(-1);
        }
        int idx = rand_int(0, vec.size() - 1);
        T val = vec[idx];
        vec.erase(vec.begin() + idx);
        return val;
    }
    
    // 从vector中随机选择一个元素，并返回
    template<typename T>
    T rand_from_vector(std::vector<T>& vec) {
        int idx = rand_int(0, vec.size() - 1);
        T val = vec[idx];
        return val;
    }
}

#endif