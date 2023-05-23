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

struct ETTEdgeHandle {
    ETTEdgeHandle() = default;
    ETTEdgeHandle(std::shared_ptr<SplayHandle> h1, std::shared_ptr<SplayHandle> h2) : 
        handle1(h1), handle2(h2) {}
    std::shared_ptr<SplayHandle> handle1;
    std::shared_ptr<SplayHandle> handle2;
};

struct ETTEdge {
    int x, y;
    ETTEdge(int x, int y) : x(std::min(x, y)), y(std::max(x, y)) {}
    bool operator <(const ETTEdge& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator <(ETTEdge&& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

struct ETTVertexHandle {
    ETTVertexHandle() = default;
    ETTVertexHandle(std::shared_ptr<SplayHandle> h) : 
        handle(h) {}
    std::shared_ptr<SplayHandle> handle;
};

struct ETTVertex {
    int x;
    ETTVertex(int x) : x(x) {}
    ETTVertex(int x, std::shared_ptr<SplayHandle> handle) : x(x) {}
    bool operator <(const ETTVertex& other) const {
        return x < other.x;
    }
    bool operator <(ETTVertex&& other) const {
        return x < other.x;
    }
};

template<typename L, bool CHECKER = std::is_base_of_v<Lazytag, L>>
struct ETTTag;
template<typename L>
struct ETTTag<L, false> : public Lazytag {
    bool rev;
    ETTTag() : rev(false) { COUNT_ETTTAG++; }
    ETTTag(bool r) : rev(r) { COUNT_ETTTAG++; }
    ETTTag(const ETTTag& other) : rev(other.rev) { COUNT_ETTTAG++; }
    ~ETTTag() { COUNT_ETTTAG--; }
    virtual auto push(std::shared_ptr<Lazytag> tag, std::shared_ptr<Handle> handle) -> void override {
        CALL(FUNCTION);
        auto ett_tag = std::dynamic_pointer_cast<ETTTag>(tag);
        if (ett_tag) {
            rev ^= ett_tag->rev;
            if (ett_tag->rev) {
                auto splay = std::dynamic_pointer_cast<SplayHandle>(handle);
                splay->swap_child();
            }
        }
    }
};
template<typename L>
struct ETTTag<L, true> : public Lazytag {
    std::shared_ptr<L> tag;
    bool rev;
    ETTTag() : rev(false), tag(std::make_shared<L>()) { COUNT_ETTTAG++; }
    ETTTag(std::shared_ptr<L> tag) : tag(std::move(tag)), rev(0) { COUNT_ETTTAG++; }
    ETTTag(bool r) : rev(r) { tag = std::make_shared<L>(); COUNT_ETTTAG++; }
    ETTTag(const ETTTag& other) : tag(std::move(other.tag)), rev(other.rev) { COUNT_ETTTAG++; }
    ~ETTTag() { COUNT_ETTTAG--; }
    virtual auto push(std::shared_ptr<Lazytag> tag, std::shared_ptr<Handle> handle) -> void override {
        CALL(FUNCTION);
        auto ett_tag = std::dynamic_pointer_cast<ETTTag>(tag);
        if (ett_tag) {
            rev ^= ett_tag->rev;
            if (ett_tag->rev) {
                auto splay = std::dynamic_pointer_cast<SplayHandle>(handle);
                splay->swap_child();
            }
            if (ett_tag->tag) {
                if (!this->tag) this->tag = std::make_shared<L>();
                this->tag->push(ett_tag->tag, handle);
            }
        }
    }
};


template<typename T, typename L, 
    bool MCHECKER = std::is_base_of_v<Mergeable, T>, 
    bool PCHECKER = std::is_base_of_v<Pushable, T> && std::is_base_of_v<Lazytag, L>>
struct ETTInfo;
template<typename T, typename L>
struct ETTInfo<T, L, true, true> : 
    public Mergeable,
    public Pushable,
    public ETTHandle,
    public std::enable_shared_from_this<ETTInfo<T, L>> {
    int x, y, ne, nv;
    std::shared_ptr<T> info;
    ETTInfo() : x(-1), y(-1), info(std::make_shared<T>()), nv(0), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(std::shared_ptr<T> info, int x) : info(std::move(info)), x(x), y(-1), nv(1), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(std::shared_ptr<T> info, int x, int y) : info(std::move(info)), x(x), y(y), nv(0), ne(1) { COUNT_ETTINFO++; }
    ETTInfo(const ETTInfo& other) : x(other.x), y(other.y), ne(other.ne), nv(other.nv), info(other.info) { COUNT_ETTINFO++; } 
    ~ETTInfo() { COUNT_ETTINFO--; }
    virtual auto edge_size() -> int override { return ne; }
    virtual auto vertex_size() -> int override { return nv; }
    virtual auto merge(std::shared_ptr<Mergeable> o) -> void override {
        CALL(FUNCTION);
        auto other = std::dynamic_pointer_cast<ETTInfo<T, L>>(o);
        x = other->x;
        y = other->y;
        ne += other->ne;
        nv += other->nv;
        if (other->info) {
            if (!info) info = std::make_shared<T>(); 
            info->merge(other->info);
        }
    }
    virtual auto push(std::shared_ptr<Lazytag> tag, std::shared_ptr<Handle> handle) -> void override {
        CALL(FUNCTION);
        auto ett_tag = std::dynamic_pointer_cast<ETTTag<L>>(tag);
        info->push(ett_tag->tag, this->shared_from_this());
    }
};
template<typename T, typename L>
struct ETTInfo<T, L, true, false> : 
    public Mergeable,
    public ETTHandle {
    int x, y, ne, nv;
    std::shared_ptr<T> info;
    ETTInfo() : x(-1), y(-1), info(std::make_shared<T>()), nv(0), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(std::shared_ptr<T> info, int x) : info(std::move(info)), x(x), y(-1), nv(1), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(std::shared_ptr<T> info, int x, int y) : info(std::move(info)), x(x), y(y), nv(0), ne(1) { COUNT_ETTINFO++; }
    ETTInfo(const ETTInfo& other) : x(other.x), y(other.y), ne(other.ne), nv(other.nv), info(other.info) { COUNT_ETTINFO++; } 
    ~ETTInfo() { COUNT_ETTINFO--; }
    virtual auto edge_size() -> int override { return ne; }
    virtual auto vertex_size() -> int override { return nv; }
    virtual auto merge(std::shared_ptr<Mergeable> o) -> void override {
        CALL(FUNCTION);
        auto other = std::dynamic_pointer_cast<ETTInfo<T, L>>(o);
        x = other->x;
        y = other->y;
        ne += other->ne;
        nv += other->nv;
        if (other->info) {
            if (!info) info = std::make_shared<T>(); 
            info->merge(other->info);
        }
    }
};

template<typename T, typename L>
struct ETTInfo<T, L, false, true> : 
    public Mergeable,
    public Pushable,
    public ETTHandle,
    public std::enable_shared_from_this<ETTInfo<T, L>>  {
    int x, y, ne, nv;
    std::shared_ptr<T> info;
    ETTInfo() : x(-1), y(-1), info(std::make_shared<T>()), nv(0), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(std::shared_ptr<T> info, int x) : info(std::move(info)), x(x), y(-1), nv(1), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(std::shared_ptr<T> info, int x, int y) : info(std::move(info)), x(x), y(y), nv(0), ne(1) { COUNT_ETTINFO++; }
    ETTInfo(const ETTInfo& other) : x(other.x), y(other.y), nv(other.nv), ne(other.ne), info(other.info) { COUNT_ETTINFO++; } 
    ~ETTInfo() { COUNT_ETTINFO--; }
    virtual auto edge_size() -> int override { return ne; }
    virtual auto vertex_size() -> int override { return nv; }
    virtual auto merge(std::shared_ptr<Mergeable> o) -> void override {
        CALL(FUNCTION);
        auto other = std::dynamic_pointer_cast<ETTInfo<T, L>>(o);
        x = other->x;
        y = other->y;
        ne += other->ne;
        nv += other->nv;
    }
    virtual auto push(std::shared_ptr<Lazytag> tag, std::shared_ptr<Handle> handle) -> void override {
        CALL(FUNCTION);
        auto ett_tag = std::dynamic_pointer_cast<ETTTag<L, true>>(tag);
        info->push(ett_tag->tag, this->shared_from_this());
    }
};

template<typename T, typename L>
struct ETTInfo<T, L, false, false> : 
    public Mergeable,
    public ETTHandle {
    int x, y, ne, nv;
    std::shared_ptr<T> info;
    ETTInfo() : x(-1), y(-1), info(std::make_shared<T>()), nv(0), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(std::shared_ptr<T> info, int x) : info(std::move(info)), x(x), y(-1), nv(1), ne(0) { COUNT_ETTINFO++; }
    ETTInfo(std::shared_ptr<T> info, int x, int y) : info(std::move(info)), x(x), y(y), nv(0), ne(1) { COUNT_ETTINFO++; }
    ETTInfo(const ETTInfo& other) : x(other.x), y(other.y), ne(other.ne), nv(other.nv), info(other.info) { COUNT_ETTINFO++; } 
    ~ETTInfo() { COUNT_ETTINFO--; }
    virtual auto edge_size() -> int override { return ne; }
    virtual auto vertex_size() -> int override { return nv; }
    virtual auto merge(std::shared_ptr<Mergeable> o) -> void override {
        CALL(FUNCTION);
        auto other = std::dynamic_pointer_cast<ETTInfo<T, L>>(o);
        x = other->x;
        y = other->y;
        ne += other->ne;
        nv += other->nv;
    }
};

}

template<typename T, typename L = bool>
class ETT {
public:
    ETT();
    ~ETT() { CALL(FUNCTION); COUNT_ETT--; }
    auto is_accessible(int u, int v) -> bool;
    auto is_ancestor_of(int a, int s) -> bool;
    auto is_root(int u) -> bool;
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
    auto clear() -> void;
private:
    using MyETTInfo = Hidden::ETTInfo<T, L>;
    using MyETTTag = Hidden::ETTTag<L>;
    using MyETTEdge = Hidden::ETTEdge;
    using MyETTEdgeHandle = Hidden::ETTEdgeHandle;
    using MyETTVertexHandle = Hidden::ETTVertexHandle;
    using MyETTVertex = Hidden::ETTVertex;
    using SplayType = Splay<MyETTInfo, MyETTTag>;
    using SplayNodeType = typename Hidden::SplayNode<MyETTInfo, MyETTTag>;
    std::map<int, std::shared_ptr<SplayType>> eulers;
    std::map<MyETTEdge, MyETTEdgeHandle> emap;
    std::map<MyETTVertex, MyETTVertexHandle> vmap;
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
    auto vertex_u = vmap[MyETTVertex(u)];
    auto handle_u = vertex_u.handle;
    if (!handle_u) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    std::shared_ptr<SplayNodeType> spl_node_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u);
    std::shared_ptr<SplayType> splay = spl_node_u->belong();
    int rank_u = splay->rank(handle_u);
    if (rank_u == 1) return;
    int last_root = splay->query_info(1)->x;
    std::shared_ptr<MyETTInfo> edge = splay->query_info(rank_u - 1);
    if (!edge) MESSAGE("ETT<T, L>", UNKNOWN);
    auto edge_info = emap[MyETTEdge(edge->x, edge->y)];
    auto handle_e1 = edge_info.handle1;
    auto handle_e2 = edge_info.handle2; 
    if (!handle_e1 || !handle_e2) MESSAGE("ETT<T, L>", UNKNOWN);
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
    auto vertex_u_info = vmap[MyETTVertex(u)];
    auto vertex_v_info = vmap[MyETTVertex(v)];
    auto handle_u = vertex_u_info.handle;
    auto handle_v = vertex_v_info.handle;
    if (!handle_u || !handle_v) return false;
    std::shared_ptr<SplayType> splay_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u)->belong();
    std::shared_ptr<SplayType> splay_v = std::dynamic_pointer_cast<SplayNodeType>(handle_v)->belong();
    return (splay_u == splay_v);
}

template<typename T, typename L>
auto ETT<T, L>::is_root(int u) -> bool {
    CALL(FUNCTION);
    auto vertex_info = vmap[MyETTVertex(u)];
    auto handle = vertex_info.handle;
    if (!handle) return false;
    auto splay = std::dynamic_pointer_cast<SplayNodeType>(handle)->belong();
    return splay->rank(handle) == 1;
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
    auto vertex_a_info = vmap[MyETTVertex(a)];
    auto vertex_s_info = vmap[MyETTVertex(s)];
    auto handle_a = vertex_a_info.handle;
    auto handle_s = vertex_s_info.handle;
    if (!handle_a || !handle_s) return false;
    auto splay = std::dynamic_pointer_cast<SplayNodeType>(handle_a)->belong();
    int ra = splay->rank(handle_a);
    int rs = splay->rank(handle_s);
    if (ra == 1) return true;
    std::shared_ptr<MyETTInfo> edge = splay->query_info(ra - 1);
    if (!edge) MESSAGE("ETT<T, L>", UNKNOWN);
    auto edge_info = emap[MyETTEdge(edge->x, edge->y)];
    auto handle_e1 = edge_info.handle1;
    auto handle_e2 = edge_info.handle2;
    if (!handle_e1 || !handle_e2) MESSAGE("ETT<T, L>", UNKNOWN);
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
将u和v连接起来，如果u和v本来就在一个连通块内，则忽略本次link.
否则v会被当作v所在树的根，并连向u.
*/
template<typename T, typename L>
auto ETT<T, L>::link(int u, int v, std::shared_ptr<T> info) -> void {
    CALL(FUNCTION);
    if (is_accessible(u, v)) return;
    if (info == nullptr) info = std::make_shared<T>();
    auto vertex_u_info = vmap[MyETTVertex(u)];
    auto vertex_v_info = vmap[MyETTVertex(v)];
    make_root(v);
    auto handle_u = vertex_u_info.handle;
    auto handle_v = vertex_v_info.handle;
    if (!handle_u || !handle_v) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto spl_node_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u);
    auto spl_node_v = std::dynamic_pointer_cast<SplayNodeType>(handle_v);
    auto splay_u = spl_node_u->belong();
    auto splay_v = spl_node_v->belong();
    
    int ru = splay_u->rank(handle_u);
    auto e1 = splay_u->insert_after(ru, MyETTInfo(info, u, v));
    auto e2 = splay_u->insert_after(ru, MyETTInfo(info, u, v));
    splay_u->insert_after(ru + 1, splay_v);
    eulers.erase(v);
    emap[MyETTEdge(u, v)] = MyETTEdgeHandle(e1, e2);
}

/*
Description:
删除u和v之间的连边，如果u和v之间本来就没有连边则会忽略本次操作.
*/
template<typename T, typename L>
auto ETT<T, L>::cut(int u, int v) -> void {
    CALL(FUNCTION);
    auto qe = emap[MyETTEdge(u, v)];
    if (!qe.handle1 || !qe.handle2) return;
    if (is_ancestor_of(v, u))
        std::swap(u, v);
    
    auto qu = vmap[MyETTVertex(u)];
    auto qv = vmap[MyETTVertex(v)];
    auto hu = qu.handle;
    auto hv = qv.handle;
    auto splay = std::dynamic_pointer_cast<SplayNodeType>(hv)->belong();
    auto he1 = qe.handle1; int re1 = splay->rank(he1);
    auto he2 = qe.handle2; int re2 = splay->rank(he2);
    if (re1 > re2) {
        std::swap(re1, re2);
        std::swap(he1, he2);
    }
    auto subtree = splay->split(re1, re2);
    subtree->remove(1);
    subtree->remove(subtree->size());
    emap.erase(MyETTEdge(u, v));
    eulers[v] = subtree;
}

/*
Description:
查询以u为根的子树的信息sum，要求信息是可合并的
*/
template<typename T, typename L>
auto ETT<T, L>::query_sum(int u) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    auto vertex_info = vmap[MyETTVertex(u)];
    auto handle = vertex_info.handle;
    if (!handle) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto spl_node = std::dynamic_pointer_cast<SplayNodeType>(handle);
    auto splay = spl_node->belong();
    int rank = splay->rank(handle);
    int l = rank, r;
    if (rank == 1) {
        r = splay->size();
    } else {
        std::shared_ptr<MyETTInfo> edge = splay->query_info(rank - 1);
        auto edge_info = emap[MyETTEdge(edge->x, edge->y)];
        auto handle_e1 = edge_info.handle1;
        auto handle_e2 = edge_info.handle2;
        auto rank_e1 = splay->rank(handle_e1);
        auto rank_e2 = splay->rank(handle_e2);
        if (rank_e1 > rank_e2) std::swap(rank_e1, rank_e2);
        r  = rank_e2 - 1;
    }
    return splay->query_sum(l, r)->info;
}

/*
Description:
查询以u的信息info.
*/
template<typename T, typename L>
auto ETT<T, L>::query_info(int u) -> std::shared_ptr<T> {
    CALL(FUNCTION);
    auto vertex_info = vmap[MyETTVertex(u)];
    auto handle_u = vertex_info.handle;
    if (!handle_u) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
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
    auto vertex_info = vmap[MyETTVertex(u)];
    auto handle_u = vertex_info.handle;
    if (!handle_u) MESSAGE("ETT<T, L>", VERTEX_NOT_FOUND);
    auto spl_node_u = std::dynamic_pointer_cast<SplayNodeType>(handle_u);
    auto splay = spl_node_u->belong();
    int rank_u = splay->rank(handle_u);
    int l = rank_u, r;
    if (rank_u == 1) {
        r = splay->size();
    } else {
        std::shared_ptr<MyETTInfo> edge = splay->query_info(rank_u - 1);
        auto edge_info = emap[MyETTEdge(edge->x, edge->y)];
        auto handle_e1 = edge_info.handle1;
        auto handle_e2 = edge_info.handle2;
        auto rank_e1 = splay->rank(handle_e1);
        auto rank_e2 = splay->rank(handle_e2);
        if (rank_e1 > rank_e2) std::swap(rank_e1, rank_e2);
        r  = rank_e2 - 1;
    }
    return splay->insert(l, r, MyETTTag(tag));
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
    auto qu = vmap[MyETTVertex(u)];
    if (qu.handle) return;
    auto splay = std::make_shared<SplayType>();
    // auto infofo = std::make_shared<MyETTInfo>(info, u);
    auto handle = splay->insert_after(0, MyETTInfo(info, u));
    vmap[MyETTVertex(u)] = MyETTVertexHandle(handle);
    eulers[u] = splay;
}

}

#endif