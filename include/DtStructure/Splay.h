#ifndef SPLAY_H
#define SPLAY_H

#include <memory>
#include <type_traits>
#include <utility>
#include <functional>

#include "Debug.h"
#include "Logger.h"
#include "Dtstructure.h"

namespace mk {

extern int COUNT_SPLAY;
extern int COUNT_SPLAYNODE;

class SplayHandle : public Handle {
public:
    SplayHandle() = default;
    virtual ~SplayHandle() = default;
    virtual auto size() -> int = 0;
    virtual auto diff() -> int = 0;
    virtual auto same() -> int = 0;
    virtual auto swap_child() -> void = 0;
};
template<typename T, typename L>
struct Splay;

namespace Hidden {

template<typename T, typename L>
struct SplayNode : 
    public SplayHandle {
    SplayNode* ch_[2];
    SplayNode* fa_;
    T info_;
    T sum_;
    L tag_;
    int same_;
    int size_;
    int diff_;
    SplayNode() : same_(1), size_(1), diff_(1) { 
        COUNT_SPLAYNODE++;
        fa_ = ch_[0] = ch_[1] = nullptr; 
    }
    SplayNode(const T& info) : same_(1), size_(1), diff_(1) { 
        COUNT_SPLAYNODE++; 
        info_ = info;
        fa_ = ch_[0] = ch_[1] = nullptr;
    }
    SplayNode(const SplayNode& other) {
        COUNT_SPLAYNODE++;
        info_ = other.info_;
        sum_ = other.sum_;
        tag_ = other.tag_;
        same_ = other.same_;
        size_ = other.size_;
        diff_ = other.diff_;
        fa_ = nullptr;
        if (other.ch_[0]) ch_[0] = new SplayNode(*other.ch_[0]);
        if (other.ch_[1]) ch_[1] = new SplayNode(*other.ch_[1]);
    }
    SplayNode(SplayNode&& other) {
        COUNT_SPLAYNODE++;
        info_ = std::move(other.info_);
        sum_ = std::move(other.sum_);
        tag_ = std::move(other.tag_);
        same_ = other.same_;
        size_ = other.size_;
        diff_ = other.diff_;
        fa_ = other.fa_; other.fa_ = nullptr;
        ch_[0] = other.ch_[0]; other.ch_[0] = nullptr;
        ch_[1] = other.ch_[1]; other.ch_[1] = nullptr;
    }
    SplayNode& operator =(const SplayNode& other) {
        if (&other == this) return *this;
        COUNT_SPLAYNODE++;
        info_ = other.info_;
        sum_ = other.sum_;
        tag_ = other.tag_;
        same_ = other.same_;
        size_ = other.size_;
        diff_ = other.diff_;
        fa_ = nullptr;
        if (other.ch_[0]) ch_[0] = new SplayNode(*other.ch_[0]);
        if (other.ch_[1]) ch_[1] = new SplayNode(*other.ch_[1]);
        return *this;
    }
    SplayNode& operator =(SplayNode&& other) {
        if (&other == this) return *this;
        COUNT_SPLAYNODE++;
        info_ = std::move(other.info_);
        sum_ = std::move(other.sum_);
        tag_ = std::move(other.tag_);
        same_ = other.same_;
        size_ = other.size_;
        diff_ = other.diff_;
        fa_ = other.fa_; other.fa_ = nullptr;
        ch_[0] = other.ch_[0]; other.ch_[0] = nullptr;
        ch_[1] = other.ch_[1]; other.ch_[1] = nullptr;
        return *this;
    }
    ~SplayNode() {
        COUNT_SPLAYNODE--;
        if (ch_[0]) delete ch_[0];
        if (ch_[1]) delete ch_[1];
    }
    auto clear() -> void {
        if (ch_[0]) ch_[0]->clear();
        if (ch_[1]) ch_[1]->clear();
        if (ch_[0]) { delete ch_[0]; ch_[0] = nullptr; }
        if (ch_[1]) { delete ch_[1]; ch_[1] = nullptr; }
        info_ = T(); sum_ = T(); tag_ = L();
        same_ = size_ = diff_ = 0;
    }
    auto query_first_info() -> T {
        auto cur = this;
        while (cur->fa_) cur = cur->fa_;
        while (cur->ch_[0]) cur = cur->ch_[0];
        return cur->info_;
    }
    auto query_last_info() -> T {
        auto cur = this;
        while (cur->fa_) cur = cur->fa_;
        while (cur->ch_[1]) cur = cur->ch_[1];
        return cur->info_;
    }
    auto same() -> int override { return same_; }
    auto size() -> int override { return size_; }
    auto diff() -> int override { return diff_; }
    auto swap_child() -> void override { std::swap(ch_[0], ch_[1]); }
    auto child(int x) -> SplayNode*& { return ch_[x]; }
    auto insert_child(SplayNode* child, int type) -> void {
        CALL(FUNCTION);
        if (ch_[type]) MESSAGE(CLASS, UNKNOWN);
        ch_[type] = child;
        child->fa_ = this;
    }
    auto remove_child(int type) -> void {
        CALL(FUNCTION);
        if (ch_[type]) {
            ch_[type]->fa_ = nullptr;
            delete ch_[type];
            ch_[type] = nullptr;
        }
    }
    auto remove_and_take_out_child(int type) -> SplayNode* {
        CALL(FUNCTION);
        auto ans = ch_[type];
        ans->fa_ = nullptr;
        ch_[type] = nullptr;
        return ans;
    }

    template<typename U, bool CHECKER = std::is_base_of_v<Mergeable, U>>
    struct push_up_helper;
    template<typename U>
    struct push_up_helper<U, true> {
        void call(SplayNode* node) {
            CALL(FUNCTION);
            node->sum_ = T();
            if (node->ch_[0])
                node->sum_.merge(&(node->ch_[0]->sum_));
            node->sum_.merge(&(node->info_));
            if (node->ch_[1])
                node->sum_.merge(&(node->ch_[1]->sum_));
        }
    };
    template<typename U>
    struct push_up_helper<U, false> { void call(SplayNode* node) {} };
    auto push_up() -> void {
        CALL(FUNCTION);
        push_up_helper<T>().call(this);
        size_ = same_; diff_ = 1;
        if (ch_[0]) { size_ += ch_[0]->size_; diff_ += ch_[0]->diff_; }
        if (ch_[1]) { size_ += ch_[1]->size_; diff_ += ch_[1]->diff_; }
    }
    template<typename U, bool CHECKER = std::is_base_of_v<Lazytag, U>>
    struct push_down_helper;
    template<typename U>
    struct push_down_helper<U, true> {
        auto call(SplayNode* node) -> void {
            CALL(FUNCTION);
            if (node->ch_[0]) node->ch_[0]->push_tag(node->tag_);
            if (node->ch_[1]) node->ch_[1]->push_tag(node->tag_);
            node->tag_ = L();
        }
    };
    template<typename U>
    struct push_down_helper<U, false> { auto call(SplayNode* node) -> void {} };
    template<typename U, typename R, bool CHECKER = std::is_base_of_v<Pushable, U> && std::is_base_of_v<Lazytag, R>>
    struct push_to_info_helper;
    template<typename U, typename R>
    struct push_to_info_helper<U, R, true> {
        auto call(SplayNode* node, const L& tag) -> void {
            CALL(FUNCTION);
            L& non_const_tag = const_cast<L&>(tag);
            node->info_.push(&non_const_tag, node);
            node->sum_.push(&non_const_tag, node);
        }
    };
    template<typename U, typename R>
    struct push_to_info_helper<U, R, false> { auto call(SplayNode*, const L&) -> void {} };
    template<typename U, bool CHECKER = std::is_base_of_v<Lazytag, U>>
    struct push_to_tag_helper;
    template<typename U>
    struct push_to_tag_helper<U, true> {
        auto call(SplayNode* node, const L& tag) -> void {
            CALL(FUNCTION);
            L& non_const_tag = const_cast<L&>(tag);
            node->tag_.push(&non_const_tag, node);
        }
    };
    template<typename U>
    struct push_to_tag_helper<U, false> { auto call(SplayNode* node, const L& tag) -> void {} };

    auto push_down() -> void {
        CALL(FUNCTION);
        push_down_helper<L>().call(this);
    }
    auto push_tag(const L& tag) {
        CALL(FUNCTION);
        push_to_info_helper<T, L>().call(this, tag);
        push_to_tag_helper<L>().call(this, tag);
    }
};

}   // HIDEN END
extern bool gdebug;
template<typename T, typename L = Empty>
class Splay {
public:
    using SplayNode = Hidden::SplayNode<T, L>;
    
    Splay() : root_(nullptr), cnt(0) { COUNT_SPLAY++; }
    Splay(const Splay& other) {
        COUNT_SPLAY++;
        root_ = new SplayNode(*other.root_);
    }
    Splay(Splay&& other) {
        COUNT_SPLAY++;
        root_ = other.root_;
        other.root_ = nullptr;
    }
    ~Splay() {
        COUNT_SPLAY--;
        //if (root_) delete root_;
    }
    auto insert(const T& info) -> SplayHandle*;
    auto insert(int l, int r, const L& tag) -> void;
    auto insert(int k, const T& info) -> SplayHandle*;
    auto insert(SplayHandle* l, SplayHandle* r, const L& tag) -> void;
    auto insert_after(int k, const T& info) -> SplayHandle*;
    auto insert_after(int k, Splay<T, L>&& tr) -> SplayHandle*;
    auto insert_before(int k, const T& info) -> SplayHandle*;
    auto insert_before(int k, Splay<T, L>&& tr) -> SplayHandle*;
    auto remove(int k) -> void;
    auto remove(const T& info) -> void;
    auto remove(SplayHandle* x) -> void;
    auto split(int l, int r) -> Splay<T, L>;
    auto split(SplayHandle* l, SplayHandle* r) -> Splay<T, L>;
    auto query_info(int k) -> T;
    auto query_info(SplayHandle*) -> T;
    auto query_info(const T& info) -> T;
    auto query_sum(int k) -> T;
    auto query_sum(int l, int r) -> T;
    auto rank(SplayHandle* x) -> int;
    auto size() -> int;
    auto clear() -> void;
private:
    Splay(SplayNode* node) { COUNT_SPLAY++; root_ = node; }
    auto rotate(SplayNode* x, SplayNode*& rt) -> void;
    auto splay(SplayNode* x, SplayNode*& rt) -> void;
    auto insert(SplayNode*& cur, SplayNode* fa, const T& info) -> SplayHandle*;
    auto query(SplayNode* cur, const T& info) -> SplayNode*;
    auto find_kth_min(SplayNode* cur, int k) -> SplayNode*;
    auto find_kth_max(SplayNode* cur, int k) -> SplayNode*;
    auto find_root_prev() -> SplayNode*;
    auto find_root_next() -> SplayNode*;
    auto my_malloc(const T& info) -> SplayNode* {
        pool[cnt] = SplayNode(info);
        return &pool[cnt++];
    }
    SplayNode* root_;
    std::array<SplayNode, 10240> pool;
    int cnt;
};

/*
Description:
清空Splay.
*/
template<typename T, typename L>
auto Splay<T, L>::clear() -> void {
    if (root_) {
        // delete root_;
        root_ = nullptr;
    }
}

/*
Description:
获取Splay的大小.
*/
template<typename T, typename L>
auto Splay<T, L>::size() -> int {
    CALL(FUNCTION);
    if (root_) return root_->size_;
    return 0;
}

template<typename T, typename L>
auto Splay<T, L>::rotate(SplayNode* x, SplayNode*& rt) -> void {
    CALL(FUNCTION);
    auto y = x->fa_;
    auto z = y->fa_;
    int l = (y->ch_[0]) != x, r = l^1;
    if (y == rt) rt = x;
    else if (z->ch_[0] == y) z->ch_[0] = x;
    else z->ch_[1] = x;
    x->fa_ = z; y->fa_ = x;
    if (x->ch_[r]) x->ch_[r]->fa_ = y;
    y->ch_[l] = x->ch_[r]; x->ch_[r] = y;
    y->push_up(); x->push_up();
}

template<typename T, typename L>
auto Splay<T, L>::splay(SplayNode* x, SplayNode*& rt) -> void {
    CALL(FUNCTION);
    auto ck = x;
    while (ck != rt && ck->fa_) ck = ck->fa_;
    if (ck != rt) MESSAGE("Splay<T, L>", UNKNOWN);
    while (x != rt) {
        auto y = x->fa_;
        auto z = y->fa_;
        if (z) z->push_down();
        y->push_down();
        x->push_down();
        if (y != rt) {
            if ((z->ch_[0] == y) ^ (y->ch_[0] == x)) rotate(x, rt);
            else rotate(y, rt);
        }
        rotate(x, rt);
    }
}

template<typename T, typename L>
auto Splay<T, L>::insert(SplayNode*& cur, SplayNode* fa, const T& info) -> SplayHandle* {
    CALL(FUNCTION);
    if (!cur) { cur = new SplayNode(info); cur->fa_ = fa; return cur; }
    cur->push_down();
    T& non_const_info = const_cast<T&>(info);
    auto cmp_ans = cur->info_.compare_to(&non_const_info);
    SplayHandle* ret;
    if (cmp_ans < 0) ret = insert(cur->ch_[1], cur, info);
    else if (cmp_ans == 0) { ret = cur; cur->same_++; }
    else ret = insert(cur->ch_[0], cur, info);
    cur->push_up();
    return ret;
}

/*
Description:
插入一个Comparable的元素到Splay中.
*/
template<typename T, typename L>
auto Splay<T, L>::insert(const T& info) -> SplayHandle* {
    CALL(FUNCTION);
    return insert(root_, nullptr, info);
}

/*
Description:
对Splay的[l,r]区间打上标记tag.
*/
template<typename T, typename L>
auto Splay<T, L>::insert(int l, int r, const L& tag) -> void {
    CALL(FUNCTION);
    if (l > r) MESSAGE("Splay<T, L>", ENSURE("l <= r"));
    if (l < 1 || l > size()) MESSAGE("Splay<T, L>", ENSURE("1 <= l <= size"));
    if (r < 1 || r > size()) MESSAGE("Splay<T, L>", ENSURE("1 <= r <= size"));
    auto prev = find_kth_min(root_, l - 1);
    auto next = find_kth_min(root_, r + 1);
    if (!prev && !next) {
        if (root_) root_->push_tag(tag);
        return;
    } else if (!prev) {
        splay(next, root_);
        root_->ch_[0]->push_tag(tag);
        root_->push_up();
        return;
    } else if (!next) {
        splay(prev, root_);
        root_->ch_[1]->push_tag(tag);
        root_->push_up();
        return;
    }
    splay(prev, root_); splay(next, root_->ch_[1]);
    root_->ch_[1]->ch_[0]->push_tag(tag);
    root_->ch_[1]->push_up();
    root_->push_up();
}

/*
Description:
对Splay的[l,r]区间打上标记tag.
l和r以SplayHandle的形式给出.
*/
template<typename T, typename L>
auto Splay<T, L>::insert(SplayHandle* l, SplayHandle* r, const L& tag) -> void {
    CALL(FUNCTION);
    auto lq = dynamic_cast<SplayNode*>(l);
    auto rq = dynamic_cast<SplayNode*>(r);
    int rank_l = rank(lq);
    int rank_r = rank(rq);
    if (rank_l > rank_r) MESSAGE("Splay<T, L>", ENSURE("l <= r"));
    splay(lq, root_);
    auto prev = find_root_prev();
    splay(rq, root_);
    auto next = find_root_next();
    if (!prev && !next) { if(root_) root_->push_tag(tag); return;}
    else if (!prev) { splay(next, root_); root_->ch_[0]->push_tag(tag); root_->push_up(); return; }
    else if (!next) { splay(prev, root_); root_->ch_[1]->push_tag(tag); root_->push_up(); return; }
    splay(prev, root_); splay(next, root_->ch_[1]);
    root_->ch_[1]->ch_[0]->push_tag(tag);
    root_->ch_[1]->push_up();
    root_->push_up();
}

/*
Description:
修改Splay的第k个元素的值为info.
*/
template<typename T, typename L>
auto Splay<T, L>::insert(int k, const T& info) -> SplayHandle* {
    CALL(FUNCTION);
    if (k < 1 || k > size()) MESSAGE("Splay<T, L>", ENSURE("1 <= k <= size"));
    auto target = find_kth_min(root_, k);
    splay(target, root_);
    root_->info_ = info;
    root_->push_up();
    return root_;
}

/*
Description:
在Splay的第k个元素之后插入一个值为info的元素.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_after(int k, const T& info) -> SplayHandle* {
    CALL(FUNCTION);
    if (k < 0 || k > size()) MESSAGE("Splay<T, L>", ENSURE("0 <= k <= size"));
    auto prev = find_kth_min(root_, k);
    auto next = find_kth_min(root_, k + 1);
    if (!prev && !next) {
        root_ = my_malloc(info);
        root_->push_up();
        return root_;
    } else if (!prev) {
        splay(next, root_);
        root_->insert_child(my_malloc(info), 0); 
        root_->ch_[0]->push_up();
        root_->push_up();
        return root_->child(0); 
    } else if (!next) {
        splay(prev, root_);
        root_->insert_child(my_malloc(info), 1);
        root_->ch_[1]->push_up(); 
        root_->push_up();
        return root_->ch_[1]; 
    }
    splay(prev, root_); splay(next, root_->child(1));
    root_->child(1)->insert_child(my_malloc(info), 0);
    root_->child(1)->child(0)->push_up();
    root_->child(1)->push_up();
    root_->push_up();
    return root_->child(1)->child(0);
}

/*
Description:
在Splay的第k个元素后插入另一个Splay.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_after(int k, Splay<T, L>&& tr) -> SplayHandle* {
    CALL(FUNCTION);
    if (k < 0 || k > size()) MESSAGE("Splay<T, L>", ENSURE("0 <= k <= size"));
    auto prev = find_kth_min(root_, k);
    auto next = find_kth_min(root_, k + 1);
    if (!prev && !next) {
        root_ = tr.root_;
        tr.root_ = nullptr;
        root_->push_up(); 
        return root_; 
    } else if (!prev) { 
        splay(next, root_); 
        root_->insert_child(tr.root_ , 0);
        tr.root_ = nullptr; 
        root_->ch_[0]->push_up(); 
        root_->push_up(); 
        return root_->ch_[0]; 
    } else if (!next) { 
        splay(prev, root_); 
        root_->insert_child(tr.root_ , 1);
        tr.root_ = nullptr; 
        root_->ch_[1]->push_up(); 
        root_->push_up(); 
        return root_->ch_[1];
    }
    splay(prev, root_); splay(next, root_->ch_[1]);
    root_->ch_[1]->insert_child(tr.root_, 0);
    tr.root_ = nullptr;
    root_->ch_[1]->ch_[0]->push_up();
    root_->ch_[1]->push_up();
    root_->push_up();
    return root_->ch_[1]->ch_[0];
} 

/*
Description:
在Splay的第k个元素之前插入一个值为info的元素.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_before(int k, const T& info) -> SplayHandle* {
    CALL(FUNCTION);
    if (k < 1 || k > size() + 1) MESSAGE("Splay<T, L>", ENSURE("1 <= k <= size + 1"));
    auto prev = find_kth_min(root_, k - 1);
    auto next = find_kth_min(root_, k);
    if (!prev && !next) { 
        root_ = new SplayNode(info); 
        root_->push_up(); 
        return root_; 
    } else if (!prev) { 
        splay(next, root_); 
        root_->insert_child(new SplayNode(info), 0); 
        root_->ch_[0]->push_up(); 
        root_->push_up(); 
        return root_->ch_[0]; 
    } else if (!next) { 
        splay(prev, root_); 
        root_->insert_child(new SplayNode(info), 1); 
        root_->ch_[1]->push_up();
        root_->push_up();
        return root_->ch_[1];
    }
    splay(prev, root_); splay(next, root_->ch_[1]);
    root_->ch_[1]->insert_child(new SplayNode(info), 0);
    root_->ch_[1]->ch_[0]->push_up();
    root_->ch_[1]->push_up();
    root_->push_up();
    return root_->child(1)->child(0);
}

/*
Description:
在Splay的第k个元素前插入另一个同类型的Splay.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_before(int k, Splay<T, L>&& tr) -> SplayHandle* {
    CALL(FUNCTION);
    if (k < 0 || k > size()) MESSAGE("Splay<T, L>", ENSURE("1 <= k <= size + 1"));
    auto prev = find_kth_min(root_, k - 1);
    auto next = find_kth_min(root_, k);
    if (!prev && !next) {
        root_ = tr.root_;
        tr.root_ = nullptr;
        root_->splay = this;
        root_->push_up();
        return root_;
    }
    else if (!prev) {
        splay(next, root_);
        root_->insert_child(tr.root_, 0);
        tr.root_ = nullptr;
        root_->ch[0]->push_up();
        root_->push_up();
        return root_->ch[0];
    }
    else if (!next) {
        splay(prev, root_);
        root_->insert_child(tr.root_, 1);
        tr.root_ = nullptr;
        root_->ch[1]->push_up();
        root_->push_up();
        return root_->ch[1];
    }
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->insert_child(tr.root_, 0);
    tr.root_ = nullptr;
    root_->ch[1]->ch[0]->push_up();
    root_->ch[1]->push_up();
    root_->push_up();
    return root_->ch[1]->ch[0];
} 

/*
Description:
删除Splay中第k个SplayNode，即使该SplayNode中保存了多个元素.
*/
template<typename T, typename L>
auto Splay<T, L>::remove(int k) -> void {
    CALL(FUNCTION);
    auto target = find_kth_min(root_, k);
    if (!target) MESSAGE("Splay<T, L>", ENSURE("1 <= k <= size"));
    splay(target, root_);
    auto prev = find_root_prev();
    auto next = find_root_next();
    if (!prev && !next) root_ = nullptr;
    else if (!prev) { splay(next, root_); root_->remove_child(0); root_->push_up(); return; }
    else if (!next) { splay(prev, root_); root_->remove_child(1); root_->push_up(); return; }
    splay(prev, root_); splay(next, root_->ch_[1]);
    root_->ch_[1]->remove_child(0);
    root_->ch_[1]->push_up();
    root_->push_up();
}

/*
Description:
从Splay中删除一个值为info的元素.
如果有多个值为info的元素被存储在同一个SplayNode中，该SplayNode不会被删除.
*/
template<typename T, typename L>
auto Splay<T, L>::remove(const T& info) -> void {
    CALL(FUNCTION);
    auto target = query(root_, info);
    splay(target, root_);
    auto prev = find_root_prev();
    auto next = find_root_next();
    root_->same_--; root_->push_up();
    if (root_->same_ > 0) return;
    if (!prev && !next) { root_ = nullptr; return; }
    else if (!prev) { splay(next, root_); root_->remove_child(0); root_->push_up(); return; }
    else if (!next) { splay(prev, root_); root_->remove_child(1); root_->push_up(); return; }
    splay(prev, root_); splay(next, root_->ch_[1]);
    root_->ch_[1]->remove_child(0);
    root_->ch_[1]->push_up();
    root_->push_up();
}

/*
Description:
删除由x管理的SplayNode，即使该SplayNode中保存了多个元素.
*/
template<typename T, typename L>
auto Splay<T, L>::remove(SplayHandle* x) -> void {
    CALL(FUNCTION);
    auto target = dynamic_cast<SplayNode>(x);
    if (target->belong() != this) MESSAGE("Splay<T, L>", SPLAY_HANDLE_NOT_FOUND);
    splay(target, root_);
    auto prev = find_root_prev();
    auto next = find_root_next();
    if (!prev && !next) { root_ = nullptr; return; }
    else if (!prev) { splay(next, root_); root_->remove_child(0); root_->push_up(); return; }
    else if (!next) { splay(prev, root_); root_->remove_child(1); root_->push_up(); return; }
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->remove_child(0);
    root_->ch[1]->push_up();
    root_->push_up();
}

template<typename T, typename L>
auto Splay<T, L>::split(int l, int r) -> Splay<T, L> {
    CALL(FUNCTION);
    if (l > r) MESSAGE("Splay<T, L>", ENSURE("l <= r"));
    auto lq = find_kth_min(root_, l);
    if (!lq) MESSAGE("Splay<T, L>", ENSURE("1 <= l <= size"));
    auto rq = find_kth_min(root_, r);
    if (!rq) MESSAGE("Splay<T, L>", ENSURE("1 <= r <= size"));
    splay(lq, root_);
    auto prev = find_root_prev();
    splay(rq, root_);
    auto next = find_root_next();
    if (!prev && !next) { 
        auto rt = root_;
        root_ = nullptr;
        return Splay<T, L>(rt);
    } else if (!prev) {
        splay(next, root_);
        auto rt = root_->remove_and_take_out_child(0);
        root_->push_up();
        return Splay<T, L>(rt);
    } else if (!next) {
        splay(prev, root_);
        auto rt = root_->remove_and_take_out_child(1);
        root_->push_up();
        return Splay<T, L>(rt);
    }
    splay(prev, root_); splay(next, root_->ch_[1]);
    auto rt = root_->ch_[1]->remove_and_take_out_child(0);
    root_->ch_[1]->push_up();
    root_->push_up();
    return Splay<T, L>(rt);
}

/*
Description:
查询Splay中[k,k]这段元素的sum.
需要保证T是Mergeable的.
*/
template<typename T, typename L>
auto Splay<T, L>::query_sum(int k) -> T {
    CALL(FUNCTION);
    return query_sum(k, k);
}

/*
Description:
查询Splay中[l,r]这段区间的sum.
需要保证T是Mergeable的.
*/
template<typename T, typename L>
auto Splay<T, L>::query_sum(int l, int r) -> T {
    CALL(FUNCTION);
    if (!std::is_base_of_v<Mergeable, T>) MESSAGE("Splay<T, L>", UNABLE("Mergeable"));
    if (l > r) MESSAGE("Splay<T, L>", ENSURE("l <= r"));
    if (l < 1 || l > size()) MESSAGE("Splay<T, L>", ENSURE("1 <= l <= size"));
    if (r < 1 || r > size()) MESSAGE("Splay<T, L>", ENSURE("1 <= r <= size"));
    auto prev = find_kth_min(root_, l - 1);
    auto next = find_kth_min(root_, r + 1);
    if (!prev && !next) return root_->sum_;
    else if (!prev) { 
        splay(next, root_);
        return root_->ch_[0]->sum_; 
    }
    else if (!next) {
        splay(prev, root_);
        return root_->ch_[1]->sum_;
    }
    splay(prev, root_);
    splay(next, root_->ch_[1]);
    return root_->ch_[1]->ch_[0]->sum_;
}

template<typename T, typename L>
auto Splay<T, L>::query(SplayNode* cur, const T& info) -> SplayNode* {
    if (!cur) return nullptr;
    auto cur_info = std::dynamic_pointer_cast<Comparable>(cur->info_);
    auto new_info = std::dynamic_pointer_cast<Comparable>(info);
    if (!cur_info || !new_info) MESSAGE("Splay<T, L>", UNABLE("Comparable"));
    cur->push_down();
    auto cmp_ans = cur_info->compare_to(new_info);
    if (cmp_ans < 0) return query(cur->ch_[1], info);
    else if (cmp_ans == 0) return cur;
    else return query(cur->ch_[0], info);
}

/*
Description:
查询等于T的元素的info.
需要保证T是Comparable的.
*/
template<typename T, typename L>
auto Splay<T, L>::query_info(const T& info) -> T {
    CALL(FUNCTION);
    auto target = query(root_, info);
    if (!target) MESSAGE("Splay<T, L>", UNKNOWN);
    return target->info_;
}

/*
Description:
查询x所管理的SplayNode的info
*/
template<typename T, typename L>
auto Splay<T, L>::query_info(SplayHandle* x) -> T {
    CALL(FUNCTION);
    auto target = dynamic_cast<SplayNode*>(x);
    splay(target, root_);
    return target->info_;
}

/*
Description:
查询第k个元素的info.
*/
template<typename T, typename L>
auto Splay<T, L>::query_info(int k) -> T {
    CALL(FUNCTION);
    auto target = find_kth_min(root_, k);
    if (!target) MESSAGE("Splay<T, L>", ENSURE("1 <= l <= size"));
    splay(target, root_);
    return target->info_;
}

/*
Description:
查询Splay中从小到大第k个元素.
*/
extern int DEBUGCNT;
template<typename T, typename L>
auto Splay<T, L>::find_kth_min(SplayNode* cur, int k) -> SplayNode* {
    CALL(FUNCTION);
    if (!cur) return nullptr;
    cur->push_down();
    DEBUGCNT++;
    int left = (cur->ch_[0]) ? (cur->ch_[0]->size_) : 0;
    if (left + 1 <= k && k <= left + (cur->same_)) return cur;
    if (left >= k) return find_kth_min(cur->ch_[0], k);
    return find_kth_min(cur->ch_[1], k - left - (cur->same_));
}

/*
Description:
查询Splay中从大到小第k个元素.
*/
template<typename T, typename L>
auto Splay<T, L>::find_kth_max(SplayNode* cur, int k) -> SplayNode* {
    CALL(FUNCTION);
    if (!cur) return nullptr;
    cur->push_down();
    int right = (cur->ch_[1]) ? (cur->ch_[1]->size_) : 0;
    if (right + 1 <= k && k <= right + (cur->same_)) return cur;
    if (right >= k) return find_kth_max(cur->ch_[1], k);
    return find_kth_max(cur->ch_[0], k - right - (cur->same_));
}

/*
Description:
查询当前Splay的根的前驱.
*/
template<typename T, typename L>
auto Splay<T, L>::find_root_prev() -> SplayNode* {
    CALL(FUNCTION);
    auto u = root_->ch_[0];
    if (!u) return u;
    while (u->ch_[1]) {
        u->push_down();
        u = u->ch_[1];
    } return u;
}

/*
Description:
查询当前Splay的根的后继.
*/
template<typename T, typename L>
auto Splay<T, L>::find_root_next() -> SplayNode* {
    CALL(FUNCTION);
    auto u = root_->ch_[1];
    if (!u) return u;
    while (u->ch_[0]) {
        u->push_down();
        u = u->ch_[0];
    } return u;
}

/*
Description:
查询x管理的SplayNode在Splay中的排名.
x的排名定义为比x小的元素的个数+1.
*/
template<typename T, typename L>
auto Splay<T, L>::rank(SplayHandle* x) -> int {
    CALL(FUNCTION);
    auto target = reinterpret_cast<SplayNode*>(x);
    splay(target, root_);
    return root_->ch_[0] ? root_->ch_[0]->size_ + 1 : 1;
}

}

#endif