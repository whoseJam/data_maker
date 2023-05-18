#include <chrono>
#include <thread>
#include <iostream>

#include "Dsu.h"
#include "Tree.h"
#include "Array.h"
#include "Tuple.h"
#include "Random.h"

using std::cout;
using std::make_shared;
using std::shared_ptr;
using namespace Random;
using namespace mk;

void test_tree_is_null() {
    auto tr = make_shared<Tree>();
    BUILD(tr);
}

void t1() {
    using namespace std;
    this_thread::sleep_for(chrono::seconds(3));
    CallStack::get()->print();
}

void test_tree_condition() {
    auto n = integer(4, 6);
    auto tr = tree(n)
    ->vertex(vertex()->add_attribute(attr("color", integer(1, 5))))
    ->edge(edge())
    ->format("$iv[$x[color] ]\n$ie[$fa $son\n]$iv[$x $nlast[ - ]]");

    auto query = array(n)
    ->fill(tuple(integer(1, n), integer(1, 5))->format("$x "))
    ->when_generating_per_element([&](shared_ptr<Array> a, int idx) {
        auto tp = a->get<Tuple>(idx);
        auto nd = tp->get<Integer>(0);
        auto rk = tp->get<Integer>(1);
    })->format("$x;\n");
    auto pro = tuple(tr, query)->format("$x\n");
    BUILD(pro);
}

void test_tree_subtree() {
    auto n = integer(4, 6);
    auto tr = tree(n)
     ->vertex(vertex()->add_attribute(attr("color", integer(1, 5))))
    ->edge(edge())
    ->format("$iv[$x[color] ]\n$ie[$fa $son\n]$iv[$x $nlast[ - ]]");
    auto hs = hash_map();
    auto tr_help = dsu(tr)
    ->on_dfs_insert([&](shared_ptr<Vertex> v) {hs->insert(v->get("color"));})
    ->on_dfs_remove([&](shared_ptr<Vertex> v) {hs->remove(v->get("color"));})
    ->on_dfs_query([&]() -> int {return hs->query_different();});

    auto query = array(n)
    ->fill(tuple(integer(1, n), integer(1, 5))->format("$x "))
    ->when_generating_per_element([&](shared_ptr<Array> a, int idx) {
        auto tp = a->get<Tuple>(idx);
        auto nd = tp->get<Integer>(0);
        auto rk = tp->get<Integer>(1);
        cout << "node="<<nd->get()<< " count="<<tr_help->query(nd->get()-1) << "\n";
    })->format("$x\n");
    auto pro = tuple(tr, query)->format("$x\n");
    BUILD(pro);
}

using Test = std::function<void()>;
Test test_map[] = {
    test_tree_is_null,      // 0
    test_tree_condition,    // 1
    test_tree_subtree       // 2
};

int main(int n, char** args) {
    int idx = atoi(args[1]);
    test_map[idx]();
    
    return 0;
}