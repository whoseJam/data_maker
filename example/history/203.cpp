#include "IO.h"
#include "Array.h"
#include "Tuple.h"
#include "Random.h"
#include "Operator.h"

using namespace mk;
using namespace std;
using namespace Random;

void Case(int maxN) {
    auto n = integer(5, maxN);
    auto k = integer(1, n - integer(1, 1));
    auto a_arr = mk::array(n, integer(1, 100));
    auto b_arr = mk::array(n, integer(1, 100))
    ->when_generating_per_element([&](shared_ptr<Array> arr, int idx){
        auto case_ = arr->from<Tuple>();
        auto a = case_->get<Array>(2)->get<Integer>(idx);
        auto b = arr->get<Integer>(idx);
        b->set(rand_int(a->get(), 100));
    });
    auto case_ = mk::tuple(
        n->format("$x "), 
        k->format("$x\n"), 
        a_arr->format("$x $last[\n]"), 
        b_arr->format("$x $last[\n]"))->format("$x");
    n->live_with(case_);
    k->live_with(case_);

    auto multicases = mk::array(integer(1, 5), case_)->format("$x");
    auto project = mk::tuple(
        multicases, 
        integer(0)->format("$x "), 
        integer(0)->format("$x\n"))->format("$x");
    BUILD(project);
}

int main() {

    auto io = standard_io()
    ->testcase(1, 3, bind(Case, 10))
    ->testcase(4, 10, bind(Case, 1000))
    ->working_directory("/home/hsj/my_problem_data/203")
    ->skip_generate_existing_data();
    io->generate();
    return 0;
}