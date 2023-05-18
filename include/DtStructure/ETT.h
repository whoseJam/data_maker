#ifndef ETT_H
#define ETT_H

#include <memory>
#include <type_traits>
#include <utility>
#include <functional>

#include "Debug.h"
#include "Edge.h"
#include "Integer.h"
#include "Logger.h"
#include "Splay.h"
#include "BareTree.h"
#include "Dtstructure.h"

namespace mk {

struct ETTHandle : public Handle {
    ETTHandle();
    virtual auto edge_size() -> int = 0;
    virtual auto vertex_size() -> int = 0;
};

namespace Hidden {
struct ETTEdge : public Comparable {
    int x, y;
    std::shared_ptr<SplayHandle> handle1;
    std::shared_ptr<SplayHandle> handle2;
    ETTEdge(int x, int y) : x(std::min(x, y)), y(std::max(x, y)) {}
    ETTEdge(int x, int y, std::shared_ptr<SplayHandle> handle1, std::shared_ptr<SplayHandle> handle2) : 
        x(std::min(x, y)), y(std::max(x, y)), handle1(std::move(handle1)), handle2(std::move(handle2)) {}
    auto compare_to(std::shared_ptr<Comparable> o) -> int override {
        CALL(FUNCTION);
        auto other = std::dynamic_pointer_cast<ETTEdge>(o);
        if (!other) MESSAGE("ETTEdge", UNKNOWN);
        if (x != other->x) return x - other->x;
        return y - other->y;
    }
};

struct ETTVertex : public Comparable {
    int x;
    std::shared_ptr<SplayHandle> handle;
    ETTVertex(int x) : x(x) {}
    ETTVertex(int x, std::shared_ptr<SplayHandle> handle) : x(x), handle(std::move(handle)) {}
    auto compare_to(std::shared_ptr<Comparable> o) -> int override {
        CALL(FUNCTION);
        auto other = std::dynamic_pointer_cast<ETTVertex>(o);
        if (!other) MESSAGE("ETTVertex", UNKNOWN);
        return x - other->x;
    }
};

template<typename L, bool CHECKER = std::is_base_of_v<Lazytag, L>>
struct ETTTag;
template<typename L>
struct ETTTag<L, false> : public Lazytag {
    bool rev;
    ETTTag() : rev(false) {}
    ETTTag(bool r) : rev(r) {}
    auto push(std::shared_ptr<Lazytag> o, std::shared_ptr<Handle> h) -> void override {
        CALL(FUNCTION);
        auto other = std::dynamic_pointer_cast<ETTTag>(o);
        other->rev ^= rev;
        if (rev) {
            auto splay_handle = std::dynamic_pointer_cast<SplayHandle>(h);
            if (!splay_handle) MESSAGE("ETTTag<L, false>", UNKNOWN);
            splay_handle->swap_child();
        }
    }
    auto push(std::shared_ptr<Info> o, std::shared_ptr<Handle> h) -> void override { CALL(FUNCTION); }
};
template<typename L>
struct ETTTag<L, true> : public Lazytag {
    std::shared_ptr<L> tag;
    bool rev;
    ETTTag() : rev(false) {}
    ETTTag(std::shared_ptr<L> tag) : tag(std::move(tag)) {}
    ETTTag(bool r) : rev(r) {}
    auto push(std::shared_ptr<Lazytag> o, std::shared_ptr<Handle> h) -> void override {
        CALL(FUNCTION);
        auto other = std::dynamic_pointer_cast<ETTTag>(o);
        other->rev ^= rev;
        if (rev) {
            auto splay_handle = std::dynamic_pointer_cast<SplayHandle>(h);
            if (!splay_handle) MESSAGE("ETTTag<L, true>", UNKNOWN);
            splay_handle->swap_child();
        }
        tag->push(o, h);
    }
    void push(std::shared_ptr<Info> o, std::shared_ptr<Handle> h) { CALL(FUNCTION); tag->push(o, h); }
};


template<typename T, bool CHECKER = std::is_base_of_v<Mergeable, T>>
struct ETTInfo;
template<typename T>
struct ETTInfo<T, true> : public Mergeable {
    int x, y;
    std::shared_ptr<T> info;
    ETTInfo() : x(-1), y(-1), info(std::make_shared<T>()) { CALL(FUNCTION); }
    ETTInfo(std::shared_ptr<T> info, int x) : info(std::move(info)), x(x), y(-1) { CALL(FUNCTION); }
    ETTInfo(std::shared_ptr<T> info, int x, int y) : info(std::move(info)), x(x), y(y) { CALL(FUNCTION); }
    virtual auto merge(std::shared_ptr<Mergeable> o) -> void override {
        CALL(FUNCTION);
        auto other = std::dynamic_pointer_cast<ETTInfo<T, true>>(o);
        x = other->x;
        y = other->y;
        if (other->info) {
            if (!info) info = std::make_shared<T>(); 
            info->merge(other->info);
        }
    }
};
template<typename T> 
struct ETTInfo<T, false> : public Info {
    int x, y;
    ETTInfo() : x(-1), y(-1) { CALL(FUNCTION); }
    ETTInfo(std::shared_ptr<T> info, int x) : info(std::move(info)), x(x), y(-1) { CALL(FUNCTION); }
    ETTInfo(std::shared_ptr<T> info, int x, int y) : info(std::move(info)), x(x), y(y) { CALL(FUNCTION); }
    std::shared_ptr<T> info;
};

}

template<typename T, typename L = bool>
class ETT {
public:
    ETT();
    auto initialize(std::shared_ptr<BareTree<T>> tr) -> void;
    auto is_accessible(int u, int v) -> bool;
    auto is_ancestor_of(int a, int s) -> bool;
    auto make_root(int u) -> void;
    auto link(int u, int v) -> void;
    auto link(int u, int v, std::shared_ptr<T> info) -> void;
    auto cut(int u, int v) -> void;
    auto query_sum(int u) -> std::shared_ptr<T>;
    auto query_info(int u) -> std::shared_ptr<T>;
    auto insert(int u, L tag) -> void;
    auto insert(int u, std::shared_ptr<L> tag) -> void;
    auto new_node(int u) -> void;
    auto new_node(int u, T info) -> void;
    auto new_node(int u, std::shared_ptr<T>) -> void;
private:
    auto dfs(int u, int f, int rt) -> void;

    using MyETTInfo = Hidden::ETTInfo<T>;
    using MyETTTag = Hidden::ETTTag<L>;
    using MyETTEdge = Hidden::ETTEdge;
    using MyETTVertex = Hidden::ETTVertex;
    using SplayType = Splay<MyETTInfo, MyETTTag>;
    using SplayNodeType = typename Hidden::SplayNode<MyETTInfo, MyETTTag>;
    std::map<int, std::shared_ptr<SplayType>> eulers;
    std::shared_ptr<Splay<MyETTEdge>> emap;
    std::shared_ptr<Splay<MyETTVertex>> vmap;
};

template<typename T, typename L>
ETT<T, L>::ETT() {
    emap = std::make_shared<Splay<MyETTEdge>>();
    vmap = std::make_shared<Splay<MyETTVertex>>();
    eulers.clear();
}

// template<typename T, typename L>
// auto ETT<T, L>::dfs(int u, int f, int rt) -> void {
//     auto& splay = eulers[rt];
//     auto h = splay->insert_after(splay->size(), tree->vinfo(u));
//     vmap->insert(ETTVertex(u, h));
//     for (auto v : tree.adjoint(u)) {
//         auto h1 = eulers[rt]->insert_after(splay->size(), 
//             std::make_shared<ETTInfo<T>>(tree->einfo(u, v), u, v));
//         dfs(v, u, rt);
//         auto h2 = splay->insert_after(splay->size(), 
//             std::make_shared<ETTInfo<T>>(tree->einfo(u, v), u, v));
//         emap->insert(ETTEdge(u, v, h1, h2));
//     }
// }

template<typename T, typename L>
auto ETT<T, L>::initialize(std::shared_ptr<BareTree<T>> tr) -> void {
    CALL(FUNCTION);
    this->tree = tr;
    int rt = tr->root();
    eulers[rt] = std::make_shared<SplayType>();
    dfs(rt, -1, rt);
}

/*
Description:
将u作为u所在的树的根.
如果u不存在则会报错.
*/
template<typename T, typename L>
auto ETT<T, L>::make_root(int u) -> void {
    CALL(FUNCTION);
    using std::cout;
    auto vertex_u = vmap->query_info(MyETTVertex(u));
    if (!vertex_u) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto handle_u = vertex_u->handle;
    std::shared_ptr<SplayNodeType> spl_node_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u);
    std::shared_ptr<SplayType> splay = spl_node_u->belong();
    int rank_u = splay->rank(handle_u);
    if (rank_u == 1) return;
    int last_root = splay->query_info(1)->x;
    std::shared_ptr<MyETTInfo> edge = splay->query_info(rank_u - 1);
    int x = edge->x, y= edge->y;
    auto edge_info = emap->query_info(MyETTEdge(x, y));
    auto handle_e1 = edge_info->handle1;
    auto handle_e2 = edge_info->handle2; 
    int r1 = splay->rank(handle_e1);
    int r2 = splay->rank(handle_e2);
    if (r1 > r2) {
        std::swap(r1, r2);
        std::swap(handle_e1, handle_e2);
    }
    int n = splay->size();
    int a = r1;
    int b = r2 - 1;
    std::shared_ptr<SplayType> bef = 1 <= a ? splay->split(1, a) : std::make_shared<SplayType>();
    std::shared_ptr<SplayType> mid = 1 <= b - a ? splay->split(1, b - a) : std::make_shared<SplayType>();
    std::shared_ptr<SplayType> aft = 1 <= n - b ? splay->split(1, n - b) : std::make_shared<SplayType>();
    if (1 <= a) bef->insert(1, a, MyETTTag(true));
    if (1 <= n - b) aft->insert(1, n - b, MyETTTag(true));
    splay->insert_after(splay->size(), mid);
    splay->insert_after(splay->size(), bef);
    splay->insert_after(splay->size(), aft);
    eulers[last_root] = nullptr;
    eulers[u] = splay;
}

/*
Description:
判断u和v是否在同一棵树中.
如果u或者v不存在，则视为u和v不在同一棵树中.
*/
template<typename T, typename L>
auto ETT<T, L>::is_accessible(int u, int v) -> bool {
    CALL(FUNCTION);
    auto vertex_u_info = vmap->query_info(MyETTVertex(u));
    auto vertex_v_info = vmap->query_info(MyETTVertex(v));
    if (!vertex_u_info || !vertex_v_info) return false;
    auto handle_u = vertex_u_info->handle;
    auto handle_v = vertex_v_info->handle;
    std::shared_ptr<SplayType> splay_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u)->belong();
    std::shared_ptr<SplayType> splay_v = std::dynamic_pointer_cast<SplayNodeType>(handle_v)->belong();
    return (splay_u == splay_v);
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
    auto vertex_a_info = vmap->query_info(MyETTVertex(a));
    auto vertex_s_info = vmap->query_info(MyETTVertex(s));
    if (!vertex_a_info || !vertex_s_info) return false;
    auto handle_a = vertex_a_info->handle;
    auto handle_s = vertex_s_info->handle;
    auto splay = std::dynamic_pointer_cast<SplayNodeType>(handle_a)->belong();
    int ra = splay->rank(handle_a);
    int rs = splay->rank(handle_s);
    if (ra == 1) return true;
    std::shared_ptr<MyETTInfo> edge = splay->query_info(ra - 1);
    auto edge_info = emap->query_info(MyETTEdge(edge->x, edge->y));
    auto handle_e1 = edge_info->handle1;
    auto handle_e2 = edge_info->handle2;
    auto rank_e1 = splay->rank(handle_e1);
    auto rank_e2 = splay->rank(handle_e2);
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
    link(u, v, nullptr);
}

/*
Description:
将u和v连接起来，如果u和v本来就在一个连通块内，则忽略本次link
否则v会被当作v所在树的根，并连向u
*/
template<typename T, typename L>
auto ETT<T, L>::link(int u, int v, std::shared_ptr<T> info) -> void {
    CALL(FUNCTION);
    if (is_accessible(u, v)) return;
    if (info == nullptr) info = std::make_shared<T>();
    auto vertex_u_info = vmap->query_info(MyETTVertex(u));
    auto vertex_v_info = vmap->query_info(MyETTVertex(v));
    if (!vertex_u_info || !vertex_v_info) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    make_root(v);
    auto handle_u = vertex_u_info->handle;
    auto handle_v = vertex_v_info->handle;
    auto spl_node_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u);
    auto spl_node_v = std::dynamic_pointer_cast<SplayNodeType>(handle_v);
    auto splay_u = spl_node_u->belong();
    auto splay_v = spl_node_v->belong();
    int ru = splay_u->rank(handle_u);
    auto e1 = splay_u->insert_after(ru, MyETTInfo(info, u, v));
    auto e2 = splay_u->insert_after(ru, MyETTInfo(info, u, v));
    splay_u->insert_after(ru + 1, splay_v);
    emap->insert(MyETTEdge(u, v, e1, e2));
}

/*
Description:
删除u和v之间的连边，如果u和v之间本来就没有连边则会忽略本次操作
*/
template<typename T, typename L>
auto ETT<T, L>::cut(int u, int v) -> void {
    CALL(FUNCTION);
    auto qe = emap->query_info(MyETTEdge(u, v));
    if (!qe) return;
    if (is_ancestor_of(v, u))
        std::swap(u, v);
    auto qu = vmap->query_info(MyETTVertex(u));
    auto qv = vmap->query_info(MyETTVertex(v));
    auto hu = qu->handle;
    auto hv = qv->handle;
    auto splay = std::dynamic_pointer_cast<SplayNodeType>(hu)->belong();
    auto he1 = qe->handle1; int re1 = splay->rank(he1);
    auto he2 = qe->handle2; int re2 = splay->rank(he2);
    if (re1 > re2) {
        std::swap(re1, re2);
        std::swap(he1, he2);
    }
    auto subtree = splay->split(re1, re2);
    subtree->remove(1);
    subtree->remove(subtree->size());
    emap->remove(MyETTEdge(u, v));
    eulers[v] = subtree;
}

/*
Description:
查询以u为根的子树的信息sum，要求信息是可合并的
*/
template<typename T, typename L>
auto ETT<T, L>::query_sum(int u) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    auto vertex_info = vmap->query_info(MyETTVertex(u));
    if (!vertex_info) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto handle_u = vertex_info->handle;
    auto spl_node_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u);
    auto splay = spl_node_u->belong();
    int rank_u = splay->rank(handle_u);
    int l = rank_u, r;
    if (rank_u == 1) {
        r = splay->size();
    } else {
        std::shared_ptr<MyETTInfo> edge = splay->query_info(rank_u - 1);
        auto edge_info = emap->query_info(MyETTEdge(edge->x, edge->y));
        auto handle_e1 = edge_info->handle1;
        auto handle_e2 = edge_info->handle2;
        auto rank_e1 = splay->rank(handle_e1);
        auto rank_e2 = splay->rank(handle_e2);
        if (rank_e1 > rank_e2) std::swap(rank_e1, rank_e2);
        r  = rank_e2 - 1;
    }
    return splay->query_sum(l, r)->info;
}

template<typename T, typename L>
auto ETT<T, L>::query_info(int u) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    auto vertex_info = vmap->query_info(MyETTVertex(u));
    if (!vertex_info) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto handle_u = vertex_info->handle;
    auto spl_node_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u);
    auto splay = spl_node_u->belong();
    return splay->query_info(handle_u)->info;
}

/*
Description:
对u的子树推送一个标记tag
*/
template<typename T, typename L>
auto ETT<T, L>::insert(int u, L tag) -> void {
    CALL(FUNCTION);
    insert(u, std::make_shared<L>(std::move(tag)));
}
/*
Description:
对u的子树推送一个标记tag
*/
template<typename T, typename L>
auto ETT<T, L>::insert(int u, std::shared_ptr<L> tag) -> void {
    CALL(FUNCTION);
    auto vertex_info = vmap->query_info(MyETTVertex(u));
    if (!vertex_info) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto handle_u = vertex_info->handle;
    auto spl_node_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u);
    auto splay = spl_node_u->belong();
    int rank_u = splay->rank(handle_u);
    int l = rank_u, r;
    if (rank_u == 1) {
        r = splay->size();
    } else {
        std::shared_ptr<MyETTInfo> edge = splay->query(rank_u - 1);
        auto edge_info = emap->query_info(MyETTEdge(edge->x, edge->y));
        auto handle_e1 = edge_info->handle1;
        auto handle_e2 = edge_info->handle2;
        auto rank_e1 = splay->rank(handle_e1);
        auto rank_e2 = splay->rank(handle_e2);
        if (rank_e1 > rank_e2) std::swap(rank_e1, rank_e2);
        r  = rank_e2 - 1;
    }
    return splay->insert(l, r, tag);
}

/*
Description:
新建一个点u
如果该点已经存在，则会忽略本次操作
*/
template<typename T, typename L>
auto ETT<T, L>::new_node(int u) -> void {
    CALL(FUNCTION);
    new_node(u, nullptr);
}

/*
Description:
新建一个点u，u上存储的信息为info
如果该点已经存在，则会忽略本次操作
*/
template<typename T, typename L>
auto ETT<T, L>::new_node(int u, T info) -> void {
    CALL(FUNCTION);
    new_node(u, std::make_shared<T>(std::move(info)));
}

/*
Description:
新建一个点u，u上存储的信息为info
如果该点已经存在，则会忽略本次操作
*/
template<typename T, typename L>
auto ETT<T, L>::new_node(int u, std::shared_ptr<T> info) -> void {
    CALL(FUNCTION);
    auto qu = vmap->query_info(MyETTVertex(u));
    if (qu) return;
    auto splay_u = std::make_shared<SplayType>();
    auto handle_u = splay_u->insert_after(0, MyETTInfo(info, u));
    vmap->insert(MyETTVertex(u, handle_u));
    eulers[u] = splay_u;
}

}

#endif