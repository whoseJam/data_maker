
#include "Option.h"

using namespace std;

Option::Option() {
    fmt = "$x";
}

Option::Option(const Option& other) :
    Node(other),
    Formatable(other) {
    if (other.result) result = other.result->clone();
    for (int i = 0; i < other.opts.size(); i++) {
        opts.push_back(other.opts[i]->clone());
        robin.push_back(dynamic_pointer_cast<Integer>(other.robin[i]->clone()));
    }
}

shared_ptr<Option> Option::add_option(shared_ptr<Node> opt, int x) {
    CALL("Option", "add_option");
    opts.push_back(opt);
    robin.push_back(mk::integer(x));
    return dynamic_pointer_cast<Option>(shared_from_this());
}

shared_ptr<Option> Option::add_option(shared_ptr<Node> opt, shared_ptr<Integer> x) {
    CALL("Option", "add_option");
    opts.push_back(opt);
    robin.push_back(x);
    return dynamic_pointer_cast<Option>(shared_from_this());
}

void Option::generate(bool re, shared_ptr<Node> from) {
    CALL("Option", "generate");
    from_node = from;
    if (generated && !re) return;
    generated = true;

    for (int i = 0; i < robin.size(); i++) {
        opts[i]->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
        robin[i]->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
    }
}

CL_CLONE(Option);

void Option::parse(const string& spec, int n, ...) {
    CALL("Option", "parse");
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
    CALL("Option", "out");
    Formatable::parse(shared_from_this(), fmt, "Option");
}