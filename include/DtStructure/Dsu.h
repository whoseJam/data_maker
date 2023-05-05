#ifndef DSU_H
#define DSU_H

#include <memory>
#include <vector>
#include <functional>

#include "Tree.h"

class Dsu : 
    public std::enable_shared_from_this<Dsu> {
public:
    Dsu();
    std::shared_ptr<Dsu> bind(std::shared_ptr<Tree> tr);
    std::shared_ptr<Dsu> on_dfs_insert(std::function<void(std::shared_ptr<Vertex>)>);
    std::shared_ptr<Dsu> on_dfs_remove(std::function<void(std::shared_ptr<Vertex>)>);
    std::shared_ptr<Dsu> on_dfs_query(std::function<int()>);
    int query(int x);
    int get_size(int x);
private:
    void ensure_solved();
    void dfs_insert(int u, int f);
    void dfs_remove(int u, int f);
    void dfs_prepare(int u, int f);
    void dfs_solve(int u, int f);
    std::shared_ptr<Tree> tr;
    std::vector<int> size;
    std::vector<int> son;
    std::function<void(std::shared_ptr<Vertex>)> callback_on_insert;
    std::function<void(std::shared_ptr<Vertex>)> callback_on_remove;
    std::function<int()> callback_on_query;
    std::vector<int> ans;
    bool solved;
};

namespace mk {
    std::shared_ptr<Dsu> dsu();
    std::shared_ptr<Dsu> dsu(std::shared_ptr<Tree> tr);
}

#endif