// #ifndef BARETREE_H
// #define BARETREE_H

// #include <map>

// #include "Tree.h"
// #include "Splay.h"

// namespace mk {

// template<typename T>
// class BareTree {
// public:
//     BareTree() = default;
//     auto initialize(std::shared_ptr<Tree> tr, int root = 0) -> void;
//     auto traiter(std::function<T(std::shared_ptr<Vertex>)>) -> void;
//     auto traiter(std::function<T(std::shared_ptr<Edge>)>) -> void;
//     auto einfo(int u, int v) -> std::shared_ptr<T>;
//     auto vinfo(int u) -> std::shared_ptr<T>;
//     auto size() -> int;
//     auto root() -> int;
// private:
//     struct TreeEdge {
//         int x, y;
//         std::shared_ptr<T> info;
//         TreeEdge(int x, int y, std::shared_ptr<T> info) : x(std::min(x, y)), y(std::max(x, y)), info(std::move(info)) {}
//         bool operator < (const TreeEdge& other) {
//             if (x != other.x) return x < other.x;
//             return y < other.y;
//         }
//     };
//     struct TreeVertex {
//         int x;
//         std::shared_ptr<T> info;
//         TreeVertex(int x, std::shared_ptr<T> info) : x(x), info(info) {}
//         bool operator < (const TreeVertex& other) { return x < other.x; }
//     };
    
//     int n;
//     std::shared_ptr<Tree> tree;
//     std::vector<std::vector<int>> links;
//     std::map<TreeEdge, std::shared_ptr<T>> emap;
//     std::map<TreeVertex, std::shared_ptr<T>> vmap;
//     std::function<T(std::shared_ptr<Vertex>)> v_traiter;
//     std::function<T(std::shared_ptr<Edge>)> e_traiter;
// };

// template<typename T>
// auto BareTree<T>::initialize(std::shared_ptr<Tree> tr, int root) -> void {
//     n = tr->size();
//     links.resize(n); vmap.clear(); emap.clear();
//     for (auto& v : tr->vertex_set()) {
//         int x = v->get();
//         vmap[TreeVertex(x)] = std::make_shared<T>(std::move(v_traiter(v)));
//     }
//     for (auto& e : tr->edge_set()) {
//         int x = e->get_start();
//         int y = e->get_end();
//         links[x].push_back(y);
//         links[y].push_back(x);
//         if (e_traiter)
//             emap[TreeEdge(x, y)] = std::make_shared<T>(std::move(e_traiter(e)));
//     }
// }

// template<typename T>
// auto BareTree<T>::traiter(std::function<T(std::shared_ptr<Vertex>)> callback) -> void {
//     v_traiter = callback;
// }

// template<typename T>
// auto BareTree<T>::traiter(std::function<T(std::shared_ptr<Edge>)> callback) -> void {
//     e_traiter = callback;
// }

// template<typename T>
// auto BareTree<T>::einfo(int u, int v) -> std::shared_ptr<T> {
//     auto ans = emap.find(TreeEdge(u, v));
//     if (ans == emap.end()) return nullptr;
//     return ans->info;
// }

// template<typename T>
// auto BareTree<T>::vinfo(int u) -> std::shared_ptr<T> {
//     auto ans = vmap.find(TreeVertex(u));
//     if (ans == vmap.end()) return nullptr;
//     return ans->info;
// }

// template<typename T>
// auto BareTree<T>::size() -> int {
//     return n;
// }

// }

// #endif