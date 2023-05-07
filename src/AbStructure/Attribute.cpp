
#include <iostream>

#include "Debug.h"
#include "Clone.h"
#include "Logger.h"
#include "Attribute.h"

using namespace std;

Attribute::Attribute() {
    CALL("Attribute", "Attribute");
    key = "UNSET";
}

Attribute::Attribute(const Attribute& other) :
    Node(other) {
    CALL("Attribute", "Attribute");
    if (!other.val) MESSAGE("Attribute", NEED("value"));
    key = other.key;
    val = other.val->clone();
}

Attribute::~Attribute() {
#ifdef OUTPUT_DELETER
    cout << "delete attribute\n";
#endif
}

shared_ptr<Attribute> Attribute::name(const std::string& name) {
    CALL("Attribute", "name");
    key = name;
    return dynamic_pointer_cast<Attribute>(shared_from_this());
}

string Attribute::get_name() {
    CALL("Attribute", "get_name");
    return key;
}

void Attribute::generate(bool re, shared_ptr<Node> from) {
    CALL("Attribute", "generate");
    from_node = from;
    if (generated && !re) return;
    generated = true;
    
    if (!val) MESSAGE("Attribute", NEED("value"));
    if (key == "UNSET") MESSAGE("Attribute", NEED("name"));
    val->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
}

CL_CLONE(Attribute);

void Attribute::out() {
    CALL("Attribute", "out");
    val->out();
}

bool Attribute::equal(shared_ptr<Hashable> o) {
    CALL("Attribute", "equal");
    shared_ptr<Attribute> other = dynamic_pointer_cast<Attribute>(o);
    if (!other) return false;
    shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(val);
    shared_ptr<Hashable> b = dynamic_pointer_cast<Hashable>(other->val);
    if (!a || !b) MESSAGE("Attribute", UNEXCEPT);
    return a->equal(b);
}

uint Attribute::hash_code() {
    CALL("Attribute", "hash_code");
    shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(val);
    if (!a) MESSAGE("Attribute", UNEXCEPT);
    return a->hash_code();
}

namespace mk {
    shared_ptr<Attribute> attr() {
        return make_shared<Attribute>();
    }
}
