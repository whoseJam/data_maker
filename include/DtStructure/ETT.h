#ifndef ETT_H
#define ETT_H

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <functional>

#include "Debug.h"
#include "Edge.h"
#include "Integer.h"
#include "Logger.h"
#include "Splay.h"
#include "Dtstructure.h"

namespace mk {

extern int COUNT_ETTINFO;
extern int COUNT_ETTTAG;
extern int COUNT_ETT;
extern bool gdebug;

struct ETTHandle : public Handle {
    ETTHandle() = default;
    virtual auto edge_size() -> int = 0;
    virtual auto vertex_size() -> int = 0;
};

namespace Hidden {

template<typename L>
struct ETTTag : public Lazytag {
    L tag;
    bool rev;
    ETTTag() : rev(false) { COUNT_ETTTAG++; }
    ETTTag(const L& tag) : tag(tag), rev(0) { COUNT_ETTTAG++; }
    ETTTag(bool r) : rev(r) { COUNT_ETTTAG++; }
    ETTTag(const ETTTag& other) : tag(other.tag), rev(other.rev) { COUNT_ETTTAG++; }
    ~ETTTag() { COUNT_ETTTAG--; }
    template<typename U, typename CHECKER = 
        std::conditional_t<
            lazytag_is_pushable_with_handle_v<L, SplayHandle>, std::integral_constant<int, 1>, 
        std::conditional_t<
            lazytag_is_pushable_without_handle_v<L>,           std::integral_constant<int, 2>, 
                                                               std::integral_constant<int, 3>>>>
    struct push_helper;
    template<typename U>
    struct push_helper<U, std::integral_constant<int, 1>> {
        auto call(ETTTag* cur, ETTTag* other, SplayHandle* handle) -> void {
            cur->tag.push(&(other->tag), handle);
        }
    };
    template<typename U>
    struct push_helper<U, std::integral_constant<int, 2>> {
        auto call(ETTTag* cur, ETTTag* other, SplayHandle* handle) -> void {
            cur->tag.push(&(other->tag));
        }
    };
    template<typename U>
    struct push_helper<U, std::integral_constant<int, 3>> {
        auto call(ETTTag* cur, ETTTag* other, SplayHandle* handle) -> void {}
    };
    auto push(ETTTag* other, SplayHandle* handle) -> void {
        rev ^= other->rev;
        if (other->rev) handle->swap_child();
        push_helper<L>().call(this, other, handle);
    }
};

template<typename T, typename L>
struct ETTInfo : 
    public Mergeable,
    public ETTHandle {
    int x, y, ne, nv;
    T info;
    ETTInfo() : x(-1), y(-1), nv(0), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(const T& info, int x) : info(info), x(x), y(-1), nv(1), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(const T& info, int x, int y) : info(info), x(x), y(y), nv(0), ne(1) { COUNT_ETTINFO++; }
    ETTInfo(const ETTInfo& other) : x(other.x), y(other.y), ne(other.ne), nv(other.nv), info(other.info) { COUNT_ETTINFO++; } 
    ~ETTInfo() { COUNT_ETTINFO--; }
    virtual auto edge_size() -> int override { return ne; }
    virtual auto vertex_size() -> int override { return nv; }

    template<typename U, typename CASE = 
        std::conditional_t<
            info_is_mergeable_with_handle_v<U, ETTHandle>, std::integral_constant<int, 1>, 
        std::conditional_t<
            info_is_mergeable_without_handle_v<U>,         std::integral_constant<int, 2>, 
                                                           std::integral_constant<int, 3>>>>
    struct merge_helper;
    template<typename U>
    struct merge_helper<U, std::integral_constant<int, 1>> {
        auto call(ETTInfo* cur, ETTInfo* other) { cur->info.merge(&(other->info), other); } 
    };  // T合并时需要用到全部参数
    template<typename U>
    struct merge_helper<U, std::integral_constant<int, 2>> {
        auto call(ETTInfo* cur, ETTInfo* other) { cur->info.merge(&(other->info)); } 
    };  // T合并时只需要用到另一个节点的用户定义参数即可
    template<typename U>
    struct merge_helper<U, std::integral_constant<int, 3>> {
        auto call(ETTInfo* cur, ETTInfo* other) {} 
    };  // T是不可合并的

    template<typename U, typename R, typename CASE = 
        std::conditional_t<
            info_is_pushable_with_handle_v<U, R, ETTHandle>, std::integral_constant<int, 1>, 
        std::conditional_t<
            info_is_pushable_without_handle_v<U, R>,         std::integral_constant<int, 2>, 
                                                             std::integral_constant<int, 3>>>>
    struct push_helper;
    template<typename U, typename R>
    struct push_helper<U, R, std::integral_constant<int, 1>> {
        auto call(ETTInfo* cur, L* other) { cur->info.push(other, cur); } 
    };  // L push -> T 时需要用到全部参数
    template<typename U, typename R>
    struct push_helper<U, R, std::integral_constant<int, 2>> {
        auto call(ETTInfo* cur, L* other) { cur->info.push(other); } 
    };  // L push -> T 时只需要用到另一个节点的用户定义参数即可
    template<typename U, typename R>
    struct push_helper<U, R, std::integral_constant<int, 3>> {
        auto call(ETTInfo* cur, L* other) {}
    };  // T是不可push的
    
    auto merge(ETTInfo* other) -> void {
        x = other->x;
        y = other->y;
        ne += other->ne;
        nv += other->nv;
        merge_helper<T>().call(this, other);
    }
    auto push(ETTTag<L>* other, SplayHandle* handle) -> void {
        push_helper<T, L>().call(this, &(other->tag));
    }
};

}

template<typename T, typename L = std::nullptr_t>
class ETT {
public:
    ETT();
    ~ETT() { CALL(FUNCTION); COUNT_ETT--; }
    auto is_accessible(int u, int v) -> bool;
    auto is_ancestor_of(int a, int s) -> bool;
    auto is_root(int u) -> bool;
    auto make_root(int u) -> void;
    auto link(int u, int v) -> void;
    auto link(int u, int v, const T& info) -> void;
    auto cut(int u, int v) -> void;
    auto query_sum(int u) -> T;
    auto query_info(int u) -> T;
    auto insert(int u, const L& tag) -> void;
    auto new_node(int u) -> void;
    auto new_node(int u, const T& info) -> void;
    auto clear() -> void;
public:

    auto make_edge(int x, int y) -> std::pair<int, int> {
        return std::make_pair(std::min(x, y), std::max(x, y));
    }
    using MyETTInfo = Hidden::ETTInfo<T, L>;
    using MyETTTag = Hidden::ETTTag<L>;
    using SplayType = Splay<MyETTInfo, MyETTTag>;
    using SplayNodeType = typename Hidden::SplayNode<MyETTInfo, MyETTTag>;
    std::map<int, SplayType> eulers;
    std::map<std::pair<int, int>, std::pair<SplayHandle*, SplayHandle*>> emap;
    std::map<int, SplayHandle*> vmap;
    static inline bool constexpr Test = info_is_pushable_with_handle_v<MyETTInfo, MyETTTag, SplayHandle>;
};

template<typename T, typename L>
auto ETT<T, L>::clear() -> void {
    CALL(FUNCTION);
    eulers.clear();
    emap.clear();
    vmap.clear();
}

template<typename T, typename L>
ETT<T, L>::ETT() {
    COUNT_ETT++;
    eulers.clear();
    emap.clear();
    vmap.clear();
}

/*
Description:
将u作为u所在的树的根.
如果u不存在则会报错.
*/
template<typename T, typename L>
auto ETT<T, L>::make_root(int u) -> void {
    CALL(FUNCTION);
    auto iter = vmap.find(u);
    if (iter == vmap.end()) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto handle = iter->second;
    SplayNodeType* spl_node = dynamic_cast<SplayNodeType*>(handle);
    int root = spl_node->query_first_info().x;
    if (root == u) return;
    SplayType splay = std::move(eulers[root]);
    int rank = splay.rank(handle);
    MyETTInfo edge = splay.query_info(rank - 1);
    auto edge_info = emap[make_edge(edge.x, edge.y)];
    auto handle_e1 = edge_info.first;
    auto handle_e2 = edge_info.second; 
    if (!handle_e1 || !handle_e2) MESSAGE("ETT<T, L>", UNKNOWN);
    int r1 = splay.rank(handle_e1);
    int r2 = splay.rank(handle_e2);
    if (r1 > r2) {
        std::swap(r1, r2);
        std::swap(handle_e1, handle_e2);
    }
    int n = splay.size();
    int a = r1;
    int b = r2 - 1;
    SplayType bef = 1 <= a ? splay.split(1, a) : SplayType();
    SplayType mid = 1 <= b - a ? splay.split(1, b - a) : SplayType();
    SplayType aft = 1 <= n - b ? splay.split(1, n - b) : SplayType();
    if (1 <= a) bef.insert(1, a, MyETTTag(true));
    if (1 <= n - b) aft.insert(1, n - b, MyETTTag(true));
    splay.insert_after(splay.size(), std::move(mid));
    splay.insert_after(splay.size(), std::move(bef));
    splay.insert_after(splay.size(), std::move(aft));
    eulers.erase(root);
    eulers.emplace(u, std::move(splay));
}

/*
Description:
判断u和v是否在同一棵树中.
如果u或者v不存在，则视为u和v不在同一棵树中.
*/
template<typename T, typename L>
auto ETT<T, L>::is_accessible(int u, int v) -> bool {
    CALL(FUNCTION);
    auto iter_u = vmap.find(u);
    if (iter_u == vmap.end()) return false;
    auto iter_v = vmap.find(v);
    if (iter_v == vmap.end()) return false;
    auto handle_u = iter_u->second;
    auto handle_v = iter_v->second;
    if (!handle_u || !handle_v) return false;
    int root_of_u = dynamic_cast<SplayNodeType*>(handle_u)->query_first_info().x;
    int root_of_v = dynamic_cast<SplayNodeType*>(handle_v)->query_first_info().x;
    return (root_of_u == root_of_v);
}

template<typename T, typename L>
auto ETT<T, L>::is_root(int u) -> bool {
    CALL(FUNCTION);
    auto iter = vmap.find(u);
    if (iter == vmap.end()) return false;
    auto handle = iter->second;
    auto root = dynamic_cast<SplayNodeType*>(handle)->query_first_info().x;
    return u == root;
}

/*
Description:
判断a是否为s的祖先.
如果某个点不存在，结果为false.
*/
template<typename T, typename L>
auto ETT<T, L>::is_ancestor_of(int a, int s) -> bool {
    CALL(FUNCTION);
    if (!is_accessible(a, s)) return false;
    auto iter_a = vmap.find(a);
    if (iter_a == vmap.end()) return false;
    auto iter_s = vmap.find(s);
    if (iter_s == vmap.end()) return false;
    auto handle_a = iter_a->second;
    auto handle_s = iter_s->second;
    SplayType& splay = eulers[dynamic_cast<SplayNodeType*>(handle_a)->query_first_info().x];
    int ra = splay.rank(handle_a);
    int rs = splay.rank(handle_s);
    if (ra == 1) return true;
    MyETTInfo edge = splay.query_info(ra - 1);
    auto edge_info = emap[make_edge(edge.x, edge.y)];
    auto handle_e1 = edge_info.first;
    auto handle_e2 = edge_info.second;
    if (!handle_e1 || !handle_e2) MESSAGE("ETT<T, L>", UNKNOWN);
    auto rank_e1 = splay.rank(handle_e1);
    auto rank_e2 = splay.rank(handle_e2);
    if (rank_e1 > rank_e2) std::swap(rank_e1, rank_e2);
    return rank_e1 <= rs && rs <= rank_e2;
}

/*
Description:
将u和v连接起来，如果u和v本来就在一个连通块内，则忽略本次link

否则v会被当作v所在树的根，并连向u
*/
template<typename T, typename L>
auto ETT<T, L>::link(int u, int v) -> void {
    CALL(FUNCTION);
    link(u, v, T());
}

/*
Description:
将u和v连接起来，如果u和v本来就在一个连通块内，则忽略本次link.
否则v会被当作v所在树的根，并连向u.
*/
template<typename T, typename L>
auto ETT<T, L>::link(int u, int v, const T& info) -> void {
    CALL(FUNCTION);
    if (is_accessible(u, v)) return;
    auto iter_u = vmap.find(u);
    if (iter_u == vmap.end()) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto iter_v = vmap.find(v);
    if (iter_v == vmap.end()) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto handle_u = iter_u->second;
    auto handle_v = iter_v->second;
    make_root(v);
    SplayType& splay_u = eulers[dynamic_cast<SplayNodeType*>(handle_u)->query_first_info().x];
    SplayType splay_v = std::move(eulers[((SplayNodeType*)handle_v)->query_first_info().x]);
    int ru = splay_u.rank(handle_u);
    auto e1 = splay_u.insert_after(ru, MyETTInfo(info, u, v));
    auto e2 = splay_u.insert_after(ru, MyETTInfo(info, u, v));
    splay_u.insert_after(ru + 1, std::move(splay_v));
    eulers.erase(v);
    emap[make_edge(u, v)] = std::make_pair(e1, e2);
}

/*
Description:
删除u和v之间的连边，如果u和v之间本来就没有连边则会忽略本次操作.
*/
template<typename T, typename L>
auto ETT<T, L>::cut(int u, int v) -> void {
    CALL(FUNCTION);
    auto iter = emap.find(make_edge(u, v));
    if (iter == emap.end()) return;
    if (is_ancestor_of(v, u))
        std::swap(u, v);
    auto iter_u = vmap.find(u);
    auto iter_v = vmap.find(v);
    auto hu = iter_u->second;
    auto hv = iter_v->second;
    SplayType& splay = eulers[dynamic_cast<SplayNodeType*>(hv)->query_first_info().x];
    auto he1 = iter->second.first; int re1 = splay.rank(he1);
    auto he2 = iter->second.second; int re2 = splay.rank(he2);
    if (re1 > re2) {
        std::swap(re1, re2);
        std::swap(he1, he2);
    }
    auto subtree = splay.split(re1, re2);
    subtree.remove(1);
    subtree.remove(subtree.size());
    emap.erase(make_edge(u, v));
    eulers.emplace(v, std::move(subtree));
}

/*
Description:
查询以u为根的子树的信息sum，要求信息是可合并的
*/
template<typename T, typename L>
auto ETT<T, L>::query_sum(int u) -> T {
    CALL(FUNCTION);
    auto iter = vmap.find(u);
    if (iter == vmap.end()) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto handle = iter->second;
    SplayType& splay = eulers[dynamic_cast<SplayNodeType*>(handle)->query_first_info().x];
    int rank = splay.rank(handle);
    int l = rank, r;
    if (rank == 1) {
        r = splay.size();
    } else {
        MyETTInfo edge = splay.query_info(rank - 1);
        auto edge_info = emap[make_edge(edge.x, edge.y)];
        auto handle_e1 = edge_info.first;
        auto handle_e2 = edge_info.second;
        auto rank_e1 = splay.rank(handle_e1);
        auto rank_e2 = splay.rank(handle_e2);
        if (rank_e1 > rank_e2) std::swap(rank_e1, rank_e2);
        r  = rank_e2 - 1;
    }
    return splay.query_sum(l, r).info;
}

/*
Description:
查询以u的信息info.
*/
template<typename T, typename L>
auto ETT<T, L>::query_info(int u) -> T {
    auto iter = vmap.find(u);
    if (iter == vmap.end()) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto handle = iter->second;
    auto splay_node = (SplayNodeType*)(handle);
    SplayType& splay = eulers[splay_node->query_first_info().x];
    return splay.query_info(handle).info;
}

/*
Description:
对u的子树推送一个标记tag.
*/
template<typename T, typename L>
auto ETT<T, L>::insert(int u, const L& tag) -> void {
    auto iter = vmap.find(u);
    if (iter == vmap.end()) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto handle = iter->second;
    SplayType& splay = eulers[dynamic_cast<SplayNodeType*>(handle)->query_first_info().x];
    int rank = splay.rank(handle);
    int l = rank, r;
    if (rank == 1) {
        r = splay.size();
    } else {
        MyETTInfo edge = splay.query_info(rank - 1);
        auto edge_info = emap[make_edge(edge.x, edge.y)];
        auto handle_e1 = edge_info.first;
        auto handle_e2 = edge_info.second;
        auto rank_e1 = splay.rank(handle_e1);
        auto rank_e2 = splay.rank(handle_e2);
        if (rank_e1 > rank_e2) std::swap(rank_e1, rank_e2);
        r  = rank_e2 - 1;
    }
    splay.insert(l, r, MyETTTag(tag));
}

/*
Description:
新建一个点u.
如果该点已经存在，则会忽略本次操作.
*/
template<typename T, typename L>
auto ETT<T, L>::new_node(int u) -> void {
    CALL(FUNCTION);
    new_node(u, T());
}

/*
Description:
新建一个点u，u上存储的信息为info
如果该点已经存在，则会忽略本次操作
*/
template<typename T, typename L>
auto ETT<T, L>::new_node(int u, const T& info) -> void {
    CALL(FUNCTION);
    if (vmap.find(u) != vmap.end()) return;
    SplayType splay;
    auto handle = splay.insert_after(0, MyETTInfo(info, u));
    vmap[u] = handle;
    eulers.emplace(u, std::move(splay));
}

}

#endif