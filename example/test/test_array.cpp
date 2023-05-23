
#include <iostream>
#include <memory>
#include <gtest.h>

#include "Dtstructure.h"
#include "HashMap.h"
#include "Random.h"
#include "Array.h"
#include "Integer.h"
#include "Node.h"
#include "Tuple.h"

using namespace std;
using namespace mk;

struct Val : public Hashable {
    Val(int x) : val(x) {}
    bool equal(shared_ptr<Hashable> other) override {
        auto o = dynamic_pointer_cast<Val>(other);
        if (!o) return false;
        return val == o->val;
    }
    uint hash_code() override {
        return val;
    }
    int val;
};

auto v(int x) -> shared_ptr<Hashable> {
    return make_shared<Val>(x);
}

TEST(ArrayTest, BasicArray) {
    for (int t = 1; t <= 10; t++) {
        auto l = integer(20, 40);
        auto n = integer(1, 10);
        auto arr = mk::array(l, n);
        BUILD(arr);
        ASSERT_TRUE(20 <= arr->length() && arr->length() <= 40);
        bool ele_is_same = true;
        for (int i = 0; i < arr->length(); i++) {
            int cur = arr->get<Integer>(i)->value();
            ASSERT_TRUE(1 <= cur && cur <= 10);
            ele_is_same &= (cur == arr->get<Integer>(0)->value());
        }
        ASSERT_FALSE(ele_is_same);
        CLEAR_ALL;
    }
    ASSERT_EQ(COUNT_ARRAY, 0);
    ASSERT_EQ(COUNT_INTEGER, 0);
}

TEST(ArrayTest, TwoDimensionArray) {
    int T = 10;
    for (int t = 1; t <= T; t++) {
        auto len = integer(3, 10);
        auto ele = integer(1, 50);
        auto row = mk::array(len, ele);
        auto rows = mk::array(len, row)->format("$x\n");
        len->live_with(rows);
        BUILD(rows);
        ASSERT_TRUE(3 <= row->length() && row->length() <= 10);
        ASSERT_TRUE(rows->length() == row->length());
        bool same_length = true;
        for (int i = 0; i < rows->length(); i++) {
            auto one_row = rows->get<Array>(i);
            same_length &= (one_row->length() == rows->get<Array>(0)->length());
            bool is_same = true;
            for (int j = 0; j < one_row->length(); j++) {
                int cur = one_row->get<Integer>(j)->value();
                ASSERT_TRUE(1 <= cur && cur <= 50);
                is_same &= (cur == one_row->get<Integer>(0)->value());
            }
            ASSERT_FALSE(is_same);
        }
        ASSERT_TRUE(same_length);
    }
    for (int t = 1; t <= T; t++) {
        auto len = integer(3, 10);
        auto ele = integer(1, 50);
        auto row = mk::array(len, ele);
        auto rows = mk::array(len, row)->format("$x\n");
        len->live_with(rows);
        ele->live_with(row);
        BUILD(rows);
        bool is_same_ = true;
        bool same_length = true;
        for (int i = 0; i < rows->length(); i++) {
            auto one_row = rows->get<Array>(i);
            is_same_ &= (one_row->get<Integer>(0)->value() == rows->get<Array>(0)->get<Integer>(0)->value());
            same_length &= (one_row->length() == rows->get<Array>(0)->length());
            bool is_same = true;
            for (int j = 0; j < one_row->length(); j++) {
                int cur = one_row->get<Integer>(j)->value();
                ASSERT_TRUE(1 <= cur && cur <= 50);
                is_same &= (cur == one_row->get<Integer>(0)->value());
            }
            ASSERT_TRUE(is_same);
        }
        ASSERT_TRUE(same_length);
        ASSERT_FALSE(is_same_);
    }
    for (int t = 1; t <= T; t++) {
        auto len = integer(3, 8);
        auto ele = integer(1, 50);
        auto row = mk::array(len, ele);
        auto rows = mk::array(10, row)->format("$x\n");
        ele->live_with(row);
        BUILD(rows);
        bool is_same_ = true;
        bool same_length = true;
        for (int i = 0; i < rows->length(); i++) {
            auto one_row = rows->get<Array>(i);
            is_same_ &= (one_row->get<Integer>(0)->value() == rows->get<Array>(0)->get<Integer>(0)->value());
            same_length &= (one_row->length() == rows->get<Array>(0)->length());
            bool is_same = true;
            for (int j = 0; j < one_row->length(); j++) {
                int cur = one_row->get<Integer>(j)->value();
                ASSERT_TRUE(1 <= cur && cur <= 50);
                is_same &= (cur == one_row->get<Integer>(0)->value());
            }
            ASSERT_TRUE(is_same);
        }
        ASSERT_FALSE(same_length);
        ASSERT_FALSE(is_same_);
    }
    for (int t = 1; t <= 1; t++) {
        auto len = integer(3, 8);
        auto ele = integer(1, 50);
        auto row = mk::array(len, ele);
        auto rows = mk::array(10, row)->format("$x\n");
        row->live_with(rows);
        BUILD(rows);
        bool col_is_same = true;
        bool same_length = true;
        for (int i = 0; i < rows->length(); i++) {
            auto one_row = rows->get<Array>(i);
            same_length &= (one_row->length() == rows->get<Array>(0)->length());
            bool is_same = true;
            for (int j = 0; j < one_row->length(); j++) {
                int cur = one_row->get<Integer>(j)->value();
                ASSERT_TRUE(1 <= cur && cur <= 50);
                col_is_same &= (cur == rows->get<Array>(0)->get<Integer>(j)->value());
            }
        }
        ASSERT_TRUE(same_length);
        ASSERT_TRUE(col_is_same);
    }
    CLEAR_ALL;
    ASSERT_EQ(COUNT_ARRAY, 0);
    ASSERT_EQ(COUNT_INTEGER, 0);
}

TEST(ArrayTest, ElementInAlessThanThatInB) {
    {
    using namespace Random;
    auto len = integer(5, 10);
    len->debug = true;
    auto a = mk::array(len, integer(1, 100));
    auto b = mk::array(len, integer(1, 100))->when_generating([&](shared_ptr<Array> b, int idx){
        auto b_ele = b->get<Integer>(idx);
        auto a_ele = track<Array>(a)->get<Integer>(idx);
        b_ele->value(rand_int(a_ele->value(), 100));
    });
    auto one_case = mk::tuple(a, b);
    auto multi_cases = mk::array(4, one_case);
    len->live_with(one_case);
    BUILD(multi_cases);
    auto diff_ab_length_between_multicases = make_shared<HashMap>();
    for (int i = 0; i < multi_cases->length(); i++) {
        auto case_ = multi_cases->get<Tuple>(i);
        auto a_arr = case_->get<Array>(0);
        auto b_arr = case_->get<Array>(1);
        auto ele_diff_in_array = make_shared<HashMap>();
        ASSERT_EQ(a_arr->length(), b_arr->length());
        for (int j = 0; j < a_arr->length(); j++) {
            auto a_ele = a_arr->get<Integer>(j);
            auto b_ele = b_arr->get<Integer>(j);
            ASSERT_TRUE(1 <= a_ele->value() && a_ele->value() <= b_ele->value() && b_ele->value() <= 100);
            ele_diff_in_array->insert(v(a_ele->value()));
        }
        ASSERT_TRUE(ele_diff_in_array->query_different() > 1);
        diff_ab_length_between_multicases->insert(v(a_arr->length()));
    }
    ASSERT_TRUE(diff_ab_length_between_multicases->query_different() > 1);
    }
    CLEAR_ALL;
    ASSERT_EQ(COUNT_ARRAY, 0);
    ASSERT_EQ(COUNT_INTEGER, 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}