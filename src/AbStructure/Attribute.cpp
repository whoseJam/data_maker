
#include <iostream>

#include "Debug.h"
#include "Clone.h"
#include "Logger.h"
#include "Attribute.h"

using namespace std;

Attribute::Attribute() {
    CALL(FUNCTION);
    key = "UNSET";
}

Attribute::Attribute(const Attribute& other) :
    Node(other) {
    CALL(FUNCTION);
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
    CALL(FUNCTION);
    key = name;
    return dynamic_pointer_cast<Attribute>(shared_from_this());
}

string Attribute::get_name() {
    CALL(FUNCTION);
    return key;
}

void Attribute::generate(bool re, shared_ptr<Node> from) {
    CALL(FUNCTION);
    from_node = from;
    if (generated && !re) return;
    generated = true;
    
    if (!val) MESSAGE("Attribute", NEED("value"));
    if (key == "UNSET") MESSAGE("Attribute", NEED("name"));
    val->generate(re, dynamic_pointer_cast<Node>(shared_from_this()));
}

CL_CLONE(Attribute);

bool Attribute::equal(shared_ptr<Hashable> o) {
    CALL(FUNCTION);
    shared_ptr<Attribute> other = dynamic_pointer_cast<Attribute>(o);
    if (!other) return false;
    shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(val);
    shared_ptr<Hashable> b = dynamic_pointer_cast<Hashable>(other->val);
    if (!a || !b) MESSAGE("Attribute", UNEXCEPT);
    return a->equal(b);
}

uint Attribute::hash_code() {
    CALL(FUNCTION);
    shared_ptr<Hashable> a = dynamic_pointer_cast<Hashable>(val);
    if (!a) MESSAGE("Attribute", UNEXCEPT);
    return a->hash_code();
}

void Attribute::parse(const string& spec, int n, ...) {
    
}

void Attribute::out() {
    CALL(FUNCTION);
    dynamic_pointer_cast<Formatable>(val)->out();
}

namespace mk {
    shared_ptr<Attribute> attr() {
        return make_shared<Attribute>();
    }
}
