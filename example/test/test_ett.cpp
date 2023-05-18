
#include "gtest/gtest.h"
#include <assert.h>
#include <gtest.h>
#include <memory>

#include "Tree.h"
#include "ETT.h"

using namespace mk;
using namespace std;

struct Val : public Info {
    int val;
    Val() : val(0) {}
    Val(int v) : val(v) {}
};


struct Sum : public Mergeable {
    int sum;
    Sum() : sum(0) {}
    Sum(int s) : sum(s) {}
    void merge(std::shared_ptr<Mergeable> o) {
        auto other = dynamic_pointer_cast<Sum>(o);
        sum += other->sum;
    }
};

struct Add : public Lazytag {
    int add;
    Add() : add(0) {}
    Add(int a) : add(a) {}
    void push(shared_ptr<Lazytag> o, shared_ptr<Handle> h) override {
        auto other = dynamic_pointer_cast<Add>(o);
        other->add += add;
    }
    void push(shared_ptr<Info> o, shared_ptr<Handle> h) override {
        int size = 0;
        {   auto sp = dynamic_pointer_cast<SplayHandle>(h);
            if (sp) size = sp->size();
        }   // Splay

        {   auto other = dynamic_pointer_cast<Val>(o);
            if (other) { other->val += add; }
        }   // Val
        {   auto other = dynamic_pointer_cast<Sum>(o);
            if (other) { other->sum += add * size; }
        }   // Sum
    }
};


TEST(ETTTest, BasicTree) {
    auto ett = make_shared<ETT<Sum>>();
    ett->new_node(0, Sum(3));   // 0(sum=3)
    ett->new_node(1, Sum(2));   // 1(sum=2)
    ASSERT_EQ(ett->is_accessible(0, 1), false);
    ASSERT_EQ(ett->is_accessible(1, 2), false);
    ett->link(0, 1);                 // 0(sum=5) -> 1(sum=2)
    ASSERT_EQ(ett->is_accessible(0, 1), true);
    ASSERT_EQ(ett->is_ancestor_of(0, 1), true);
    ASSERT_EQ(ett->is_ancestor_of(1, 0), false);
    ett->make_root(1);                 // 1(sum=5) -> 0(sum=3)
    ASSERT_EQ(ett->is_ancestor_of(0, 1), false);
    ASSERT_EQ(ett->is_ancestor_of(1, 0), true);
    ASSERT_EQ(ett->query_sum(0)->sum, 3);
    ASSERT_EQ(ett->query_sum(1)->sum, 5);
    ett->new_node(2, Sum(10)); // 2(sum=10)
    ASSERT_EQ(ett->query_sum(2)->sum, 10);
    ett->new_node(3);                  // 3(sum=0)
    ASSERT_EQ(ett->query_sum(3)->sum, 0);
    ett->link(2, 3);                // 2(sum=10) -> 3(sum=0)
    ASSERT_EQ(ett->query_sum(2)->sum, 10);
    ASSERT_EQ(ett->query_sum(3)->sum, 0);
    ett->link(1, 2); 
    /* 1(sum=15) -> 2(sum=10) -> 3(sum=0)
                 -> 0(sum=3)
    */
    ASSERT_EQ(ett->is_accessible(0, 2), true);
    ASSERT_EQ(ett->is_ancestor_of(1, 3), true);
    ett->cut(1, 0);
    ASSERT_EQ(ett->query_sum(1)->sum, 12);
    ett->make_root(0);
    ASSERT_EQ(ett->query_sum(0)->sum, 3);
    ett->cut(1, 0);
    ASSERT_EQ(ett->query_sum(1)->sum, 12);
    ASSERT_EQ(ett->query_info(1)->sum, 2);
    ASSERT_EQ(ett->is_accessible(0, 1), false);
    ASSERT_EQ(ett->is_ancestor_of(1, 0), false);
}

TEST(ETTTest, BasicSubtree) {
    auto ett = make_shared<ETT<Sum, Add>>();
    for (int i = 1; i <= 10; i++) ett->new_node(i, Sum(i));
    for (int i = 2; i <= 5; i++) ett->link(1, i);
    for (int i = 6; i <= 10; i++) ett->link(2, i);
    ASSERT_EQ(ett->query_sum(2), 2+6+7+8+9+10);
    ASSERT_EQ(ett->query_info(2), 2);
    ASSERT_EQ(ett->is_ancestor_of(2, 6), true);
    ASSERT_EQ(ett->is_ancestor_of(6, 2), false);
    ASSERT_EQ(ett->is_ancestor_of(2, 3), false);
    
    ett->insert(2, Add(3));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}