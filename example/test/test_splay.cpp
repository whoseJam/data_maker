
#include <assert.h>
#include <gtest.h>
#include <cmath>

#include "Splay.h"
#include "Array.h"
#include "Tuple.h"
#include "Option.h"
#include "Character.h"

using namespace mk;
using namespace std;
using namespace Random;

const int inf=0x3f3f3f3f;

struct Int : public Comparable {
    int val;
    Int() : val(0) {}
    Int(int v) : val(v) {}
    int compare_to(shared_ptr<Comparable> o) override {
        int v = dynamic_pointer_cast<Int>(o)->val;
        return val - v;
    }
};

struct Val : public Info {
    int val;
    Val() : val(0) {}
    Val(int v) : val(v) {}
};

struct Sum : public Mergeable {
    int sum;
    Sum() : sum(0) {}
    Sum(int v) : sum(v) {}
    void merge(shared_ptr<Mergeable> o) override {
        auto other = dynamic_pointer_cast<Sum>(o);
        sum += other->sum;
    }
};

struct XorSumAndMin : public Mergeable {
    int sum, mn;
    XorSumAndMin() : sum(0), mn(inf) {}
    XorSumAndMin(int v) : sum(v), mn(v) {}
    void merge(shared_ptr<Mergeable> o) override {
        auto other = dynamic_pointer_cast<XorSumAndMin>(o);
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
    void merge(shared_ptr<Mergeable> o) override {
        auto other = dynamic_pointer_cast<Longest1>(o);
        mxlen = max(mxlen, max(other->mxlen, right1 + other->left1));
        left1 = (len == left1) ? left1 + other->left1 : left1;
        right1 = (other->len == other->right1) ? right1 + other->right1 : other->right1;
        len += other->len;
    }
};

struct Rev : public Lazytag {
    bool rev;
    Rev() : rev(0) {}
    Rev(bool r) : rev(r) {}
    void push(shared_ptr<Lazytag> o, shared_ptr<Handle> h) override {
        auto other = dynamic_pointer_cast<Rev>(o);
        other->rev ^= rev;
        {   auto sp = dynamic_pointer_cast<SplayHandle>(h);
            if (rev) sp->swap_child();
        }   //Splay
    }
    void push(shared_ptr<Info> o, shared_ptr<Handle> h) override {}
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

struct AddAndMul : public Lazytag {
    int k, b;
    AddAndMul() : k(1), b(0) {}
    AddAndMul(int k, int b) : k(k), b(b) {}
    void push(shared_ptr<Lazytag> o, shared_ptr<Handle> h) override {
        auto other = dynamic_pointer_cast<AddAndMul>(o);
        other->k *= k;
        other->b *= k; other->b += b;
    }
    void push(shared_ptr<Info> o, shared_ptr<Handle> h) override {
        int size = dynamic_pointer_cast<SplayHandle>(h)->size();
        auto other = dynamic_pointer_cast<Sum>(o);
        other->sum = (other->sum) * k + b;
    }
};

TEST(SplayTest, BasicInsert) {
    auto splay = make_shared<Splay<Sum>>();
    splay->insert_after(0, Sum(1));     // [1]
    splay->insert_after(0, Sum(2));     // [2, 1]
    ASSERT_EQ(splay->query_sum(1, 1)->sum, 2);
    ASSERT_EQ(splay->query_sum(2, 2)->sum, 1);
    ASSERT_EQ(splay->query_sum(1, 2)->sum, 3);
    splay->insert_after(2, Sum(3));     // [2, 1, 3]
    splay->insert_after(3, Sum(4));     // [2, 1, 3, 4]
    ASSERT_EQ(splay->query_sum(3, 4)->sum, 7);
    ASSERT_EQ(splay->query_sum(2, 4)->sum, 8);
    splay->insert_before(1, Sum(5));    // [5, 2, 1, 3, 4]
    ASSERT_EQ(splay->query_sum(1, 3)->sum, 8);
    splay->insert_before(6, Sum(6));    // [5, 2, 1, 3, 4, 6]
    ASSERT_EQ(splay->query_sum(4, 6)->sum, 13);
    splay->insert_after(2, Sum(7));     // [5, 2, 7, 1, 3, 4, 6]
    splay->insert_before(2, Sum(8));    // [5, 8, 2, 7, 1, 3, 4, 6]
    ASSERT_EQ(splay->query_sum(2, 3)->sum, 10);
    ASSERT_EQ(splay->query_sum(3, 4)->sum, 9);
    ASSERT_EQ(splay->query_sum(2, 4)->sum, 17);
}

TEST(SplayTest, BasicRemove) {
    auto splay = make_shared<Splay<Sum>>();
    splay->insert_after(0, Sum(1));
    splay->insert_after(1, Sum(2));
    splay->insert_after(2, Sum(3));     // [1, 2, 3]
    ASSERT_EQ(splay->query_sum(1, 3)->sum, 6);
    splay->remove(2);                   // [1, 3]
    ASSERT_EQ(splay->size(), 2);
    ASSERT_EQ(splay->query_sum(1, 2)->sum, 4);
    splay->insert_before(1, Sum(4));    // [4, 1, 3]
    splay->insert_before(4, Sum(5));    // [4, 1, 3, 5]
    splay->insert_after(4, Sum(6));     // [4, 1, 3, 5, 6]
    ASSERT_EQ(splay->query_sum(1, 5)->sum, 19);
    splay->remove(1);                   // [1, 3, 5, 6]
    ASSERT_EQ(splay->size(), 4);
    ASSERT_EQ(splay->query_sum(1, 4)->sum, 15);
    splay->remove(4);                   // [1, 3, 5]
    ASSERT_EQ(splay->size(), 3);
    ASSERT_EQ(splay->query_sum(2, 3)->sum, 8);
}

TEST(SplayTest, NotMergeable) {
    auto splay = make_shared<Splay<Val, Add>>();
    splay->insert_after(0, Val(1));     // [1]
    splay->insert_after(1, Val(2));     // [1, 2]
    splay->insert_after(2, Val(3));     // [1, 2, 3]
    splay->insert_before(1, Val(4));    // [4, 1, 2, 3]
    splay->insert_before(4, Val(5));    // [4, 1, 2, 5, 3]
    splay->insert_after(4, Val(6));     // [4, 1, 2, 5, 6, 3]
    {   vector<int> ans = {4, 1, 2, 5, 6, 3};
        for (int i = 0; i < 6; i++) {
            ASSERT_EQ(splay->query_info(i + 1)->val, ans[i]);
        }
    }
    splay->insert(2, 4, Add(4));                 // [4, 5, 6, 9, 6, 3]
    ASSERT_EQ(splay->query_info(2)->val, 5);
    ASSERT_EQ(splay->query_info(3)->val, 6);
    ASSERT_EQ(splay->query_info(1)->val, 4);
}

TEST(SplayTest, BasicSplit) {
    auto splay = make_shared<Splay<Val, Add>>();
    for (int i = 1; i <= 10; i++) {
        splay->insert_after(splay->size(), Val(i));
    }   // [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    ASSERT_EQ(splay->query_info(4)->val, 4);
    auto sub_sequence = splay->split(3, 7); // [3, 4, 5, 6, 7] + [1, 2, 8, 9, 10]
    {   vector<int> original = {1, 2, 8, 9, 10};
        vector<int> subseq = {3, 4, 5, 6, 7};
        for (int i = 1; i <= 5; i++) {
            ASSERT_EQ(splay->query_info(i)->val, original[i - 1]);
            ASSERT_EQ(sub_sequence->query_info(i)->val, subseq[i - 1]);
        }
    }
    sub_sequence->insert(1, 3, Add(100)); // [103, 104, 105, 6, 7]
    splay->insert_after(4, sub_sequence); // [1, 2, 8, 9, 103, 104, 105, 6, 7, 10]
    {   vector<int> ans = {1, 2, 8, 9, 103, 104, 105, 6, 7, 10};
        for (int i = 1; i <= 10; i++)
            ASSERT_EQ(splay->query_info(i)->val, ans[i - 1]);
    }
}

TEST(SplayTest, BasicHandle) {
    auto splay = make_shared<Splay<Val, Add>>();
    shared_ptr<SplayHandle> h, t;
    for (int i = 1; i <= 10; i++) {
        auto tmp = splay->insert_after(splay->size(), Val(i));
        if (i == 1) h = tmp;
        if (i == 6) t = tmp; 
    }   // [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    splay->insert(h, t, Add(100));
    {   vector<int> ans = {101, 102, 103, 104, 105, 106, 7, 8, 9, 10};
        for (int i = 1; i <= 10; i++)
            ASSERT_EQ(splay->query_info(i)->val, ans[i - 1]);
    }
}

TEST(SplayTest, StrongTestLongest1) {
    int n = 1000;
    auto origin = mk::array(n, integer(0, 1));
    auto arr = mk::array(n, 
        option(
            mk::tuple(character('A'), integer(1, n), integer(1, n))->after_generate([](shared_ptr<Tuple> tp)->void {
                auto l = tp->get<Integer>(1); auto lv = l->get();
                auto r = tp->get<Integer>(2); auto rv = r->get();
                if (lv > rv) { l->set(rv); r->set(lv); }
            }), 50,
            mk::tuple(character('M'), integer(1, n), integer(0, 1)), 50
        )
    );
    origin->generate(0, nullptr);
    arr->generate(0, nullptr);
    vector<int> check; check.resize(n);
    auto splay = make_shared<Splay<Longest1>>();
    for (int i = 0; i < n; i++) {
        check[i] = origin->get<Integer>(i)->get();
        splay->insert_after(i, Longest1(check[i]));
    }
    for (int i = 0; i < n; i++) {
        char type = arr->get<Option>(i)->get<Tuple>()->get<Character>(0)->get();
        if (type == 'A') {
            int l = arr->get<Option>(i)->get<Tuple>()->get<Integer>(1)->get();
            int r = arr->get<Option>(i)->get<Tuple>()->get<Integer>(2)->get();
            int ans = 0, cur1 = 0;
            for (int j = l-1; j <= r-1; j++) {
                if (check[j] == 1) cur1++;
                else cur1 = 0; ans = max(ans, cur1);
            }
            ASSERT_EQ(splay->query_sum(l, r)->mxlen, ans);
        } else {
            int pos = arr->get<Option>(i)->get<Tuple>()->get<Integer>(1)->get();
            int to = arr->get<Option>(i)->get<Tuple>()->get<Integer>(2)->get();
            check[pos - 1] = to;
            splay->insert(pos, Longest1(to));
        }
    }
}

TEST(SplayTest, StrongTestRevAndXorAndMin) {
    int n = 1000;
    auto origin = mk::array(n, integer(0, 100000));
    auto arr = mk::array(n, 
        option(
            mk::tuple(character('A'), integer(1, n), integer(1, n))->after_generate([](shared_ptr<Tuple> tp)->void {
                auto l = tp->get<Integer>(1); auto lv = l->get();
                auto r = tp->get<Integer>(2); auto rv = r->get();
                if (lv > rv) { l->set(rv); r->set(lv); }
            }), 50,
            mk::tuple(character('R'), integer(1, n), integer(1, n))->after_generate([](shared_ptr<Tuple> tp)->void {
                auto l = tp->get<Integer>(1); auto lv = l->get();
                auto r = tp->get<Integer>(2); auto rv = r->get();
                if (lv > rv) { l->set(rv); r->set(lv); } 
            }), 50
        )
    );
    origin->generate(0, nullptr);
    arr->generate(0, nullptr);
    vector<int> check; check.resize(n);
    auto splay = make_shared<Splay<XorSumAndMin, Rev>>();
    for (int i = 0; i < n; i++) {
        check[i] = origin->get<Integer>(i)->get();
        splay->insert_after(i, XorSumAndMin(check[i]));
    }
    for (int i = 0; i < n; i++) {
        char type = arr->get<Option>(i)->get<Tuple>()->get<Character>(0)->get();
        if (type == 'A') {
            int l = arr->get<Option>(i)->get<Tuple>()->get<Integer>(1)->get();
            int r = arr->get<Option>(i)->get<Tuple>()->get<Integer>(2)->get();
            int ans = 0, mn = inf;
            for (int j = l-1; j <= r-1; j++) {
                mn = min(mn, check[j]);
                ans ^= check[j];
            }
            auto q = splay->query_sum(l, r);
            ASSERT_EQ(q->mn, mn);
            ASSERT_EQ(q->sum, ans);
        } else {
            int l = arr->get<Option>(i)->get<Tuple>()->get<Integer>(1)->get();
            int r = arr->get<Option>(i)->get<Tuple>()->get<Integer>(2)->get();
            for (int j = l-1, k = r-1; j <= k; j++, k--) swap(check[j], check[k]); 
            splay->insert(l, r, Rev(true));
        }
    }
}

TEST(SplayTest, BasicComparable) {
    auto splay = make_shared<Splay<Int>>();
    auto v1 = splay->insert(Int(1));
    auto v5 = splay->insert(Int(5));
    auto v3 = splay->insert(Int(3));
    ASSERT_EQ(splay->rank(v1), 1);
    ASSERT_EQ(splay->rank(v5), 3);
    ASSERT_EQ(splay->rank(v3), 2);
    auto v4 = splay->insert(Int(4));
    auto v2 = splay->insert(Int(2));
    auto v33 = splay->insert(Int(3));
    ASSERT_EQ(splay->rank(v33), splay->rank(v3));
    ASSERT_EQ(splay->rank(v4), 5);
    ASSERT_EQ(splay->rank(v2), 2);
    ASSERT_EQ(splay->rank(v3), 3);
    ASSERT_EQ(v3->same(), 2);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}