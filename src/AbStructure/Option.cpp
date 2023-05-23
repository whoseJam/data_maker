
#include "Option.h"

using namespace std;

namespace mk {

int COUNT_OPTION = 0;

Option::Option() {
    COUNT_OPTION++;
    fmt = "$x";
}

Option::Option(const Option& other) :
    Node(other),
    Formatable(other) {
    COUNT_OPTION++;
    if (other.result) result = other.result->clone(0);
    for (int i = 0; i < other.opts.size(); i++) {
        opts.push_back(other.opts[i]->clone(0));
        robin.push_back(dynamic_pointer_cast<Integer>(other.robin[i]->clone(0)));
    }
}

Option::~Option() {
    COUNT_OPTION--;
}

shared_ptr<Option> Option::add_option(shared_ptr<Node> opt, int x) {
    CALL(FUNCTION);
    opts.push_back(opt);
    robin.push_back(mk::integer(x));
    return dynamic_pointer_cast<Option>(shared_from_this());
}

shared_ptr<Option> Option::add_option(shared_ptr<Node> opt, shared_ptr<Integer> x) {
    CALL(FUNCTION);
    opts.push_back(opt);
    robin.push_back(x);
    return dynamic_pointer_cast<Option>(shared_from_this());
}

void Option::generate(bool re) {
    CALL(FUNCTION);
    if (generated && !re) return;
    generated = true;

    for (int i = 0; i < robin.size(); i++) {
        opts[i]->generate(re);
        robin[i]->generate(re);
    }
}

CL_CLONE(Option);

void Option::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
    try {
        CALL_FORMATTER(spec, n);
    } catch (SpecNotFoundException& e) {
        va_list valist;
        va_start(valist, n);
        if (spec == SPEC_SELF) {
            if (!IN_RANGE(0, n, 0)) MESSAGE("Option", FUNC_ARGS_MISMATCH(n, 0, 0));
            auto ele = get<Formatable>();
            if (!ele) MESSAGE("Option", UNABLE("Formatable"));
            ele->out();
        } else {
            MESSAGE("Option", FUNC_NOT_FOUND(spec));
        }
        va_end(valist);
    }
}

void Option::out() {
    CALL(FUNCTION);
    Formatable::parse(shared_from_this(), fmt, "Option");
}

shared_ptr<Option> option() {
    return make_shared<Option>();
}

}
