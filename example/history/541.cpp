#include "ETT.h"
#include "IO.h"
#include "Integer.h"
#include "Logger.h"
#include "Node.h"
#include "Random.h"
#include "Splay.h"
#include "Tree.h"
#include "Array.h"
#include "Tuple.h"
#include "Option.h"
#include "Character.h"

using namespace std;
using namespace mk;
using namespace Random;
int fa[100005];

void Case(int mnN, int mxN, int mnM, int mxM, int maxW) {
    {
    ETT<bool> ett;
    auto n = integer(mnN, mxN);
    auto m = integer(mnM, mxM);
    auto optQ = mk::tuple(character('Q'), integer(1, n));
    auto optC = mk::tuple(character('C'), integer(1, n), integer(1, n));
    auto optF = mk::tuple(character('F'), integer(1, n), integer(1, maxW));
    auto opt = mk::option(
        optQ, 10,
        optC, 10,
        optF, 10);

    auto tr = tree(n)->vertex(
        vertex()->add_attribute(attr("w", integer(1, maxW)))
    );
    auto opt_arr = mk::array(m, opt)->format("$x\n")
    ->before_generate([&](shared_ptr<Array> arr) {
        ett.clear();
        int n = track<Tree>(tr)->size();
        for (int i = 0; i < n; i++) {
            ett.new_node(i + 1);
            if (i%10000 == 0) CONSOLE(cout<<"node init = "<<i<<"\n");
        }
        int cnt = 0;
        for (auto& e : tr->edge_set()) {
            ett.link(e->start() + 1, e->end() + 1);
            fa[e->end() + 1] = e->start() + 1;
            cnt++;
            if (cnt%10000 == 0) CONSOLE(cout<<"edge init = "<<cnt<<"\n");
        }
        CONSOLE(cout<<"before generate work finish\n");
        ett.make_root(1); })
    ->when_generating([&](shared_ptr<Array> This, int idx) {
        auto opt = This->get<Option>(idx);
        auto group = opt->get<Tuple>();
        auto type = group->get<Character>(0)->value();
        if (idx % 10000 == 0) CONSOLE(std::cout<<"Build Stage = "<<idx<<"\n");
        if (type == 'C') {
            auto x = group->get<Integer>(1);
            auto y = group->get<Integer>(2);
            if (ett.is_root(x->value())) {
                int old = x->value();
                while (x->value() == old) {
                    x->value(rand_int(1, track<Integer>(n)->value()));
                }
            }
            while (x->value() == y->value() || ett.is_ancestor_of(x->value(), y->value())) {
                y->value(rand_int(1, track<Integer>(n)->value()));
            }
            ett.cut(x->value(), fa[x->value()]);
            ett.link(y->value(), x->value());
            fa[x->value()] = y->value();
        } });
    auto pro = mk::tuple(
        n->format("$x\n"), 
        tr->format("$ie[$fa\n]$iv[$x[w] ]\n"), 
        m->format("$x\n"),
        opt_arr)->format("$x");
    n->live_with(pro);
    m->live_with(pro);
    BUILD_AND_OUT(pro);
    CLEAR_ALL;
    }
    if ((COUNT_SPLAY | COUNT_ETTTAG | COUNT_SPLAYNODE | COUNT_ETTINFO | COUNT_ETT) != 0) {
        CONSOLE(cout<<"Error\n");
        exit(-1);
    }
}

int main() {
    auto io = standard_io()
    ->testcase(1, 2, bind(Case, 4, 10, 10, 20, 100))
    ->testcase(3, 3, bind(Case, 800, 1000, 800, 1000, 20))
    ->testcase(4, 10, bind(Case, 100000, 100000, 300000, 300000, 10))
    ->skip_generate_existing_data()
    ->working_directory("/home/hsj/my_problem_data/test");
    io->generate();
    return 0;
}