#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <vector>
#include <iostream>
#include <functional>

#include "Node.h"
#include "Define.h"
#include "Logger.h"
#include "Format.h"
#include "HashMap.h"

namespace mk {

class Tuple : 
    public Node, 
    public Iterable,
    public Formatable {
public:
    Tuple();
    Tuple(const Tuple& other);
    virtual ~Tuple();
    auto append(std::shared_ptr<Node> ele) -> std::shared_ptr<Tuple>;
    auto format(const std::string& fmt) -> std::shared_ptr<Tuple>;
    auto before_generate(std::function<void(std::shared_ptr<Tuple>)>) -> std::shared_ptr<Tuple>;
    auto when_generating(std::function<void(std::shared_ptr<Tuple>, int)>) -> std::shared_ptr<Tuple>;
    auto after_generate(std::function<void(std::shared_ptr<Tuple>)>) -> std::shared_ptr<Tuple>;

    template<typename T>
    std::shared_ptr<T> get(int idx) {
        if (idx < 0 || idx >= elements.size()) MESSAGE("Tuple", INDEX_OUT_OF_BOUNDARY);
        std::shared_ptr<T> ans = std::dynamic_pointer_cast<T>(elements[idx]);
        if (!ans) MESSAGE("Tuple", TYPE_ERROR);
        return ans;
    }

    virtual auto generate(bool re) -> void override;
    virtual auto clone(bool first) -> std::shared_ptr<Node> override;

    virtual auto iter_reset() -> void override;
    virtual auto iter_next() -> void override;
    virtual auto iter_loop_finish() -> bool override;
    virtual auto iter_at_last() -> bool override;
    
    virtual void parse(const std::string& specifier, int n, ...) override;
    virtual void out() override;
private:
//  callback
    std::function<void(std::shared_ptr<Tuple>)> callback_before_generate;
    std::function<void(std::shared_ptr<Tuple>, int)> callback_when_generating;
    std::function<void(std::shared_ptr<Tuple>)> callback_after_generate;

//  define stage
    std::vector<std::shared_ptr<Node>> elements;

//  output stage
    int cur_iter;
};

auto tuple() -> std::shared_ptr<Tuple>;
auto tuple(std::shared_ptr<Node>) -> std::shared_ptr<Tuple>;
auto tuple(std::shared_ptr<Node>, std::shared_ptr<Node>) -> std::shared_ptr<Tuple>;
auto tuple(std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>) -> std::shared_ptr<Tuple>;
auto tuple(std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>) -> std::shared_ptr<Tuple>;
auto tuple(std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node>) -> std::shared_ptr<Tuple>;
}

#endif