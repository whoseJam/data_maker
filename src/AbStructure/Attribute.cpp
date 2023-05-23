
#include <iostream>
#include <memory>
#include <type_traits>

#include "Debug.h"
#include "Logger.h"
#include "Attribute.h"
#include "Node.h"

using namespace std;

namespace mk {

int COUNT_ATTRIBUTE = 0;

Attribute::Attribute() {
    CALL(FUNCTION);
    COUNT_ATTRIBUTE++;
    key = "UNSET";
}

Attribute::Attribute(const Attribute& other) :
    Node(other) {
    CALL(FUNCTION);
    COUNT_ATTRIBUTE++;
    if (!other.val) MESSAGE("Attribute", NEED("value"));
    key = other.key;
    val = other.val->clone(0);
}

Attribute::~Attribute() {
    COUNT_ATTRIBUTE--;
}

/*
Description:
设置该属性的name.
*/
shared_ptr<Attribute> Attribute::name(const std::string& name) {
    CALL(FUNCTION);
    key = name;
    return dynamic_pointer_cast<Attribute>(shared_from_this());
}

/*
Description:
设置该属性的value.
*/
auto Attribute::value(shared_ptr<Node> val) -> shared_ptr<Attribute> {
    CALL(FUNCTION);
    this->val = val;
    return dynamic_pointer_cast<Attribute>(shared_from_this());
}

/*
Description:
获取该属性的name.
*/
auto Attribute::name() -> string {
    CALL(FUNCTION);
    return key;
}

auto Attribute::generate(bool re) -> void {
    CALL(FUNCTION);
    GENERATE;
    if (generated && !re) return;
    generated = true;
    
    if (!val) MESSAGE("Attribute", NEED("value"));
    if (key == "UNSET") MESSAGE("Attribute", NEED("name"));
    val->generate(re);
}

CL_CLONE(Attribute);

void Attribute::parse(const string& spec, int n, ...) {   
}

void Attribute::out() {
    CALL(FUNCTION);
    dynamic_pointer_cast<Formatable>(val)->out();
}

/*
Description:
构造一个空属性.
*/
auto attr() -> shared_ptr<Attribute> {
    return make_shared<Attribute>();
}

/*
Description:
构造一个名为name，值为value的属性.
*/
auto attr(const string& key, shared_ptr<Node> val) -> shared_ptr<Attribute> {
    return attr()->name(key)->value(val);
}

}
