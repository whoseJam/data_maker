
#include <assert.h>
#include <gtest.h>
#include <cmath>
#include <memory>

#include "Dtstructure.h"
#include "Splay.h"
#include "Random.h"

using namespace mk;
using namespace std;
using namespace Random;

const int inf=0x3f3f3f3f;

struct Add : public Lazytag {
    int add;
    Add() : add(0) {}
    Add(int a) : add(a) {}
    void push(Add* other) {
        add += other->add;
    }
};

struct AddAndMul : public Lazytag {
    int k, b;
    AddAndMul() : k(1), b(0) {}
    AddAndMul(int k, int b) : k(k), b(b) {}
    void push(AddAndMul* other) {
        k *= other->k;
        b *= other->b; b += other->b;
    }
};

struct Rev : public Lazytag {
    bool rev;
    Rev() : rev(0) {}
    Rev(bool r) : rev(r) {}
    void push(Rev* other, SplayHandle* handle) {
        rev ^= other->rev;
        if (other->rev) handle->swap_child();
    }
};


struct Int : public Comparable {
    int val;
    Int() : val(0) {}
    Int(int v) : val(v) {}
    int compare_to(Int* other) {
        return val - other->val;
    }
};

struct Val : public Pushable {
    int val;
    Val() : val(0) {}
    Val(int v) : val(v) {}
    void push(Add* other) {
        val += other->add;
    }
};

struct Sum : public Mergeable, public Pushable {
    int sum;
    Sum() : sum(0) {}
    Sum(int v) : sum(v) {}
    void merge(Sum* other) {
        sum += other->sum;
    }
    void push(Add* other, SplayHandle* handle) {
        sum += handle->size() * other->add;
    }
};

struct XorSumAndMin : public Mergeable {
    int sum, mn;
    XorSumAndMin() : sum(0), mn(inf) {}
    XorSumAndMin(int v) : sum(v), mn(v) {}
    void merge(XorSumAndMin* other) {
        sum ^= other->sum;
        mn = min(mn, other->mn);
    }
};

struct Longest1 : public Mergeable {
    int mxlen;
    int len;
    int left1;
    int right1;
    Longest1() : len(0), left1(0), right1(0), mxlen(0) {}
    Longest1(int v) : len(1), left1(v), right1(v), mxlen(v) {}
    void merge(Longest1* other) {
        mxlen = max(mxlen, max(other->mxlen, right1 + other->left1));
        left1 = (len == left1) ? left1 + other->left1 : left1;
        right1 = (other->len == other->right1) ? right1 + other->right1 : other->right1;
        len += other->len;
    }
};

TEST(SplayTest, BasicInsert) {
    {
    Splay<Sum> splay;
    splay.insert_after(0, Sum(1));     // [1]
    splay.insert_after(0, Sum(2));     // [2, 1]
    ASSERT_EQ(splay.query_sum(1, 1).sum, 2);    
    ASSERT_EQ(splay.query_sum(2, 2).sum, 1);
    ASSERT_EQ(splay.query_sum(1, 2).sum, 3);
    splay.insert_after(2, Sum(3));     // [2, 1, 3]
    splay.insert_after(3, Sum(4));     // [2, 1, 3, 4]
    ASSERT_EQ(splay.query_sum(3, 4).sum, 7);
    ASSERT_EQ(splay.query_sum(2, 4).sum, 8);
    splay.insert_before(1, Sum(5));    // [5, 2, 1, 3, 4]
    ASSERT_EQ(splay.query_sum(1, 3).sum, 8);
    splay.insert_before(6, Sum(6));    // [5, 2, 1, 3, 4, 6]
    ASSERT_EQ(splay.query_sum(4, 6).sum, 13);
    splay.insert_after(2, Sum(7));     // [5, 2, 7, 1, 3, 4, 6]
    splay.insert_before(2, Sum(8));    // [5, 8, 2, 7, 1, 3, 4, 6]
    ASSERT_EQ(splay.query_sum(2, 3).sum, 10);
    ASSERT_EQ(splay.query_sum(3, 4).sum, 9);
    ASSERT_EQ(splay.query_sum(2, 4).sum, 17);
    }
    ASSERT_EQ(COUNT_SPLAY, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
}

TEST(SplayTest, BasicRemove) {
    {
    Splay<Sum> splay;
    splay.insert_after(0, Sum(1));
    splay.insert_after(1, Sum(2));
    splay.insert_after(2, Sum(3));     // [1, 2, 3]
    ASSERT_EQ(splay.query_sum(1, 3).sum, 6);
    splay.remove(2);                   // [1, 3]
    ASSERT_EQ(splay.size(), 2);
    ASSERT_EQ(splay.query_sum(1, 2).sum, 4);
    splay.insert_before(1, Sum(4));    // [4, 1, 3]
    splay.insert_before(4, Sum(5));    // [4, 1, 3, 5]
    splay.insert_after(4, Sum(6));     // [4, 1, 3, 5, 6]
    ASSERT_EQ(splay.query_sum(1, 5).sum, 19);
    splay.remove(1);                   // [1, 3, 5, 6]
    ASSERT_EQ(splay.size(), 4);
    ASSERT_EQ(splay.query_sum(1, 4).sum, 15);
    splay.remove(4);                   // [1, 3, 5]
    ASSERT_EQ(splay.size(), 3);
    ASSERT_EQ(splay.query_sum(2, 3).sum, 8);
    }
    ASSERT_EQ(COUNT_SPLAY, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
}

TEST(SplayTest, NotMergeable) {
    {
    Splay<Val, Add> splay;
    splay.insert_after(0, Val(1));     // [1]
    splay.insert_after(1, Val(2));     // [1, 2]
    splay.insert_after(2, Val(3));     // [1, 2, 3]
    splay.insert_before(1, Val(4));    // [4, 1, 2, 3]
    splay.insert_before(4, Val(5));    // [4, 1, 2, 5, 3]
    splay.insert_after(4, Val(6));     // [4, 1, 2, 5, 6, 3]
    {   vector<int> ans = {4, 1, 2, 5, 6, 3};
        for (int i = 0; i < 6; i++) {
            ASSERT_EQ(splay.query_info(i + 1).val, ans[i]);
        }
    }
    splay.insert(2, 4, Add(4));                 // [4, 5, 6, 9, 6, 3]
    ASSERT_EQ(splay.query_info(2).val, 5);
    ASSERT_EQ(splay.query_info(3).val, 6);
    ASSERT_EQ(splay.query_info(1).val, 4);
    }
    ASSERT_EQ(COUNT_SPLAY, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
}

TEST(SplayTest, BasicSplit) {
    {
    Splay<Val, Add> splay;
    for (int i = 1; i <= 10; i++) {
        splay.insert_after(splay.size(), Val(i));
    }   // [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    ASSERT_EQ(splay.query_info(4).val, 4);
    auto sub_sequence = splay.split(3, 7); // [3, 4, 5, 6, 7] + [1, 2, 8, 9, 10]
    {   vector<int> original = {1, 2, 8, 9, 10};
        vector<int> subseq = {3, 4, 5, 6, 7};
        for (int i = 1; i <= 5; i++) {
            ASSERT_EQ(splay.query_info(i).val, original[i - 1]);
            ASSERT_EQ(sub_sequence.query_info(i).val, subseq[i - 1]);
        }
    }
    sub_sequence.insert(1, 3, Add(100)); // [103, 104, 105, 6, 7]
    splay.insert_after(4, std::move(sub_sequence)); // [1, 2, 8, 9, 103, 104, 105, 6, 7, 10]
    {   vector<int> ans = {1, 2, 8, 9, 103, 104, 105, 6, 7, 10};
        for (int i = 1; i <= 10; i++)
            ASSERT_EQ(splay.query_info(i).val, ans[i - 1]);
    }
    }
    ASSERT_EQ(COUNT_SPLAY, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
}

TEST(SplayTest, BasicHandle) {
    {
    Splay<Val, Add> splay;
    SplayHandle* h, *t;
    for (int i = 1; i <= 10; i++) {
        auto tmp = splay.insert_after(splay.size(), Val(i));
        if (i == 1) h = tmp;
        if (i == 6) t = tmp; 
    }   // [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    splay.insert(h, t, Add(100));
    {   vector<int> ans = {101, 102, 103, 104, 105, 106, 7, 8, 9, 10};
        for (int i = 1; i <= 10; i++)
            ASSERT_EQ(splay.query_info(i).val, ans[i - 1]);
    }
    }
    ASSERT_EQ(COUNT_SPLAY, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
}

TEST(SplayTest, StrongTestLongest1) {
    {
    int n = 1000;
    vector<int> check; check.resize(n);
    Splay<Longest1> splay;
    for (int i = 0; i < n; i++) {
        check[i] = rand_int(0, 1);
        splay.insert_after(i, Longest1(check[i]));
    }
    for (int i = 0; i < n; i++) {
        char type = rand_int(0, 1);
        if (type == 0) {
            int l = rand_int(1, n);
            int r = rand_int(1, n);
            if (l > r) swap(l, r);
            int ans = 0, cur1 = 0;
            for (int j = l-1; j <= r-1; j++) {
                if (check[j] == 1) cur1++;
                else cur1 = 0; ans = max(ans, cur1);
            }
            ASSERT_EQ(splay.query_sum(l, r).mxlen, ans);
        } else {
            int pos = rand_int(1, n);
            int to = rand_int(0, 1);
            check[pos - 1] = to;
            splay.insert(pos, Longest1(to));
        }
    }
    }
    ASSERT_EQ(COUNT_SPLAY, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
}

TEST(SplayTest, StrongTestRevAndXorAndMin) {
    {
    int n = 1000;
    vector<int> check; check.resize(n);
    Splay<XorSumAndMin, Rev> splay;
    for (int i = 0; i < n; i++) {
        check[i] = rand_int(0, 100000);
        splay.insert_after(i, XorSumAndMin(check[i]));
    }
    for (int i = 0; i < n; i++) {
        int type = rand_int(0, 1);
        if (type == 0) {
            int l = rand_int(1, n);
            int r = rand_int(1, n);
            if (l > r) swap(l, r);
            int ans = 0, mn = inf;
            for (int j = l-1; j <= r-1; j++) {
                mn = min(mn, check[j]);
                ans ^= check[j];
            }
            auto q = splay.query_sum(l, r);
            ASSERT_EQ(q.mn, mn);
            ASSERT_EQ(q.sum, ans);
        } else {
            int l = rand_int(1, n);
            int r = rand_int(1, n);
            if (l > r) swap(l, r);
            for (int j = l-1, k = r-1; j <= k; j++, k--) swap(check[j], check[k]); 
            splay.insert(l, r, Rev(true));
        }
    }
    }
    ASSERT_EQ(COUNT_SPLAY, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
}

TEST(SplayTest, BasicComparable) {
    {
    Splay<Int> splay;
    auto v1 = splay.insert(Int(1));
    auto v5 = splay.insert(Int(5));
    auto v3 = splay.insert(Int(3));
    ASSERT_EQ(splay.rank(v1), 1);
    ASSERT_EQ(splay.rank(v5), 3);
    ASSERT_EQ(splay.rank(v3), 2);
    auto v4 = splay.insert(Int(4));
    auto v2 = splay.insert(Int(2));
    auto v33 = splay.insert(Int(3));
    ASSERT_EQ(splay.rank(v33), splay.rank(v3));
    ASSERT_EQ(splay.rank(v4), 5);
    ASSERT_EQ(splay.rank(v2), 2);
    ASSERT_EQ(splay.rank(v3), 3);
    ASSERT_EQ(v3->same(), 2);
    }
    ASSERT_EQ(COUNT_SPLAY, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
}

TEST(SplayTest, PressureTest) {
    {
    int n = 100000;
    Splay<Sum, Rev> splay;
    for (int i = 0; i < n; i++)
        splay.insert_after(i, Sum(1));
    for (int i = 0; i < 300000; i++) {
        int type = rand_int(0, 1);
        if (type == 0) {
            int l = rand_int(1, n);
            int r = rand_int(1, n);
            if (l > r) swap(l, r);
            splay.insert(l, r, Rev(true));
        } else {
            int l = rand_int(1, n);
            int r = rand_int(1, n);
            if (l > r) swap(l, r);
            splay.query_sum(l, r);
        }
    }

    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}