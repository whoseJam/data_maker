#include "Trie.h"

#include <iostream>
#include <assert.h>

using namespace std;
using namespace mk;

struct MyInfo : public Info {
    MyInfo(const string& name, int ag) {this->name = name; age = ag; }
    string name;
    int age;
};

struct MergeInfo : public Info, public Mergeable {
public:
    int size;
    MergeInfo(int x) {size = x;}
    MergeInfo(Zero) {size = 0;}
    virtual void merge(shared_ptr<Mergeable> other) override {
        this->size += dynamic_pointer_cast<MergeInfo>(other)->size;
    }
};

#define check(myinfo_ptr, expect_name, expect_age) \
    assert(myinfo_ptr != nullptr); \
    assert(myinfo_ptr->name == expect_name); \
    assert(myinfo_ptr->age == expect_age);

void weak_test_1() {
    auto trie = make_shared<Trie<MyInfo>>();
    int v1 = trie->insert("hello", MyInfo("hsj", 18));
    int v2 = trie->insert("world", MyInfo("lk", 19));
    auto q1 = trie->query("hell");
    assert(q1 == nullptr);
    auto q2 = trie->query("hello");
    check(q2, "hsj", 18);
    int v3 = trie->insert("dash", MyInfo("djh", 17));
    auto q3 = trie->query("dash", v2);
    assert(q3 == nullptr);
    auto q4 = trie->query("dash", v3);
    check(q4, "djh", 17);
    auto q5 = trie->query("world");
    check(q5, "lk", 19);
    cout << "weak test 1 passed\n";
}

void weak_test_2() {
    auto trie = make_shared<Trie<MyInfo>>();
    int v1 = trie->insert("hello", MyInfo("aaa", 0));
    int v2 = trie->insert("hell", MyInfo("bbb", 1));
    int v3 = trie->insert("hellm", MyInfo("ccc", 2));
    
    auto q1 = trie->query("hello", v1);
    auto q2 = trie->query("hello", v2);
    auto q3 = trie->query("hell", v2);
    auto q4 = trie->query("hello", v3);
    auto q5 = trie->query("hell", v3);
    auto q6 = trie->query("hellm", v3);
    check(q1, "aaa", 0); check(q2, "aaa", 0); check(q3, "bbb", 1);
    check(q4, "aaa", 0); check(q5, "bbb", 1); check(q6, "ccc", 2);

    assert(trie->query("hell", v1) == nullptr);
    assert(trie->query("hellm", v1) == nullptr);
    assert(trie->query("hellm", v2) == nullptr);

    int v4 = trie->remove("hello", v1);
    int v5 = trie->remove("hell", v1);
    assert(trie->query("hello", v4) == nullptr);
    auto q7 = trie->query("hello", v5);
    check(q7, "aaa", 0);

    cout << "weak test 2 passed\n";
}

void weak_test_3() {
    #define C(mergeinfo_ptr, expect_size) \
        assert(mergeinfo_ptr != nullptr); \
        assert(mergeinfo_ptr->size == expect_size);
    auto trie = make_shared<Trie<MergeInfo>>();
    int v1 = trie->insert("hello", MergeInfo(3));
    int v2 = trie->insert("hella", MergeInfo(2));
    int v3 = trie->insert("hela", MergeInfo(100));
    int v4 = trie->insert("hell", MergeInfo(10));
    auto q1 = (trie->query("hello", v2));
    auto q2 = (trie->query("hell", v2));
    auto q3 = (trie->query("hella", v2));
    auto q4 = (trie->query("hel", v2));
    auto q5 = (trie->query("aaaa", v2));
    auto q6 = (trie->query("hela", v3));
    auto q7 = (trie->query("hell", v4));
    auto q8 = (trie->query("hel", v2));
    auto q9 = (trie->query("hel", v3));
    auto q10 = (trie->query("hel", v4));
    C(q1, 3); C(q2, 5); C(q3, 2); C(q4, 5);
    assert(q5 == nullptr);
    C(q6, 100); C(q7, 15); C(q8, 5); C(q9, 105); C(q10, 115);
    cout << "weak test 3 passed\n";
}

void weak_test_4() {
    #define C(mergeinfo_ptr, expect_size) \
        assert(mergeinfo_ptr != nullptr); \
        assert(mergeinfo_ptr->size == expect_size);
    auto trie = make_shared<Trie<MergeInfo>>();
    int v00 = trie->insert("hello", MergeInfo(3));
    int v0 = trie->insert("hella", MergeInfo(2));
    int v1 = trie->insert("hela", MergeInfo(100));
    int v2 = trie->insert("hell", MergeInfo(10));
    int v3 = trie->remove("hel");
    int v4 = trie->remove("hella");
    int v5 = trie->remove("hela");
    auto q1 = (trie->query("hel", v3));
    auto q2 = (trie->query("hel", v4));
    auto q3 = (trie->query("hel", v5));
    C(q1, 115); C(q2, 113); C(q3, 13);
    auto q4 = (trie->query("hel", v0));
    auto q5 = (trie->query("hel", v1));
    auto q6 = (trie->query("hel", v2));
    C(q4, 5); C(q5, 105); C(q6, 115);
    cout << "weak test 4 passed\n";
}

void weak_test_5() {
    auto trie = make_shared<Trie<MyInfo>>();
    int v1 = trie->insert("", MyInfo("empty value", 1));
    auto q1 = (trie->query(""));
    auto q2 = (trie->query("", 0));
    auto q3 = (trie->query("", v1));
    assert(q1 != nullptr); assert(q1->name == "empty value");
    assert(q2 == nullptr);
    assert(q3 != nullptr); assert(q3->name == "empty value");
    cout << "weak test 5 passed\n";
}

void weak_test_6() {
    auto trie = make_shared<Trie<MyInfo>>();
    int v1 = trie->insert("111", MyInfo("hsj", 1));
    int v2 = trie->insert("11", MyInfo("hsjj", 2));
    auto q1 = (trie->query("111", v2));
    auto q2 = (trie->query("11", v2));
    assert(q1 != nullptr); assert(q1->name == "hsj");
    assert(q2 != nullptr); assert(q2->name == "hsjj");
    cout << "weak test 6 passed\n";
}

struct TTag : public Lazytag {
    int add;
    TTag(int a) : add(a) {}
    TTag(Zero) : add(0) {}
    void push(shared_ptr<Lazytag> tag);
    void push(shared_ptr<Mergeable> info);
};
struct TInfo : public Info, public Mergeable {
    int size;
    int sum;
    TInfo(Zero) : sum(0), size(0) {}
    TInfo(int val) : sum(val), size(1) {}
    void merge(shared_ptr<Mergeable> other) {
        auto other_ = dynamic_pointer_cast<TInfo>(other);
        size += other_->size; sum += other_->sum;
    }
};
void TTag::push(shared_ptr<Lazytag> tag) {
    auto tag_ = dynamic_pointer_cast<TTag>(tag);
    tag_->add += add;
}
void TTag::push(shared_ptr<Mergeable> info) {
    auto info_ = dynamic_pointer_cast<TInfo>(info);
    info_->sum += info_->size * add;
}

void weak_test_7() {
    auto trie = make_shared<Trie<TInfo, TTag>>();
    int v1 = trie->insert("aaa", TInfo(4));
    int v2 = trie->insert("aab", TInfo(6));
    assert(trie->query("aa", v1)->sum == 4);
    assert(trie->query("aa", v2)->sum == 10);
    int v3 = trie->insert("aac", TTag(5));
    assert(trie->query("aa", v3)->sum == 10);
    assert(trie->query("aa", v3)->size == 2);
    int v4 = trie->insert("aa", TTag(100));
    assert(trie->query("aaa", v4)->sum == 104);
    assert(trie->query("aa", v4)->sum == 210);
    assert(trie->query("aac")->sum == 0);
    cout << "weak test 7 passed\n";
}

int main() {
    weak_test_1();
    weak_test_2();
    weak_test_3();
    weak_test_4();
    weak_test_5();
    weak_test_6();
    weak_test_7();
    return 0;
}