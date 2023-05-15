#ifndef HLD_H
#define HLD_H

#include "Tree.h"

namespace mk {

template<typename T, typename L>
class HLD {
public:
    HLD() = default;
    auto initialize(std::shared_ptr<Tree> tr) -> void;
    auto lca(int x, int y) -> int;
    auto trait(std::function<T(std::shared_ptr<Vertex>)>) -> void;
private:
    auto dfs1(int u, int f, int d) -> void;
    auto dfs2(int u, int t) -> void;
    std::shared_ptr<Tree> tr;
    std::vector<int> son;
    std::vector<int> size;
    std::vector<int> top;
    std::vector<int> fa;
    std::vector<int> dep;
    std::vector<int> dfn;
    int tot;
};

template<typename T, typename L>
auto HLD<T, L>::initialize(std::shared_ptr<Tree> tr) -> void {
    this->tr = tr;
    tr->generate(0, nullptr);
    int n = tr->get_size();
    son.resize(n); size.resize(n); fa.resize(n);
    top.resize(n); dep.resize(n); dfn.resize(n);
    tot = 0;
    dfs1(0, -1, 0);
    dfs2(0, 0);
}

template<typename T, typename L>
auto HLD<T, L>::dfs1(int u, int f, int d) -> void {
    size[u] = 1; son[u] = -1; fa[u] = f; dep[u] = d;
    for (auto v : tr->get_adjoint()) {
        if (v != f) {
            dfs1(v, u, d + 1);
            size[u] += size[v];
            if (son[u] == -1 || size[son[u]] < size[v])
                son[u] = v;
        }
    }
}

template<typename T, typename L>
auto HLD<T, L>::dfs2(int u, int t) -> void {
    top[u] = t; dfn[u] = tot++;
    if (son[u]) dfs2(son[u], t);
    for (auto v : tr->get_adjoint(u)) {
        if (v != fa[u] && v != son[u])
            dfs2(v, v);
    }
}

template<typename T, typename L>
auto HLD<T, L>::lca(int x, int y) -> int {
    int fx = top[x], fy = top[y];
    while (fx != fy) {
        if (dep[fx] < dep[fy]) std::swap(fx, fy), std::swap(x, y);
        x = fa[fx]; fx = top[x];
    }
    return dep[x] < dep[y] ? x : y;
}

}

#endif