#include "Tree.h"
#include "Array.h"
#include "Tuple.h"
#include "Option.h"
#include "Character.h"

using namespace std;
using namespace mk;

void Case(int N, int M, int maxW) {
    auto n = integer(N);
    auto m = integer(M);
    auto optQ = mk::tuple(character('Q'), n);
    auto optC = mk::tuple(character('C'), integer(1, n), integer(1, n));
    auto optF = mk::tuple(character('F'), integer(1, n), integer(1, maxW));
    auto opt = mk::option(
        optQ, 10,
        optC, 20,
        optF, 10);
    auto opt_arr = mk::array(m, opt)->when_generating_per_element([](shared_ptr<Array> This, int idx){
        auto tr = This->from<Tuple>()->get<Tree>(1);
        
    });

    auto tr = tree(n)->edge(edge())->vertex(
        vertex()->add_attribute(attr("w", integer(1, maxW)))
    );
    auto pro = mk::tuple(
        n->format("$x\n"), 
        tr->format("$ie[$fa\n]$iv[$x[w] ]\n"), 
        m,
        opt);
}

int main() {


    return 0;
}