#include "Array.h"
#include "Tuple.h"
#include "IO.h"

using namespace mk;
using namespace std;

void Case(int N, int S, int maxA) {
    auto n = integer(N);
    auto s = integer(S);
    auto arr = mk::array(n, integer(1, maxA));
    auto pro = mk::tuple(
        n->format("$x "),
        s->format("$x\n"),
        arr->format("$x $last[\n]")
    )->format("$x");
    BUILD(pro);
}

int main() {
    auto io = standard_io()
    ->testcase(1, bind(Case, 100, 10, 6))
    ->testcase(2, 3, bind(Case, 1000, 100, 80))
    ->testcase(4, 5, bind(Case, 50000, 50000, 25000))
    ->testcase(6, 10, bind(Case, 200000, 200000, 120000))
    ->working_directory("/home/hsj/my_problem_data/463")
    ->skip_generate_existing_data();
    io->generate();
    return 0;
}