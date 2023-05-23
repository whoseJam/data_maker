
#include <assert.h>
#include <gtest.h>
#include <memory>

#include "Random.h"
#include "Dtstructure.h"
#include "Splay.h"
#include "Tree.h"
#include "ETT.h"

using namespace mk;
using namespace std;
using namespace Random;

struct Add : public Lazytag {
    int add;
    Add() : add(0) {}
    Add(int a) : add(a) {}
    virtual void push(Lazytag* tag, Handle* handle) override {
        auto add_tag = dynamic_cast<Add*>(tag);
        if (add_tag) {
            add += add_tag->add;
        }
    }
};

struct Val : public Info, public Pushable {
    int val;
    Val() : val(0) {}
    Val(int v) : val(v) {}
    virtual void push(Lazytag* tag, Handle* handle) override {
        auto add_tag = dynamic_cast<Add*>(tag);
        if (add_tag) {
            val += add_tag->add;
        }
    }
};


struct Sum : public Mergeable, public Pushable {
    int sum;
    Sum() : sum(0) {}
    Sum(int s) : sum(s) {}
    void merge(Mergeable* info) override {
        auto sum_info = dynamic_cast<Sum*>(info);
        if (sum_info) {
            sum += sum_info->sum;
        }
    }
    virtual void push(Lazytag* tag, Handle* handle) override {
        auto add_tag = dynamic_cast<Add*>(tag);
        if (add_tag) {
            auto ett_handle = dynamic_cast<ETTHandle*>(handle);
            sum += add_tag->add * ett_handle->vertex_size();
        }
    }
};


TEST(ETTTest, BasicTree) {
    {
    ETT<Sum> ett;
    ett.new_node(0, Sum(3));   // 0(sum=3)
    ASSERT_EQ(ett.query_info(0).sum, 3);
    ASSERT_EQ(ett.query_sum(0).sum, 3);
    ASSERT_EQ(COUNT_ETTINFO, 2); ASSERT_EQ(COUNT_SPLAY ,1);
    ett.new_node(1, Sum(2));   // 1(sum=2)
    ASSERT_EQ(COUNT_ETTINFO, 4); ASSERT_EQ(COUNT_SPLAY, 2);
    ASSERT_EQ(ett.is_accessible(0, 1), false);
    ASSERT_EQ(ett.is_accessible(1, 2), false);
    ett.link(0, 1);                 // 0(sum=5) -> 1(sum=2)
    ASSERT_EQ(COUNT_ETTINFO, 8);
    ASSERT_EQ(ett.is_accessible(0, 1), true);
    ASSERT_EQ(ett.is_ancestor_of(0, 1), true);
    ASSERT_EQ(ett.is_ancestor_of(1, 0), false);
    ASSERT_EQ(COUNT_ETTINFO, 8); ASSERT_EQ(COUNT_SPLAY, 1);
    ett.make_root(1);                 // 1(sum=5) -> 0(sum=3)
    ASSERT_EQ(COUNT_ETTINFO, 8); ASSERT_EQ(COUNT_SPLAY, 1);
    ASSERT_EQ(ett.is_ancestor_of(0, 1), false);
    ASSERT_EQ(ett.is_ancestor_of(1, 0), true);
    ASSERT_EQ(ett.query_sum(0).sum, 3);
    ASSERT_EQ(ett.query_sum(1).sum, 5);
    ASSERT_EQ(COUNT_ETTINFO, 8);
    
    ett.new_node(2, Sum(10)); // 2(sum=10)
    ASSERT_EQ(COUNT_ETTINFO, 10); ASSERT_EQ(COUNT_SPLAY, 2);
    ASSERT_EQ(ett.query_sum(2).sum, 10);
    ett.new_node(3);                  // 3(sum=0)
    
    ASSERT_EQ(COUNT_ETTINFO, 12); ASSERT_EQ(COUNT_SPLAY, 3);
    ASSERT_EQ(ett.query_sum(3).sum, 0);
    ett.link(2, 3);                // 2(sum=10) -> 3(sum=0)
    ASSERT_EQ(COUNT_ETTINFO, 16); ASSERT_EQ(COUNT_SPLAY, 2);
    ASSERT_EQ(ett.query_sum(2).sum, 10);
    ASSERT_EQ(ett.query_sum(3).sum, 0);
    ett.link(1, 2);
    ASSERT_EQ(COUNT_ETTINFO, 20); ASSERT_EQ(COUNT_SPLAY, 1);
    /* 1(sum=15) -> 2(sum=10) -> 3(sum=0)
                 -> 0(sum=3)
    */
    ASSERT_EQ(ett.is_accessible(0, 2), true);
    ASSERT_EQ(ett.is_ancestor_of(1, 3), true);
    ett.cut(1, 0);
    ASSERT_EQ(COUNT_ETTINFO, 16);
    ASSERT_EQ(ett.query_sum(1).sum, 12);
    ett.make_root(0);
    ASSERT_EQ(ett.query_sum(0).sum, 3);
    ett.cut(1, 0);
    ASSERT_EQ(ett.query_sum(1).sum, 12);
    ASSERT_EQ(ett.query_info(1).sum, 2);
    ASSERT_EQ(ett.is_accessible(0, 1), false);
    ASSERT_EQ(ett.is_ancestor_of(1, 0), false);
    }
    ASSERT_EQ(COUNT_ETT, 0);
    ASSERT_EQ(COUNT_ETTINFO, 0);
    ASSERT_EQ(COUNT_ETTTAG, 0);
    ASSERT_EQ(COUNT_SPLAY, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
}

TEST(ETTTest, BasicSubtree) {
    {
    ETT<Sum, Add> ett;
    for (int i = 1; i <= 10; i++) ett.new_node(i, Sum(i));
    for (int i = 2; i <= 5; i++) ett.link(1, i);
    for (int i = 6; i <= 10; i++) ett.link(2, i);
    ASSERT_EQ(ett.query_sum(2).sum, 2+6+7+8+9+10);
    ASSERT_EQ(ett.query_info(2).sum, 2);
    ASSERT_EQ(ett.is_ancestor_of(2, 6), true);
    ASSERT_EQ(ett.is_ancestor_of(6, 2), false);
    ASSERT_EQ(ett.is_ancestor_of(2, 3), false);
    ett.insert(2, Add(3));
    ASSERT_EQ(ett.query_info(6).sum, 6+3);
    ASSERT_EQ(ett.query_sum(2).sum, 2+6+7+8+9+10 + 3+3+3+3+3+3);
    ASSERT_EQ(ett.query_sum(3).sum, 3);
    ett.cut(2, 1);
    ett.link(3, 2);
    ASSERT_EQ(ett.query_sum(3).sum, 3+60);
    ASSERT_TRUE(ett.is_ancestor_of(3, 10));
    ett.cut(10, 2);
    ASSERT_EQ(ett.query_sum(3).sum, 63-13);
    ASSERT_FALSE(ett.is_ancestor_of(3, 10));
    }
    ASSERT_EQ(COUNT_ETT, 0);
    ASSERT_EQ(COUNT_ETTINFO, 0);
    ASSERT_EQ(COUNT_ETTTAG, 0);
    ASSERT_EQ(COUNT_SPLAYNODE, 0);
    ASSERT_EQ(COUNT_SPLAY, 0);
}

TEST(ETTTest, MemoryLeakTest) {
    {
    ETT<bool> ett;
    int n = 1000;
    int m = 3000;
    vector<int> fa; fa.resize(n + 5);
    for (int i = 1; i <= n; i++) ett.new_node(i);
    std::cout<<"Finish\n";
    for (int i = 2; i <= n; i++) {
        fa[i] = rand_int(1, i-1);
        ett.link(fa[i], i);
        if (i % 1000 == 0) std::cout<<"op i="<<i<<"\n";
    }
    cout<<"start operate\n";
    for (int i = 1; i <= m; i++) {
        if (i % 1000 == 0) std::cout<<"op i="<<i<<"\n";
        int x = rand_int(1, n);
        int y = rand_int(1, n);
        if (ett.is_root(x)) {
            int old = x;
            while (x == old) {
                x = rand_int(1, n);
            }
        }
        while (ett.is_ancestor_of(x, y) || x == y) {
            y = rand_int(1, n);
        }
        ett.cut(x, fa[x]);
        ett.link(y, x);
        fa[x] = y;
    }
    }
    if ((COUNT_SPLAY | COUNT_ETTTAG | COUNT_SPLAYNODE | COUNT_ETTINFO | COUNT_ETT) != 0) {
        cout<<"Error\n";
        exit(-1);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}