#include "IO.h"
#include "Dsu.h"
#include "Tree.h"
#include "Array.h"
#include "Tuple.h"
#include "Random.h"

using namespace mk;
using namespace std;
using namespace Random;

void Case(int N, int M, int maxC) {
    auto n = integer(N, N);
    auto m = integer(M, M);
    auto tr = tree(n)
    ->vertex(vertex()->add_attribute(attr("color", integer(1, maxC))))
    ->edge(edge())
    ->format("$iv[$x[color] ]\n$ie[$fa $son\n]");

    auto hmap = hash_map();
    auto subcnt = dsu(tr)
    ->on_dfs_insert([&](shared_ptr<Vertex> v){hmap->insert(v->get("color")->get<Integer>());})
    ->on_dfs_remove([&](shared_ptr<Vertex> v){hmap->remove(v->get("color")->get<Integer>());})
    ->on_dfs_query([&](){return hmap->query_different();});

    auto qry = mk::array(m)
    ->fill(mk::tuple(integer(1, n), integer(1, n)))
    ->when_generating_per_element([&](shared_ptr<Array> a, int idx){
        auto node = a->get<Tuple>(idx)->get<Integer>(0);
        auto rank = a->get<Tuple>(idx)->get<Integer>(1);
        int x = node->get() - 1;
        int ans = subcnt->query(x);
        int size = subcnt->get_size(x);
        int choice = rand_int(0, size/ans);
        rank->set(choice);
    });

    auto pro = mk::tuple(
        n->format("$x "),
        m->format("$x\n"),
        tr,
        qry->format("$x\n")
    )->format("$x");
    BUILD(pro);
}

void test() {
    Case(60, 4, 3);
    exit(0);
}

int main() {
    test();

    auto io = standard_io()
    ->testcase(1, 3, std::bind(Case, 1000, 500, 700))
    ->testcase(4, 5, std::bind(Case, 100000, 100000, 10000))
    ->testcase(6, 7, std::bind(Case, 100000, 100000, 300))
    ->testcase(8, 10, std::bind(Case, 100000, 100000, 50000))
    ->working_directory("/home/hsj/my_problem_data/2");
    //->skip_generate_existing_data();
    io->generate();
    return 0;
}