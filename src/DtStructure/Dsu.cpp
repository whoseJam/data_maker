
#include "Dsu.h"
#include "Logger.h"

using namespace std;

Dsu::Dsu() {
    solved = false;
}

shared_ptr<Dsu> Dsu::bind(shared_ptr<Tree> tr) {
    CALL("Dsu", "bind");
    this->tr = tr;
    return dynamic_pointer_cast<Dsu>(shared_from_this());
}

shared_ptr<Dsu> Dsu::on_dfs_insert(function<void(shared_ptr<Vertex>)> callback) {
    CALL("Dsu", "on_dfs_insert");
    callback_on_insert = callback;
    return shared_from_this();
}

shared_ptr<Dsu> Dsu::on_dfs_remove(function<void(shared_ptr<Vertex>)> callback) {
    CALL("Dsu", "on_dfs_remove");
    callback_on_remove = callback;
    return dynamic_pointer_cast<Dsu>(shared_from_this());
}

shared_ptr<Dsu> Dsu::on_dfs_query(function<int()> callback) {
    CALL("Dsu", "on_dfs_query");
    callback_on_query = callback;
    return dynamic_pointer_cast<Dsu>(shared_from_this());
}

int Dsu::query(int x) {
    CALL("Dsu", "query");
    if (!tr) MESSAGE("Dsu", NEED("bind"));
    ensure_solved();
    if (x < 0 || x >= tr->get_size()) MESSAGE("Dsu", INDEX_OUT_OF_BOUNDARY);
    return ans[x];
}

int Dsu::get_size(int x) {
    CALL("Dsu", "get_size");
    if (!tr) MESSAGE("Dsu", NEED("bind"));
    ensure_solved();
    if (x < 0 || x >= tr->get_size()) MESSAGE("Dsu", INDEX_OUT_OF_BOUNDARY);
    return size[x];
}

void Dsu::ensure_solved() {
    CALL("Dsu", "ensure_solved");
    if (!solved) {
        solved = true;
        tr->generate(0);
        int n = tr->get_size();
        size.resize(n);
        son.resize(n);
        ans.resize(n);
        dfs_prepare(0, -1);
        if (!callback_on_insert) MESSAGE("Dsu", NEED("on_dfs_insert"));
        if (!callback_on_remove) MESSAGE("Dsu", NEED("on_dfs_remove"));
        if (!callback_on_query) MESSAGE("Dsu", NEED("on_dfs_query"));
        dfs_solve(0, -1);
    }
}

void Dsu::dfs_prepare(int u, int f) {
    CALL("Dsu", "dfs_prepare");
    size[u] = 1; son[u] = -1;
    for (auto to : tr->get_adjoint(u)) {
        if (to != f) {
            dfs_prepare(to, u);
            size[u] += size[to];
            if (son[u] == -1 || size[son[u]] < size[to])
                son[u] = to;
        }
    }
}

void Dsu::dfs_insert(int u, int f) {
    CALL("Dsu", "dfs_insert");
    callback_on_insert(tr->get(u));
    for (auto to : tr->get_adjoint(u)) {
        if (to != f) {
            dfs_insert(to, u);
        }
    }
}

void Dsu::dfs_remove(int u, int f) {
    CALL("Dsu", "dfs_remove");
    callback_on_remove(tr->get(u));
    for (auto to : tr->get_adjoint(u)) {
        if (to != f) {
            dfs_remove(to, u);
        }
    }
}

void Dsu::dfs_solve(int u, int f) {
    CALL("Dsu", "dfs_solve");
    for (auto to : tr->get_adjoint(u)) {
        if (to != son[u] && to != f) {
            dfs_solve(to, u);
            dfs_remove(to, u);
        }
    }
    if (son[u] != -1) dfs_solve(son[u], u);
    for (auto to : tr->get_adjoint(u)) {
        if (to != son[u] && to != f) {
            dfs_insert(to, u);
        }
    }
    callback_on_insert(tr->get(u));
    ans[u] = callback_on_query();
}

namespace mk {
    shared_ptr<Dsu> dsu() {
        return make_shared<Dsu>();
    }

    shared_ptr<Dsu> dsu(shared_ptr<Tree> tr) {
        return dsu()->bind(tr);
    }
}
