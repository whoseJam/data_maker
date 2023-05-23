#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <memory>
#include <iostream>

#include "Node.h"
#include "Define.h"
#include "Format.h"
#include "HashMap.h"

namespace mk {

extern int COUNT_ATTRIBUTE;

class Attribute : 
    public Node,
    public Formatable {
public:
    Attribute();
    Attribute(const Attribute& other);
    virtual ~Attribute();
    auto name(const std::string& name) -> std::shared_ptr<Attribute>;
    auto value(std::shared_ptr<Node> val) -> std::shared_ptr<Attribute>;
    
    template<typename T>
    std::shared_ptr<T> get() {
        return std::dynamic_pointer_cast<T>(val);
    }
    auto name() -> std::string;

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone(bool first) override;

    virtual void parse(const std::string& spec, int n, ...) override;
    virtual void out() override;
private:
    std::shared_ptr<Node> val;
    std::string key;
};

auto attr() -> std::shared_ptr<Attribute>;
auto attr(const std::string& key, std::shared_ptr<Node> val) -> std::shared_ptr<Attribute>;
}

#endif