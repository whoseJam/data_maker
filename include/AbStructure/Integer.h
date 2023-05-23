#ifndef INTEGER_H
#define INTEGER_H

#include <memory>

#include "Node.h"
#include "Define.h"
#include "Format.h"
#include "Operator.h"

namespace mk {

extern int COUNT_INTEGER;

class Integer : 
    public Node,
    public Formatable {
public:
    Integer();
    Integer(const Integer& other);
    virtual ~Integer();
    auto calculate(std::shared_ptr<Operator<Integer>> op) -> std::shared_ptr<Integer>;
    auto lower_bound(int x) -> std::shared_ptr<Integer>;
    auto upper_bound(int x) -> std::shared_ptr<Integer>;
    auto lower_bound(std::shared_ptr<Integer> l) -> std::shared_ptr<Integer>;
    auto upper_bound(std::shared_ptr<Integer> r) -> std::shared_ptr<Integer>;
    auto format(const char* fmt) -> std::shared_ptr<Integer>;

    auto value() -> int;
    auto lower_bound() -> std::shared_ptr<Integer>;
    auto upper_bound() -> std::shared_ptr<Integer>;
    auto value(int val) -> std::shared_ptr<Integer>;
    auto value(std::shared_ptr<Integer> val) -> std::shared_ptr<Integer>;

    virtual auto generate(bool re) -> void override;
    virtual auto clone(bool first) -> std::shared_ptr<Node> override;

    virtual auto parse(const std::string& spec, int n, ...) -> void override;
    virtual auto out() -> void override;
public:
    std::shared_ptr<Integer> l;
    std::shared_ptr<Integer> r;
    std::shared_ptr<Operator<Integer>> op;
    int status;
    enum {EMPTY, BY_LR, BY_OP, BY_SET_PTR, BY_SET_VAL};

    int int_val;
    std::shared_ptr<Integer> ptr_val;
};

std::shared_ptr<Integer> integer(int x);

template<typename T, typename V, typename CHECKER = 
    std::enable_if_t<
       (std::is_same_v<std::decay_t<T>, int> ||
        std::is_same_v<
            Integer,
            shared_ptr_t<std::decay_t<T>>>) &&
       (std::is_same_v<std::decay_t<V>, int> ||
        std::is_same_v<
            Integer,
            shared_ptr_t<std::decay_t<V>>>)>>
std::shared_ptr<Integer> integer(T&& l, V&& r) {
    return std::make_shared<Integer>()
    ->lower_bound(l)
    ->upper_bound(r);
}

}

#endif