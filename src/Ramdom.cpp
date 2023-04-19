
#include <ctime>
#include <random>
#include <cstdlib>
#include <iostream>

#include "../include/Random.h"

using namespace std;

namespace Random {
    bool inited = false;
    std::random_device rd;

    void init() {
        inited = true;
        srand(time(0));
    }

    // 随机生成一个[l,r]中的数字
    int rand_int(int l, int r) {
        if (!inited) init();
        return rd() % (r - l + 1) + l;
    }

    // 随机生成一个[l,r]中的char字符
    char rand_char(char l, char r) {
        if (!inited) init();
        return rd() % (r - l + 1) + l;
    }

    // 使用洗牌算法打乱一个vector
    void rand_vector(vector<int>& vec) { 
        if (!inited) init();
        for (int i = 1; i < vec.size(); i++) {
            int pos = rand_int(0, i - 1);
            swap(vec[i], vec[pos]);
        }
    }

}