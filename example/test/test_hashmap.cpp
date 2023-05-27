
#include <gtest.h>

#include "HashMap.h"

using namespace mk;

struct Int {
    int x;
    Int(int x) : x(x) {}
    Int(const Int& other) : x(other.x) {}
    uint hash_code() { return x; }
    bool equal(const Int& other) { return x == other.x; }
};

struct Intx {
    int x;
    Intx(int x) : x(x) {}
    Intx(const Intx& other) : x(other.x) {}
    uint hash_code() { return 0; }
    bool equal(const Intx& other) { return x == other.x; }
};

TEST(HashMapTest, BasicTest) {
    HashMap<Int> hash;
    for (int i = 1; i <= 10; i++)
        hash.insert(Int(i));
    ASSERT_EQ(hash.query_different(), 10);
    ASSERT_EQ(hash.query_total(), 10);
    for (int i = 1; i <= 10; i++)
        hash.insert(Int(i));
    ASSERT_EQ(hash.query_different(), 10);
    ASSERT_EQ(hash.query_total(), 20);
    for (int i = 1; i <= 5; i++)
        hash.remove(Int(i));
    ASSERT_EQ(hash.query_different(), 10);
    ASSERT_EQ(hash.query_total(), 15);
    for (int i = 1; i <= 4; i++)
        hash.remove(Int(i));
    ASSERT_EQ(hash.query_different(), 6);
    ASSERT_EQ(hash.query_total(), 11);
}

TEST(HashMapTest, ChainTest) {
    HashMap<Intx> hash;
    for (int i = 1; i <= 10; i++)
        hash.insert(Intx(i));
    ASSERT_EQ(hash.query_different(), 10);
    ASSERT_EQ(hash.query_total(), 10);
    for (int i = 1; i <= 10; i++)
        hash.insert(Intx(i));
    ASSERT_EQ(hash.query_different(), 10);
    ASSERT_EQ(hash.query_total(), 20);
    for (int i = 1; i <= 5; i++)
        hash.remove(Intx(i));
    ASSERT_EQ(hash.query_different(), 10);
    ASSERT_EQ(hash.query_total(), 15);
    for (int i = 1; i <= 4; i++)
        hash.remove(Intx(i));
    ASSERT_EQ(hash.query_different(), 6);
    ASSERT_EQ(hash.query_total(), 11);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}