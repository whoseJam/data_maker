
#include <assert.h>
#include <string.h>

#include "Random.h"
#include "SegmentTree.h"

using namespace Random;
using namespace std;
using namespace mk;

struct Sum : public Info, public Mergeable {
    int val;
    int sum;
    Sum(Zero) : val(0), sum(0) {}
    Sum(int v) : val(v), sum(v) {}
    void merge(shared_ptr<Mergeable> other) {
        auto other_ = dynamic_pointer_cast<Sum>(other);
        int sm = sum;
        sum += other_->sum;
    }
};

struct Atag : public Lazytag {
    int add;
    Atag(Zero) : add(0) {}
    Atag(int v) : add(v) {}
    void push(shared_ptr<Mergeable> other) {
        auto other_ = dynamic_pointer_cast<Sum>(other);
        other_->val += add;
        other_->sum += add * (SEGMENT_R - SEGMENT_L + 1);
    }
    void push(shared_ptr<Lazytag> other) {
        auto other_ = dynamic_pointer_cast<Atag>(other);
        other_->add += add;
    }
};

struct AMtag : public Lazytag {
    int k, b;
    AMtag(Zero) : k(1), b(0) {}
    AMtag(int k, int b) : k(k), b(b) {}
    void push(shared_ptr<Mergeable> other) {
        auto other_ = dynamic_pointer_cast<Sum>(other);
        other_->sum = (other_->sum * k) + b * (SEGMENT_R - SEGMENT_L + 1);
        other_->val = (other_->val * k) + b;
    }
    void push(shared_ptr<Lazytag> other) {
        auto other_ = dynamic_pointer_cast<AMtag>(other);
        other_->k = (other_->k * k);
        other_->b = (other_->b * k) + b;
    }
};

void weak_test_1() {
    auto seg = make_shared<SegmentTree<Sum>>(1, 10);
    for (int i = 1; i <= 10; i++)
        seg->insert(i, Sum(i));
    for (int i = 1; i <= 10; i++) {
        int right = (1 + i) * i / 2;
        assert(seg->query(1, i)->sum == right);
    }
    cout<<"weak test 1 passed\n";
}

void weak_test_2() {
    auto seg = make_shared<SegmentTree<Sum, Atag>>(1, 10);
    int sum[15]; memset(sum, 0, sizeof(sum));
    for (int T = 0; T <= 100; T++) {
        int l = rand_int(1, 10);
        int r = rand_int(1, 10);
        int d = rand_int(-100, 100);
        if (l > r) swap(l, r);
        seg->insert(l, r, Atag(d));
        for (int i = l; i <= r; i++) sum[i]+=d;
        l = rand_int(1, 10);
        r = rand_int(1, 10);
        if (l > r) swap(l, r); int ans = 0;
        for (int i = l; i <= r; i++) ans+=sum[i];
        assert(seg->query(l, r)->sum == ans);
    }
    cout << "weak test 2 passed\n";
}

void weak_test_3() {
    auto seg = make_shared<SegmentTree<Sum, AMtag>>(1, 10);
    int sum[15]; memset(sum, 0, sizeof(sum));
    for (int T = 0; T <= 100; T++) {
        int l = rand_int(1, 10);
        int r = rand_int(1, 10);
        int d = rand_int(-100, 100);
        int tp = rand_int(0, 100);
        if (l > r) swap(l, r);
        if (tp <= 50) {
            seg->insert(l, r, AMtag(1, d));
            for (int i = l; i <= r; i++) sum[i]+=d;
        } else {
            seg->insert(l, r, AMtag(d, 0));
            for (int i = l; i <= r; i++) sum[i]*=d;
        }

        l = rand_int(1, 10);
        r = rand_int(1, 10);
        if (l > r) swap(l, r); int ans = 0;
        for (int i = l; i <= r; i++) ans+=sum[i];
        assert(seg->query(l, r)->sum == ans);
    }
    cout << "weak test 3 passed\n";
}

int main() {
    weak_test_1();
    weak_test_2();
    weak_test_3();
    return 0;
}