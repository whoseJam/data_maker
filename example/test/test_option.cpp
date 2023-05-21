
#include <gtest.h>

#include "Array.h"
#include "Integer.h"
#include "Node.h"
#include "Tuple.h"
#include "Option.h"
#include "Character.h"

using namespace std;
using namespace mk;

TEST(OptionTest, BasicOption) {
    auto n = integer(10000);
    auto a_type = mk::tuple(
        character('a'),
        integer(1, n)
    );
    auto b_type = mk::tuple(
        character('b'),
        integer(10000, 20000),
        integer(1, 10)
    );
    auto arr = mk::array(n, 
        make_shared<Option>()
        ->add_option(a_type, 80)
        ->add_option(b_type, 20)
    )->format("$x\n");
    BUILD(arr);
    int cnt_a = 0;
    int cnt_b = 0;
    for (int i = 0; i < arr->length(); i++) {
        auto opt = arr->get<Option>(i)->get<Tuple>();
        char type = opt->get<Character>(0)->value();
        if (type == 'a') {
            int cur = opt->get<Integer>(1)->value();
            ASSERT_TRUE(0 <= cur && cur <= n->value());
            cnt_a++;
        }
        if (type == 'b') {
            int cur1 = opt->get<Integer>(1)->value();
            int cur2 = opt->get<Integer>(2)->value();
            ASSERT_TRUE(10000 <= cur1 && cur1 <= 20000);
            ASSERT_TRUE(1 <= cur2 && cur2 <= 10);
            cnt_b++;
        }
    }
    double rate = 1.0 * cnt_a / cnt_b;
    ASSERT_NEAR(rate, 4, 0.4);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}