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

class SplayHandle : public Handle {
public:
    SplayHandle();
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
struct SplayNode;

template<typename T, typename L, bool CHECKER = std::is_base_of_v<Mergeable, T>>
struct SplayMergeableHelper;
template<typename T, typename L>
struct SplayMergeableHelper<T, L, true> {
    auto push_up(std::shared_ptr<SplayNode<T, L>> cur) -> void {
        CALL(FUNCTION);
        cur->sum = std::make_shared<T>();
        cur->size_ = cur->same_; cur->diff_ = 1;
        if (cur->ch[0]) {
            if (cur->ch[0]->sum) cur->sum->merge(cur->ch[0]->sum);
            cur->size_ += cur->ch[0]->size_;
            cur->diff_ += cur->ch[0]->diff_;
        }
        if (cur->info) cur->sum->merge(cur->info);
        if (cur->ch[1]) {
            if (cur->ch[1]->sum) cur->sum->merge(cur->ch[1]->sum);
            cur->size_ += cur->ch[1]->size_;
            cur->diff_ += cur->ch[1]->diff_;
        }
    }
};
template<typename T, typename L>
struct SplayMergeableHelper<T, L, false> {
    auto push_up(std::shared_ptr<SplayNode<T, L>> cur) -> void {
        CALL(FUNCTION);
        cur->size_ = cur->same_; cur->diff_ = 1;
        if (cur->ch[0]) {
            cur->size_ += cur->ch[0]->size_;
            cur->diff_ += cur->ch[0]->diff_;
        }
        if (cur->ch[1]) {
            cur->size_ += cur->ch[1]->size_;
            cur->diff_ += cur->ch[1]->diff_;
        }
    }
};

template<typename T, typename L, bool CHECKER = std::is_base_of_v<Lazytag, L>>
struct SplayLazytagHelper;
template<typename T, typename L>
struct SplayLazytagHelper<T, L, true> {
    using MySplayNode = SplayNode<T, L>;
    auto push_down(std::shared_ptr<MySplayNode> node) -> void {
        CALL(FUNCTION);
        if (node->ch[0]) {
            node->ch[0]->splay = node->splay;
            if (node->tag) push_tag(node->tag, node->ch[0]);
        }
        if (node->ch[1]) {
            node->ch[1]->splay = node->splay;
            if (node->tag) push_tag(node->tag, node->ch[1]);
        }
        node->tag = nullptr;
    }
    auto push_tag(std::shared_ptr<L> tag, std::shared_ptr<MySplayNode> node) -> void {
        CALL(FUNCTION);
        if (!node->tag) node->tag = std::make_shared<L>();
        if (!node->sum && std::is_base_of_v<Mergeable, T>) node->sum = std::make_shared<T>();
        if (!node->info) node->info = std::make_shared<T>();
        tag->push(node->tag, node);
        if (std::is_base_of_v<Mergeable, T>) tag->push(node->sum, node);
        tag->push(node->info, node);
    }
};
template<typename T, typename L>
struct SplayLazytagHelper<T, L, false> {
    using MySplayNode = SplayNode<T, L>;
    auto push_down(std::shared_ptr<MySplayNode> node) -> void {
        CALL(FUNCTION);
        if (node->ch[0]) node->ch[0]->splay = node->splay;
        if (node->ch[1]) node->ch[1]->splay = node->splay;
    }
    auto push_tag(std::shared_ptr<L> tag, std::shared_ptr<MySplayNode> node) -> void { CALL(FUNCTION); }
};

template<typename T, typename L>
struct SplayInfo : 
    public Pushable,
    public Mergeable,
    public SplayHandle {
    using MySplayNode = SplayNode<T, L>;
    std::shared_ptr<MySplayNode> ch[2];
    std::weak_ptr<MySplayNode> fa;
    std::weak_ptr<MySplayNode> this_;
    std::shared_ptr<T> info;
    int same_;
    int size_;
    int diff_;
    SplayInfo() : same_(1), size_(1), diff_(1) {};
    SplayInfo(std::shared_ptr<SplayInfo> o) {
        same_ = size_ = o->same_;
        diff_ = 1;
        info = std::make_shared<T>(o->info);
        fa = o->fa;
        ch[0] = o->ch[0];
        ch[1] = o->ch[1];
    }
    auto push(std::shared_ptr<Lazytag> tag, std::shared_ptr<Handle> last) { // last == null
        auto push_info = std::dynamic_pointer_cast<Pushable>(info);
        if (push_info) push_info->push(tag, this->shared_from_this());
    }
    auto merge(std::shared_ptr<Mergeable> o) -> void override {
        auto other = std::dynamic_pointer_cast<SplayInfo>(o);
        size_ += other->size_;
        diff_ += other->diff_;
        if (std::is_base_of_v<Mergeable, T>) {
            auto inner_info = std::dynamic_pointer_cast<Mergeable>(info);
            inner_info->merge(other->info);
        }
    }
    auto same() -> int override { return same_; }
    auto size() -> int override { return size_; }
    auto diff() -> int override { return diff_; }
    auto swap_child() -> void override { std::swap(ch[0], ch[1]); }
    auto insert_child(std::shared_ptr<MySplayNode> child, int type) -> void {
        CALL(FUNCTION);
        if (ch[type]) MESSAGE(CLASS, UNKNOWN);
        ch[type] = child;
        child->fa = this_;
    }
    auto remove_child(int type) -> void {
        CALL(FUNCTION);
        ch[type]->fa.reset();
        ch[type] = nullptr;
    }
};

template<typename T, typename L>
struct SplayNode :
    public std::enable_shared_from_this<SplayNode<T, L>> {
    SplayNode(std::shared_ptr<T> info, std::shared_ptr<Splay<T, L>> splay) : 
        info(std::move(info)), splay(std::move(splay)) {}
    virtual ~SplayNode() = default;
    void push_up() {
        auto lch = info->ch[0];
        auto rch = info->ch[1];
        std::shared_ptr<SplayInfo<T, L>> cur_info;
        if (info) cur_info = std::make_shared<SplayInfo<T, L>>(info);
        else cur_info = std::make_shared<SplayInfo<T, L>>();
        auto new_info = std::make_shared<SplayInfo<T, L>>();
        if (lch) new_info->merge(lch->info);
        if (cur_info) new_info->merge(cur_info);
        if (rch) new_info->merge(rch->info);
        info = new_info;
    }
    void push_down() {
        auto lch = info->ch[0];
        auto rch = info->ch[1];
        if (lch) {
            lch->splay = splay;
            lch->push_tag(tag_);
        }
        if (rch) {
            rch->splay = splay;
            rch->push_tag(tag_);
        }
    }
    void push_tag(std::shared_ptr<L> tag) {
        tag->push(tag_, nullptr);
        tag->push(info, nullptr);
    }
    std::weak_ptr<Splay<T, L>> splay;
    std::shared_ptr<L> tag_;
    std::shared_ptr<SplayInfo<T, L>> info;
};

}   // HIDEN END

template<typename T, typename L = bool>
class Splay : 
    public std::enable_shared_from_this<Splay<T, L>> {
public:
    using SplayNode = Hidden::SplayNode<T, L>;
    template<typename A, typename B>
    using SplayLazytagHelper = Hidden::SplayLazytagHelper<A, B>;
    template<typename A, typename B>
    using SplayMergeableHelper = Hidden::SplayMergeableHelper<A, B>;

    Splay() = default;
    auto insert(T info) -> std::shared_ptr<SplayHandle>;
    auto insert(std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle>;
    auto insert(int l, int r, L tag) -> void;
    auto insert(int l, int r, std::shared_ptr<L> tag) -> void;
    auto insert(int k, T info) -> std::shared_ptr<SplayHandle>;
    auto insert(int k, std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle>;
    auto insert(std::shared_ptr<SplayHandle> l, std::shared_ptr<SplayHandle> r, L tag) -> void;
    auto insert(std::shared_ptr<SplayHandle> l, std::shared_ptr<SplayHandle> r, std::shared_ptr<L> tag) -> void;
    auto insert_after(int k, T info) -> std::shared_ptr<SplayHandle>;
    auto insert_after(int k, std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle>;
    auto insert_after(int k, std::shared_ptr<Splay<T, L>> tr) -> std::shared_ptr<SplayHandle>;
    auto insert_before(int k, T info) -> std::shared_ptr<SplayHandle>;
    auto insert_before(int k, std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle>;
    auto insert_before(int k, std::shared_ptr<Splay<T, L>> tr) -> std::shared_ptr<SplayHandle>;
    auto remove(int k) -> void;
    auto remove(T info) -> void;
    auto remove(std::shared_ptr<T> info) -> void;
    auto remove(std::shared_ptr<SplayHandle> x) -> void;
    auto split(int l, int r) -> std::shared_ptr<Splay<T, L>>;
    auto split(std::shared_ptr<SplayHandle> l, std::shared_ptr<SplayHandle> r) -> std::shared_ptr<Splay<T, L>>;
    auto query_info(int k) -> std::shared_ptr<T>;
    auto query_info(std::shared_ptr<SplayHandle>) -> std::shared_ptr<T>;
    auto query_info(T info) -> std::shared_ptr<T>;
    auto query_info(std::shared_ptr<T> info) -> std::shared_ptr<T>;
    auto query_sum(int k) -> std::shared_ptr<T>;
    auto query_sum(int l, int r) -> std::shared_ptr<T>;
    auto rank(std::shared_ptr<SplayHandle> x) -> int;
    auto size() -> int;
private:
    auto root(std::shared_ptr<SplayNode> rt) -> void;
    auto rotate(std::shared_ptr<SplayNode> x, std::shared_ptr<SplayNode>& rt) -> void;
    auto splay(std::shared_ptr<SplayNode> x, std::shared_ptr<SplayNode>& rt) -> void;
    auto insert(std::shared_ptr<SplayNode>& cur, std::shared_ptr<SplayNode> fa, std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle>;
    auto query(std::shared_ptr<SplayNode> cur, std::shared_ptr<T> info) -> std::shared_ptr<SplayNode>;
    auto find_kth_min(std::shared_ptr<SplayNode> cur, int k) -> std::shared_ptr<SplayNode>;
    auto find_kth_max(std::shared_ptr<SplayNode> cur, int k) -> std::shared_ptr<SplayNode>;
    auto find_root_prev() -> std::shared_ptr<SplayNode>;
    auto find_root_next() -> std::shared_ptr<SplayNode>;
    std::shared_ptr<SplayNode> root_;
};

template<typename T, typename L>
auto Splay<T, L>::root(std::shared_ptr<SplayNode> rt) -> void {
    root_ = std::move(rt);
    root_->splay = this->shared_from_this();
}

/*
Description:
获取Splay的大小
*/
template<typename T, typename L>
auto Splay<T, L>::size() -> int {
    CALL(FUNCTION);
    if (!root_) return 0;
    return root_->size_;
}

template<typename T, typename L>
auto Splay<T, L>::rotate(std::shared_ptr<SplayNode> x, std::shared_ptr<SplayNode>& rt) -> void {
    CALL(FUNCTION);
    auto y = x->fa.lock();
    auto z = y->fa.lock();
    int l = (y->ch[0]) != x, r = l^1;
    if (y == rt) rt = x; else if (z->ch[0] == y) z->ch[0] = x; else z->ch[1] = x;
    x->fa = z; y->fa = x;
    if (x->ch[r]) x->ch[r]->fa = y;
    y->ch[l] = x->ch[r]; x->ch[r] = y;
    y->push_up(); x->push_up();
}

template<typename T, typename L>
auto Splay<T, L>::splay(std::shared_ptr<SplayNode> x, std::shared_ptr<SplayNode>& rt) -> void {
    CALL(FUNCTION);
    while (x != rt) {
        auto y = x->fa.lock();
        auto z = y->fa.lock();
        if (z) z->push_down();
        y->push_down();
        x->push_down();
        if (y != rt) {
            if ((z->ch[1] == y) ^ (y->ch[1] == x)) rotate(x, rt);
            else rotate(y, rt);
        }
        rotate(x, rt);
    }
}

template<typename T, typename L>
auto Splay<T, L>::insert(std::shared_ptr<SplayNode>& cur, std::shared_ptr<SplayNode> fa, std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    if (!cur) { cur = std::make_shared<SplayNode>(info, this->shared_from_this()); cur->fa = fa; return cur; }
    auto cur_info = std::dynamic_pointer_cast<Comparable>(cur->info);
    auto new_info = std::dynamic_pointer_cast<Comparable>(info);
    if (!cur_info || !new_info) MESSAGE("Splay<T, L>", UNABLE("Comparable"));
    cur->push_down();
    auto cmp_ans = cur_info->compare_to(new_info);
    std::shared_ptr<SplayHandle> ret;
    if (cmp_ans < 0) ret = insert(cur->ch[1], cur, info);
    else if (cmp_ans == 0) { ret = cur; cur->same_++; }
    else ret = insert(cur->ch[0], cur, info);
    cur->push_up();
    return ret;
}

/*
Description:
插入一个Comparable的元素到Splay中
*/
template<typename T, typename L>
auto Splay<T, L>::insert(T info) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    return insert(std::make_shared<T>(std::move(info)));
}

/*
Description:
插入一个Comparable的元素到Splay中
*/
template<typename T, typename L>
auto Splay<T, L>::insert(std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    return insert(root_, nullptr, info);
}

/*
Description:
对Splay的[l,r]区间打上标记tag
*/
template<typename T, typename L>
auto Splay<T, L>::insert(int l, int r, L tag) -> void {
    CALL(FUNCTION);
    insert(l, r, std::make_shared<L>(std::move(tag)));
}

/*
Description:
对Splay的[l,r]区间打上标记tag
*/
template<typename T, typename L>
auto Splay<T, L>::insert(int l, int r, std::shared_ptr<L> tag) -> void {
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
    if (!prev && !next) { if (root_) root_->push_tag(tag); return; }
    else if (!prev) { splay(next, root_); root_->ch[0]->push_tag(tag); root_->push_up(); return; }
    else if (!next) { splay(prev, root_); root_->ch[1]->push_tag(tag); root_->push_up(); return; }
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->ch[0]->push_tag(tag);
    root_->ch[1]->push_up();
    root_->push_up();
}

/*
Description:
对Splay的[l,r]区间打上标记tag.
l和r以SplayHandle的形式给出.
*/
template<typename T, typename L>
auto Splay<T, L>::insert(std::shared_ptr<SplayHandle> l, std::shared_ptr<SplayHandle> r, L tag) -> void {
    CALL(FUNCTION);
    insert(l, r, std::make_shared<L>(tag));
}

/*
Description:
对Splay的[l,r]区间打上标记tag.
l和r以SplayHandle的形式给出.
*/
template<typename T, typename L>
auto Splay<T, L>::insert(std::shared_ptr<SplayHandle> l, std::shared_ptr<SplayHandle> r, std::shared_ptr<L> tag) -> void {
    CALL(FUNCTION);
    auto lq = std::dynamic_pointer_cast<SplayNode>(l);
    auto rq = std::dynamic_pointer_cast<SplayNode>(r);
    if (lq->belong() != this->shared_from_this()) MESSAGE("Splay<T, L>", SPLAY_HANDLE_NOT_FOUND);
    if (rq->belong() != this->shared_from_this()) MESSAGE("Splay<T, L>", SPLAY_HANDLE_NOT_FOUND);
    int rank_l = rank(lq);
    int rank_r = rank(rq);
    if (rank_l > rank_r) MESSAGE("Splay<T, L>", ENSURE("l <= r"));
    splay(lq, root_);
    auto prev = find_root_prev();
    splay(rq, root_);
    auto next = find_root_next();
    if (!prev && !next) { if(root_) root_->push_tag(tag); return;}
    else if (!prev) { splay(next, root_); root_->ch[0]->push_tag(tag); root_->push_up(); return; }
    else if (!next) { splay(prev, root_); root_->ch[1]->push_tag(tag); root_->push_up(); return; }
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->ch[0]->push_tag(tag);
    root_->ch[1]->push_up();
    root_->push_up();
}

/*
Description:
修改Splay的第k个元素的值为info.
*/
template<typename T, typename L>
auto Splay<T, L>::insert(int k, T info) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    return insert(k, std::make_shared<T>(std::move(info)));
}

/*
Description:
修改Splay的第k个元素的值为info.
*/
template<typename T, typename L>
auto Splay<T, L>::insert(int k, std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    if (k < 1 || k > size()) MESSAGE("Splay<T, L>", ENSURE("1 <= k <= size"));
    auto target = find_kth_min(root_, k);
    splay(target, root_);
    root_->info = info;
    root_->push_up();
    return root_;
}

/*
Description:
在Splay的第k个元素之后插入一个值为info的元素.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_after(int k, T info) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    return insert_after(k, std::make_shared<T>(std::move(info)));
}

/*
Description:
在Splay的第k个元素之后插入一个值为info的元素.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_after(int k, std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    if (k < 0 || k > size()) MESSAGE("Splay<T, L>", ENSURE("0 <= k <= size"));
    auto prev = find_kth_min(root_, k);
    auto next = find_kth_min(root_, k + 1);
    if (!prev && !next) { root_ = std::make_shared<SplayNode>(info, this->shared_from_this()); root_->push_up(); return root_; }
    else if (!prev) { splay(next, root_); root_->insert_child(std::make_shared<SplayNode>(info, this->shared_from_this()), 0); root_->ch[0]->push_up(); root_->push_up(); return root_->ch[0]; }
    else if (!next) { splay(prev, root_); root_->insert_child(std::make_shared<SplayNode>(info, this->shared_from_this()), 1); root_->ch[1]->push_up(); root_->push_up(); return root_->ch[1]; }
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->insert_child(std::make_shared<SplayNode>(info, this->shared_from_this()), 0);
    root_->ch[1]->ch[0]->push_up();
    root_->ch[1]->push_up();
    root_->push_up();
    return root_->ch[1]->ch[0];
}

/*
Description:
在Splay的第k个元素后插入另一个Splay.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_after(int k, std::shared_ptr<Splay<T, L>> tr) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    if (k < 0 || k > size()) MESSAGE("Splay<T, L>", ENSURE("0 <= k <= size"));
    auto prev = find_kth_min(root_, k);
    auto next = find_kth_min(root_, k + 1);
    if (!prev && !next) { root_ = std::move(tr->root_); root_->splay = this->shared_from_this(); root_->push_up(); return root_; }
    else if (!prev) { splay(next, root_); root_->insert_child(std::move(tr->root_) , 0); root_->ch[0]->push_up(); root_->push_up(); return root_->ch[0]; }
    else if (!next) { splay(prev, root_); root_->insert_child(std::move(tr->root_) , 1); root_->ch[1]->push_up(); root_->push_up(); return root_->ch[1]; }
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->insert_child(std::move(tr->root_), 0);
    root_->ch[1]->ch[0]->push_up();
    root_->ch[1]->push_up();
    root_->push_up();
    return root_->ch[1]->ch[0];
} 

/*
Description:
在Splay的第k个元素之前插入一个值为info的元素.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_before(int k, T info) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    return insert_before(k, std::make_shared<T>(std::move(info)));
}

/*
Description:
在Splay的第k个元素之前插入一个值为info的元素.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_before(int k, std::shared_ptr<T> info) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    if (k < 1 || k > size() + 1) MESSAGE("Splay<T, L>", ENSURE("1 <= k <= size + 1"));
    auto prev = find_kth_min(root_, k - 1);
    auto next = find_kth_min(root_, k);
    if (!prev && !next) { root_ = std::make_shared<SplayNode>(info, this->shared_from_this()); root_->push_up(); return root_; }
    else if (!prev) { splay(next, root_); root_->insert_child(std::make_shared<SplayNode>(info, this->shared_from_this()), 0); root_->ch[0]->push_up(); root_->push_up(); return root_->ch[0]; }
    else if (!next) { splay(prev, root_); root_->insert_child(std::make_shared<SplayNode>(info, this->shared_from_this()), 1); root_->ch[1]->push_up(); root_->push_up(); return root_->ch[1]; }
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->insert_child(std::make_shared<SplayNode>(info, this->shared_from_this()), 0);
    root_->ch[1]->ch[0]->push_up();
    root_->ch[1]->push_up();
    root_->push_up();
    return root_->ch[1]->ch[0];
}

/*
Description:
在Splay的第k个元素前插入另一个同类型的Splay.
*/
template<typename T, typename L>
auto Splay<T, L>::insert_before(int k, std::shared_ptr<Splay<T, L>> tr) -> std::shared_ptr<SplayHandle> {
    CALL(FUNCTION);
    if (k < 0 || k > size()) MESSAGE("Splay<T, L>", ENSURE("1 <= k <= size + 1"));
    auto prev = find_kth_min(root_, k - 1);
    auto next = find_kth_min(root_, k);
    if (!prev && !next) { root_ = std::move(tr->root_); root_->splay = this->shared_from_this(); root_->push_up(); return root_; }
    else if (!prev) { splay(next, root_); root_->insert_child(std::move(tr->root_), 0); root_->ch[0]->push_up(); root_->push_up(); return root_->ch[0]; }
    else if (!next) { splay(prev, root_); root_->insert_child(std::move(tr->root_), 1); root_->ch[1]->push_up(); root_->push_up(); return root_->ch[1]; }
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->insert_child(std::move(tr->root_), 0);
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
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->remove_child(0);
    root_->ch[1]->push_up();
    root_->push_up();
}

/*
Description:
从Splay中删除一个值为info的元素.
如果有多个值为info的元素被存储在同一个SplayNode中，该SplayNode不会被删除.
*/
template<typename T, typename L>
auto Splay<T, L>::remove(T info) -> void {
    CALL(FUNCTION);
    remove(std::make_shared<T>(std::move(info)));
}

/*
Description:
从Splay中删除一个值为info的元素.
如果有多个值为info的元素被存储在同一个SplayNode中，该SplayNode不会被删除.
*/
template<typename T, typename L>
auto Splay<T, L>::remove(std::shared_ptr<T> info) -> void {
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
    splay(prev, root_); splay(next, root_->ch[1]);
    root_->ch[1]->remove_child(0);
    root_->ch[1]->push_up();
    root_->push_up();
}

/*
Description:
删除由x管理的SplayNode，即使该SplayNode中保存了多个元素.
*/
template<typename T, typename L>
auto Splay<T, L>::remove(std::shared_ptr<SplayHandle> x) -> void {
    CALL(FUNCTION);
    auto target = std::dynamic_pointer_cast<SplayNode>(x);
    if (target->belong() != this->shared_from_this()) MESSAGE("Splay<T, L>", SPLAY_HANDLE_NOT_FOUND);
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
auto Splay<T, L>::split(int l, int r) -> std::shared_ptr<Splay<T, L>> {
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
    if (!prev && !next) { auto ans = std::make_shared<Splay<T, L>>(); ans->root(std::move(root_)); return ans; }
    else if (!prev) { splay(next, root_); auto rt = root_->ch[0]; root_->remove_child(0); PUSHUP(Splay, root_); auto ans = std::make_shared<Splay<T, L>>(); ans->root(rt); return ans; }
    else if (!next) { splay(prev, root_); auto rt = root_->ch[1]; root_->remove_child(1); PUSHUP(Splay, root_); auto ans = std::make_shared<Splay<T, L>>(); ans->root(rt); return ans; }
    splay(prev, root_); splay(next, root_->ch[1]);
    auto rt = root_->ch[1]->ch[0];
    root_->ch[1]->remove_child(0);
    PUSHUP(Splay, root_->ch[1]);
    PUSHUP(Splay, root_);
    auto ans = std::make_shared<Splay<T, L>>(); ans->root(rt); return ans;
}

/*
Description:
查询Splay中[k,k]这段元素的sum.
需要保证T是Mergeable的.
*/
template<typename T, typename L>
auto Splay<T, L>::query_sum(int k) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    return query_sum(k, k);
}

/*
Description:
查询Splay中[l,r]这段区间的sum.
需要保证T是Mergeable的.
*/
template<typename T, typename L>
auto Splay<T, L>::query_sum(int l, int r) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    if (!std::is_base_of_v<Mergeable, T>) MESSAGE("Splay<T, L>", UNABLE("Mergeable"));
    if (l > r) MESSAGE("Splay<T, L>", ENSURE("l <= r"));
    auto lq = find_kth_min(root_, l);
    if (!lq) MESSAGE("Splay<T, L>", ENSURE("1 <= l <= size"));
    auto rq = find_kth_min(root_, r);
    if (!rq) MESSAGE("Splay<T, L>", ENSURE("1 <= r <= size"));
    splay(lq, root_);
    auto prev = find_root_prev();
    splay(rq, root_);
    auto next = find_root_next();
    if (!prev && !next) return root_->sum;
    else if (!prev) { splay(next, root_); return root_->ch[0]->sum; }
    else if (!next) { splay(prev, root_); return root_->ch[1]->sum; }
    splay(prev, root_); splay(next, root_->ch[1]);
    return root_->ch[1]->ch[0]->sum;
}

template<typename T, typename L>
auto Splay<T, L>::query(std::shared_ptr<SplayNode> cur, std::shared_ptr<T> info) -> std::shared_ptr<SplayNode> {
    if (!cur) return nullptr;
    auto cur_info = std::dynamic_pointer_cast<Comparable>(cur->info);
    auto new_info = std::dynamic_pointer_cast<Comparable>(info);
    if (!cur_info || !new_info) MESSAGE("Splay<T, L>", UNABLE("Comparable"));
    PUSHDOWN(Splay, cur);
    auto cmp_ans = cur_info->compare_to(new_info);
    if (cmp_ans < 0) return query(cur->ch[1], info);
    else if (cmp_ans == 0) return cur;
    else return query(cur->ch[0], info);
}

/*
Description:
查询等于T的元素的info.
需要保证T是Comparable的.
*/
template<typename T, typename L>
auto Splay<T, L>::query_info(T info) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    return query_info(std::make_shared<T>(info));
}

/*
Description:
查询等于T的元素的info.
需要保证T是Comparable的.
*/
template<typename T, typename L>
auto Splay<T, L>::query_info(std::shared_ptr<T> info) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    auto target = query(root_, info);
    return target ? target->info : nullptr;
}

/*
Description:
查询x所管理的SplayNode的info
*/
template<typename T, typename L>
auto Splay<T, L>::query_info(std::shared_ptr<SplayHandle> x) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    auto target = std::dynamic_pointer_cast<SplayNode>(x);
    return target->info;
}

/*
Description:
查询第k个元素的info.
*/
template<typename T, typename L>
auto Splay<T, L>::query_info(int k) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    auto target = find_kth_min(root_, k);
    if (!target) MESSAGE("Splay<T, L>", ENSURE("1 <= l <= size"));
    return target->info;
}

/*
Description:
查询Splay中从小到大第k个元素.
*/
template<typename T, typename L>
auto Splay<T, L>::find_kth_min(std::shared_ptr<SplayNode> cur, int k) -> std::shared_ptr<SplayNode> {
    CALL(FUNCTION);
    if (!cur) return nullptr;
    PUSHDOWN(Splay, cur);
    int left = (cur->ch[0]) ? (cur->ch[0]->size_) : 0;
    if (left + 1 <= k && k <= left + (cur->same_)) return cur;
    if (left >= k) return find_kth_min(cur->ch[0], k);
    return find_kth_min(cur->ch[1], k - left - (cur->same_));
}

/*
Description:
查询Splay中从大到小第k个元素.
*/
template<typename T, typename L>
auto Splay<T, L>::find_kth_max(std::shared_ptr<SplayNode> cur, int k) -> std::shared_ptr<SplayNode> {
    CALL(FUNCTION);
    if (!cur) return nullptr;
    PUSHDOWN(Splay, cur);
    int right = (cur->ch[1]) ? (cur->ch[1]->size_) : 0;
    if (right + 1 <= k && k <= right + (cur->same_)) return cur;
    if (right >= k) return find_kth_max(cur->ch[1], k);
    return find_kth_max(cur->ch[0], k - right - (cur->same_));
}

/*
Description:
查询当前Splay的根的前驱.
*/
template<typename T, typename L>
auto Splay<T, L>::find_root_prev() -> std::shared_ptr<SplayNode> {
    CALL(FUNCTION);
    auto u = root_->ch[0];
    if (!u) return u;
    while (u->ch[1]) {
        PUSHDOWN(Splay, u);
        u = u->ch[1];
    } return u;
}

/*
Description:
查询当前Splay的根的后继.
*/
template<typename T, typename L>
auto Splay<T, L>::find_root_next() -> std::shared_ptr<SplayNode> {
    CALL(FUNCTION);
    auto u = root_->ch[1];
    if (!u) return u;
    while (u->ch[0]) {
        PUSHDOWN(Splay, u);
        u = u->ch[0];
    } return u;
}

/*
Description:
查询x管理的SplayNode在Splay中的排名.
x的排名定义为比x小的元素的个数+1.
*/
template<typename T, typename L>
auto Splay<T, L>::rank(std::shared_ptr<SplayHandle> x) -> int {
    CALL(FUNCTION);
    auto target = std::dynamic_pointer_cast<SplayNode>(x);
    if (target->belong() != this->shared_from_this()) MESSAGE("Splay<T, L>", SPLAY_HANDLE_NOT_FOUND);
    splay(target, root_);
    return root_->ch[0] ? root_->ch[0]->size_ + 1 : 1;
}

}

#endif