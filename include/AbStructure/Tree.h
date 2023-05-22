#ifndef TREE_H
#define TREE_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>

#include "Edge.h"
#include "Vertex.h"
#include "Format.h"
#include "Integer.h"
#include "Attribute.h"

namespace mk {

class Tree : 
    public Node, 
    public Iterable,
    public Formatable {
public:
    enum TreeForm { CHAIN = 0, FLOWER = 1, LONG_TREE = 2, RANDOM_TREE = 3, RANDOM_FORM, ROBIN_RANDOM_FORM };

    Tree();
    Tree(const Tree& other);
    auto size(int n) -> std::shared_ptr<Tree>;
    auto size(std::shared_ptr<Integer> n) -> std::shared_ptr<Tree>;
    auto tree_form(TreeForm tf) -> std::shared_ptr<Tree>;
    auto edge(std::shared_ptr<Edge> e) -> std::shared_ptr<Tree>;
    auto vertex(std::shared_ptr<Vertex> v) -> std::shared_ptr<Tree>;
    auto format(const std::string& fmt) -> std::shared_ptr<Tree>;
    
    auto vertex(int u) -> std::shared_ptr<Vertex>;
    auto vertex_set() -> std::vector<std::shared_ptr<Vertex>>&;
    auto edge_set() -> std::vector<std::shared_ptr<Edge>>&;
    auto size() -> int;

    virtual auto generate(bool re) -> void override;
    virtual auto clone(bool first) -> std::shared_ptr<Node> override;

    virtual void iter_reset() override;
    virtual void iter_next() override;
    virtual bool iter_loop_finish() override;
    virtual bool iter_at_last() override;

    virtual void parse(const std::string& specifier, int n, ...) override;
    virtual void out() override;
private:
    void gen_long_tree();
    void gen_random_tree();
    void gen_flower();
    void gen_chain();
    void add_edge(int x, int y);

//  inner helper
    using TreeFunPtr = void(Tree::*)();
    static int robin_iter;
    static TreeFunPtr gen_func[4];

    std::shared_ptr<Integer> vertex_num;
    TreeForm tf;
    std::shared_ptr<Edge> template_edge;
    std::shared_ptr<Vertex> template_vertex;
    std::vector<std::shared_ptr<Edge>> edges;
    std::vector<std::shared_ptr<Vertex>> vertices;
    int stat;
    int cur_iter;
    bool empty_stat_called;
};

std::shared_ptr<Tree> tree();
template<typename T, typename CHECKER = 
    std::enable_if_t<
        std::is_same_v<std::decay_t<T>, int> || 
        std::is_same_v<
            Integer, 
            shared_ptr_t<std::decay_t<T>>>>>
std::shared_ptr<Tree> tree(T&& n) {
    auto n_move = n;
    return tree()->size(std::forward<T>(n_move));
}
}

#endif