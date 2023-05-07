#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <memory>
#include <iostream>

#include "Node.h"
#include "Define.h"
#include "Format.h"
#include "HashMap.h"

class Attribute : 
    public Node,
    public Hashable,
    public std::enable_shared_from_this<Attribute> {
public:
    Attribute();
    Attribute(const Attribute& other);
    virtual ~Attribute();
    std::shared_ptr<Attribute> name(const std::string& name);
    CL_UPDATE_FUNC(Attribute, value, val, UF_assign, CK_base_is(Node), );
    
    template<typename T>
    std::shared_ptr<T> get() {
        return std::dynamic_pointer_cast<T>(val);
    }
    std::string get_name();

    virtual void generate(bool re, std::shared_ptr<Node> from) override;
    virtual std::shared_ptr<Node> clone() override;
    virtual void out() override;

    virtual bool equal(std::shared_ptr<Hashable> other) override;
    virtual uint hash_code() override;
private:
    std::shared_ptr<Node> val;
    std::string key;
};

namespace mk {
    std::shared_ptr<Attribute> attr();
    
    template<typename T, typename CHECKER = 
        std::enable_if_t<
            std::is_base_of_v<
                Node,
                shared_ptr_t<std::decay_t<T>>>>>
    std::shared_ptr<Attribute> attr(const std::string& name, T&& val) {
        return std::make_shared<Attribute>()
            ->name(name)->value(val);
    }
}

#endif