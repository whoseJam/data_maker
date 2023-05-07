#ifndef TREE_H
#define TREE_H

#include <map>
#include <string>
#include <vector>
#include <functional>

#include "Edge.h"
#include "Vertex.h"
#include "Format.h"
#include "Integer.h"
#include "Attribute.h"

class Tree : 
    public Node, 
    public Hashable,
    public Iterable,
    public Formatable {
public:
    enum TreeForm { CHAIN = 0, FLOWER = 1, LONG_TREE = 2, RANDOM_TREE = 3, RANDOM_FORM, ROBIN_RANDOM_FORM };

    Tree();
    Tree(const Tree& other);
    std::shared_ptr<Tree> size(int num);
    CL_UPDATE_FUNC(Tree, size, vertex_num, UF_set, CK_equal_to(Integer), );
    std::shared_ptr<Tree> tree_form(TreeForm tf);
    CL_UPDATE_FUNC(Tree, edge, template_edge, UF_assign, CK_equal_to(Edge), );
    CL_UPDATE_FUNC(Tree, vertex, template_vertex, UF_assign, CK_equal_to(Vertex), );
    std::shared_ptr<Tree> format(const std::string& fmt);
    
    std::shared_ptr<Vertex> get(int u);
    std::shared_ptr<Edge> get(int f, int s);
    const std::vector<int> get_adjoint(int u);
    int get_size();

    virtual void generate(bool re, std::shared_ptr<Node> from) override;
    virtual std::shared_ptr<Node> clone() override;
    virtual void out() override;
    
    virtual bool equal(std::shared_ptr<Hashable> other) override;
    virtual uint hash_code();

    virtual void iter_reset() override;
    virtual void iter_next() override;
    virtual bool iter_loop_finish() override;
    virtual bool iter_at_last() override;

    virtual void parse(const std::string& specifier, int n, ...) override;
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
    std::vector<std::vector<int>> links;
    std::vector<std::shared_ptr<Edge>> edges;
    std::vector<std::shared_ptr<Vertex>> vertices;
    int stat;
    int cur_iter;
    bool empty_stat_called;
};

namespace mk {
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