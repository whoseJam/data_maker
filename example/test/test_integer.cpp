
#include <gtest.h>
#include <iostream>

#include "Integer.h"
#include "Node.h"

using namespace std;
using namespace mk;

TEST(IntegerTest, NestedInteger) {
    for (int i = 1; i <= 10; i++) {
        auto a = integer(1, 10)->format("$x hello\n");
        auto b = integer(integer(1, 100), 100);
        auto c = integer(integer(1, 10), integer(20, 30));
        BUILD(a); BUILD(b); BUILD(c);
        ASSERT_TRUE(a->lower_bound()->value() == 1);
        ASSERT_TRUE(1 <= a->value() && a->value() <= 10);
        auto bl = b->lower_bound();
        ASSERT_TRUE(1 <= bl->value() && bl->value() <= 100);
        ASSERT_TRUE(bl->value() <= b->value() && b->value() <= 100);
        auto cl = c->lower_bound();
        auto cr = c->upper_bound();
        ASSERT_TRUE(1 <= cl->value() && cl->value() <= 10);
        ASSERT_TRUE(20 <= cr->value() && cr->value() <= 30);
        ASSERT_TRUE(cl->value() <= c->value() && c->value() <= cr->value());
    }
}

TEST(IntegerTest, BasicOperatorTest) {
    for (int i = 1; i <= 10; i++) {
        auto a = integer(1, 100) - integer(1, 100);
        auto b = integer(1, 10) + integer(1, 10) * integer(1, 10);
        auto c = (integer(1, 10) + integer(1, 10)) * integer(10);
        BUILD(a); BUILD(b); BUILD(c);
        ASSERT_TRUE(-99 <= a->value() && a->value() <= 99);
        ASSERT_TRUE(2 <= b->value() && b->value() <= 110);
        ASSERT_TRUE(20 <= c->value() && c->value() <= 200);
        auto A = integer(1, 10);
        auto B = integer(1, 10);
        auto AplusB = A + B;
        auto AminusB = A - B;
        auto ans1 = AplusB * AminusB;
        auto ans2 = A * A - B * B;
        BUILD(ans1); BUILD(ans2);
        ASSERT_TRUE(ans1->value() == ans2->value());
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}