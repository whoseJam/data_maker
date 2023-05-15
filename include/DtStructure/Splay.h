#ifndef SPLAY_H
#define SPLAY_H

#include <memory>
#include <utility>
#include <functional>

#include "Debug.h"
#include "Logger.h"
#include "Dtstructure.h"

namespace mk {

template<typename T, typename L = bool>
class Splay {
public:
    Splay() = default;
    void insert(T info);
    void insert(std::shared_ptr<T> info);
    void insert_after(int k, T info);
    void insert_after(int k, std::shared_ptr<T> info);
    void insert_before(int k, T info);
    void insert_before(int k, std::shared_ptr<T> info);
    void insert(int l, int r, L tag);
    auto remove(int k) -> void;
    auto query(int l, int r) -> std::shared_ptr<T>;
    auto size() -> int;
private:

    struct SplayNode :
        public std::enable_shared_from_this<Splay<T, L>::SplayNode> {
        SplayNode(std::shared_ptr<T> info) : info(std::move(info)), same(1), size(1), diff(1) {}
        SplayNode(std::shared_ptr<T> info, std::shared_ptr<SplayNode> fa) :
            info(std::move(info)), fa(std::move(fa)), same(1), size(1), diff(1) {}
        auto insert_child(std::shared_ptr<SplayNode> child, int type) -> void {
            CALL("Splay<T, L>::SplayNode", "insert_child(std::shared_ptr<SplayNode>, int) -> void");
            if (ch[type]) MESSAGE("Splay<T, L>::SplayNode", UNKNOWN);
            ch[type] = child;
            child->fa = this->shared_from_this();
        }
        auto remove_child(int type) -> void {
            CALL("Splay<T, L>::SplayNode", "remove_child(int) -> void");
            ch[type] = nullptr;
        }
        std::shared_ptr<L> tag;
        std::shared_ptr<T> sum;
        std::shared_ptr<T> info;
        std::shared_ptr<SplayNode> fa;
        std::shared_ptr<SplayNode> ch[2];
        int same;
        int size;
        int diff;
    };
    auto rotate(std::shared_ptr<SplayNode> x, std::shared_ptr<SplayNode>& rt) -> void;
    auto splay(std::shared_ptr<SplayNode> x, std::shared_ptr<SplayNode>& rt) -> void;
    auto insert(std::shared_ptr<SplayNode>& cur, std::shared_ptr<SplayNode> fa, std::shared_ptr<T> info) -> void;
    auto find_kth_min(std::shared_ptr<SplayNode> cur, int k) -> std::shared_ptr<SplayNode>;
    auto find_kth_max(std::shared_ptr<SplayNode> cur, int k) -> std::shared_ptr<SplayNode>;
    auto find_root_prev() -> std::shared_ptr<SplayNode>;
    auto find_root_next() -> std::shared_ptr<SplayNode>;

    template<typename V, bool CHECKER = std::is_base_of_v<Mergeable, V>>
    struct MergeableHelper;
    template<typename V>
    struct MergeableHelper<V, true> {
        void push_up(std::shared_ptr<SplayNode> cur) {
            CALL("Splay<T, L>::MergeableHelper<V, true>", "push_up(std::shared_ptr<SplayNode>) -> void");
            cur->sum = std::make_shared<T>(Zero());
            cur->size = cur->same; cur->diff = 1;
            if (cur->ch[0]) {
                if (cur->ch[0]->sum) cur->sum->merge(cur->ch[0]->sum);
                cur->size += cur->ch[0]->size;
                cur->diff += cur->ch[0]->diff;
            }
            if (cur->info) cur->sum->merge(cur->info);
            if (cur->ch[1]) {
                if (cur->ch[1]->sum) cur->sum->merge(cur->ch[1]->sum);
                cur->size += cur->ch[1]->size;
                cur->diff += cur->ch[1]->diff;
            }
        }
    };
    template<typename V>
    struct MergeableHelper<V, false> {
        auto push_up(std::shared_ptr<SplayNode> cur) -> void {
            CALL("Splay<T, L>::MergeableHelper<V, false>", "push_up(std::shared_ptr<SplayNode>) -> void");
            cur->size = cur->same; cur->diff = 1;
            if (cur->lch) {
                cur->size += cur->lch->size;
                cur->diff += cur->lch->diff;
            }
            if (cur->rch) {
                cur->size += cur->rch->size;
                cur->diff += cur->rch->diff;
            }
        }
    };

    template<typename V, bool CHECKER = std::is_base_of_v<Lazytag, V>>
    struct LazytagHelper;
    template<typename V>
    struct LazytagHelper<V, true> {
        auto push_down(std::shared_ptr<SplayNode> node) -> void {
            CALL("Splay<T, L>::LazytagHelper<V, true>", "push_down(std::shared_ptr<SplayNode>) -> void");
            if (!node->tag) return;
            push_tag(node->tag, node->lch);
            push_tag(node->tag, node->rch);
            node->tag = nullptr;
        }
        auto push_tag(std::shared_ptr<L> tag, std::shared_ptr<SplayNode> node) -> void {
            CALL("Splay<T, L>::LazytagHelper<V, true>", "push_tag(std::shared_ptr<L>, std::shared_ptr<SplayNode>) -> void");
            if (!node->tag) node->tag = std::make_shared<L>(Zero());
            if (!node->sum) node->sum = std::make_shared<T>(Zero());
            if (!node->info) node->info = std::make_shared<T>(Zero());
            tag->push(node->tag);
            tag->push(node->sum);
            tag->push(node->info);
        }
    };
    template<typename V>
    struct LazytagHelper<V, false> {
        auto push_down(std::shared_ptr<SplayNode> node) -> void {}
        auto push_tag(std::shared_ptr<L> tag, std::shared_ptr<SplayNode> node) -> void {}
    };

    std::shared_ptr<SplayNode> root;
};

template<typename T, typename L>
auto Splay<T, L>::size() -> int {
    CALL("Splay<T, L>", "size() -> int");
    if (!root) return 0;
    return root->size;
}

template<typename T, typename L>
auto Splay<T, L>::rotate(std::shared_ptr<SplayNode> x, std::shared_ptr<SplayNode>& rt) -> void {
    CALL("Splay<T, L>", "rotate(std::shared_ptr<SplayNode>, std::shared_ptr<SplayNode>&)");
    auto y = x->fa, z = y->fa;
    int l = (y->ch[0]) != x, r = l^1;
    if (y == rt) rt = x; else if (z->ch[0] == y) z->ch[0] = x; else z->ch[1] = x;
    x->fa = z; y->fa = x;
    if (x->ch[r]) x->ch[r]->fa = y;
    y->ch[l] = x->ch[r]; x->ch[r] = y;
    PUSHUP(y); PUSHUP(x);
}

template<typename T, typename L>
auto Splay<T, L>::splay(std::shared_ptr<SplayNode> x, std::shared_ptr<SplayNode>& rt) -> void {
    CALL("Splay<T, L>", "splay(std::shared_ptr<SplayNode>, std::shared_ptr<SplayNode>&)");
    while (x != rt) {
        auto y = x->fa;
        auto z = y->fa;
        if (z) LazytagHelper<L>().push_down(z);
        LazytagHelper<L>().push_down(y);
        LazytagHelper<L>().push_down(x);
        if (y != rt) {
            if ((z->ch[1] == y) ^ (y->ch[1] == x)) rotate(x, rt);
            else rotate(y, rt);
        }
        rotate(x, rt);
    }
}

template<typename T, typename L>
auto Splay<T, L>::insert(std::shared_ptr<SplayNode>& cur, std::shared_ptr<SplayNode> fa, std::shared_ptr<T> info) -> void {
    CALL("Splay<T, L>", "insert(std::shared_ptr<SplayNode>&, std::shared_ptr<SplayNode>, std::shared_ptr<T>) -> void");
    if (!cur) {cur = std::make_shared<SplayNode>(info, fa); return;}
    auto cur_info = std::dynamic_pointer_cast<Comparable>(cur->info);
    auto new_info = std::dynamic_pointer_cast<Comparable>(info);
    if (!cur_info || !new_info) MESSAGE("Splay<T, L>", UNABLE("Comparable"));
    auto cmp_ans = cur_info->compare_to(new_info);
    if (cmp_ans < 0) insert(cur->ch[1], cur, info);
    else if (cmp_ans == 0) cur->same++;
    else insert(cur->ch[0], cur, info);
    MergeableHelper<T>().push_up(cur);
}

template<typename T, typename L>
auto Splay<T, L>::insert(T info) -> void {
    CALL("Splay<T, L>", "insert(T) -> void");
    insert(std::make_shared<T>(std::move(info)));
}

template<typename T, typename L>
auto Splay<T, L>::insert(std::shared_ptr<T> info) -> void {
    CALL("Splay<T, L>", "insert(std::shared_ptr<T>) -> void");
    insert(root, nullptr, info);
}

template<typename T, typename L>
auto Splay<T, L>::insert_after(int k, T info) -> void {
    CALL("Splay<T, L>", "insert_after(int, T) -> void");
    insert_after(k, std::make_shared<T>(std::move(info)));
}

template<typename T, typename L>
auto Splay<T, L>::insert_after(int k, std::shared_ptr<T> info) -> void {
    CALL("Splay<T, L>", "insert_after(int, std::shared_ptr<T>) -> void");
    if (k < 0 || k > size()) MESSAGE("Splay<T, L>", ENSURE("0 <= k <= size"));
    auto prev = find_kth_min(root, k);
    auto next = find_kth_min(root, k + 1);
    if (!prev && !next) { root = std::make_shared<SplayNode>(info); return; }
    else if (!prev) { splay(next, root); root->insert_child(std::make_shared<SplayNode>(info), 0); PUSHUP(root->ch[0]); PUSHUP(root); return; }
    else if (!next) { splay(prev, root); root->insert_child(std::make_shared<SplayNode>(info), 1); PUSHUP(root->ch[1]); PUSHUP(root); return; }
    splay(prev, root); splay(next, root->ch[1]);
    root->ch[1]->insert_child(std::make_shared<SplayNode>(info), 0);
    PUSHUP(root->ch[1]->ch[0]);
    PUSHUP(root->ch[1]);
    PUSHUP(root);
}

template<typename T, typename L>
auto Splay<T, L>::insert_before(int k, T info) -> void {
    CALL("Splay<T, L>", "insert_before(int, T) -> void");
    insert_before(k, std::make_shared<T>(std::move(info)));
}

template<typename T, typename L>
auto Splay<T, L>::insert_before(int k, std::shared_ptr<T> info) -> void {
    CALL("Splay<T, L>", "insert_before(int, std::shared_ptr<T>) -> void");
    if (k < 1 || k > size() + 1) MESSAGE("Splay<T, L>", ENSURE("1 <= k <= size + 1"));
    auto prev = find_kth_min(root, k - 1);
    auto next = find_kth_min(root, k);
    if (!prev && !next) { root = std::make_shared<SplayNode>(info); return; }
    else if (!prev) { splay(next, root); root->insert_child(std::make_shared<SplayNode>(info), 0); PUSHUP(root->ch[0]); PUSHUP(root); return; }
    else if (!next) { splay(prev, root); root->insert_child(std::make_shared<SplayNode>(info), 1); PUSHUP(root->ch[1]); PUSHUP(root); return; }
    splay(prev, root); splay(next, root->ch[1]);
    root->ch[1]->insert_child(std::make_shared<SplayNode>(info), 0);
    PUSHUP(root->ch[1]->ch[0]);
    PUSHUP(root->ch[1]);
    PUSHUP(root);
}

template<typename T, typename L>
auto Splay<T, L>::remove(int k) -> void {
    CALL("Splay<T, L>", "remove(int) -> void");
    auto target = find_kth_min(root, k);
    if (!target) MESSAGE("Splay<T, L>", ENSURE("1 <= k <= size"));
    splay(target, root);
    auto prev = find_root_prev();
    auto next = find_root_next();
    if (!prev && !next) root = nullptr;
    else if (!prev) { splay(next, root); root->remove_child(0); return; }
}

template<typename T, typename L>
auto Splay<T, L>::query(int l, int r) -> std::shared_ptr<T> {
    CALL("Splay<T, L>", "query(int, int) -> std::shared_ptr<T>");
    auto lq = find_kth_min(root, l);
    if (!lq) MESSAGE("Splay<T, L>", ENSURE("1 <= l <= size"));
    auto rq = find_kth_min(root, r);
    if (!rq) MESSAGE("Splay<T, L>", ENSURE("1 <= r <= size"));
    splay(lq, root);
    auto prev = find_root_prev();
    splay(rq, root);
    auto next = find_root_next();
    if (!prev && !next) return root->sum;
    else if (!prev) { splay(next, root); return root->ch[0]->sum; }
    else if (!next) { splay(prev, root); return root->ch[1]->sum; }
    splay(prev, root); splay(next, root->ch[1]);
    return root->ch[1]->ch[0]->sum;
}

template<typename T, typename L>
auto Splay<T, L>::find_kth_min(std::shared_ptr<SplayNode> cur, int k) -> std::shared_ptr<SplayNode> {
    CALL("Splay", "find_kth_min");
    if (!cur) return nullptr;
    PUSHDOWN(cur);
    int left = (cur->ch[0]) ? (cur->ch[0]->size) : 0;
    if (left + 1 <= k && k <= left + (cur->same)) return cur;
    if (left >= k) return find_kth_min(cur->ch[0], k);
    return find_kth_min(cur->ch[1], k - left - (cur->same));
}

template<typename T, typename L>
auto Splay<T, L>::find_kth_max(std::shared_ptr<SplayNode> cur, int k) -> std::shared_ptr<SplayNode> {
    CALL("Splay", "find_kth_min");
    if (!cur) return nullptr;
    PUSHDOWN(cur);
    int right = (cur->ch[1]) ? (cur->ch[1]->size) : 0;
    if (right + 1 <= k && k <= right + (cur->same)) return cur;
    if (right >= k) return find_kth_max(cur->ch[1], k);
    return find_kth_max(cur->ch[0], k - right - (cur->same));
}

template<typename T, typename L>
auto Splay<T, L>::find_root_prev() -> std::shared_ptr<SplayNode> {
    CALL("Splay", "find_root_prev");
    auto u = root->ch[0];
    if (!u) return u;
    while (u->ch[1]) {
        PUSHDOWN(u);
        u = u->ch[1];
    } return u;
}

template<typename T, typename L>
auto Splay<T, L>::find_root_next() -> std::shared_ptr<SplayNode> {
    CALL("Splay", "find_root_next");
    auto u = root->ch[1];
    if (!u) return u;
    while (u->ch[0]) {
        LazytagHelper<T>().push_down(u);
        u = u->ch[1];
    } return u;
}

}

#endif