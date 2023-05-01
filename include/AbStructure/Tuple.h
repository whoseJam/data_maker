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

class Tuple : 
    public Node, 
    public Hashable,
    public Iterable,
    public Formatable {
public:
    Tuple();
    Tuple(const Tuple& other);
    virtual ~Tuple();
    CL_UPDATE_FUNC(Tuple, append, elements, UF_append_vector);
    CL_UPDATE_FUNC(Tuple, unshift, elements, UF_unshift_vector);
    std::shared_ptr<Tuple> format(const std::string& fmt);
    std::shared_ptr<Tuple> when_generating_per_element(std::function<void(std::shared_ptr<Tuple>, int)> );
    std::shared_ptr<Tuple> after_generate(std::function<void(std::shared_ptr<Tuple>)>);

    template<typename T>
    std::shared_ptr<T> get(int idx) {
        if (idx < 0 || idx >= elements.size()) MESSAGE("Tuple", INDEX_OUT_OF_BOUNDARY);
        std::shared_ptr<T> ans = std::dynamic_pointer_cast<T>(elements[idx]);
        if (!ans) MESSAGE("Tuple", TYPE_ERROR);
        return ans;
    }

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone() override;
    virtual void out() override;

    virtual bool equal(std::shared_ptr<Hashable> other) override;
    virtual uint hash_code() override;

    virtual void iter_reset() override;
    virtual void iter_next() override;
    virtual bool iter_loop_finish() override;
    virtual bool iter_at_last() override;
    
    virtual void parse(const std::string& specifier, int n, ...) override;
private:
//  callback
    std::function<void(std::shared_ptr<Tuple>, int)> callback_when_generating_per_element;
    std::function<void(std::shared_ptr<Tuple>)> callback_after_generate;

//  define stage
    std::vector<std::shared_ptr<Node>> elements;

//  output stage
    int cur_iter;
};

namespace mk {
    template<typename A, typename CHECKER = 
        std::enable_if_t<
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<A>>>>>
    std::shared_ptr<Tuple> tuple(A&& a) {
        return std::make_shared<Tuple>()
            ->append(std::forward<A>(a));
    }

    template<typename A, typename B, typename CHECKER = 
        std::enable_if_t<
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<A>>> &&
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<B>>>>>
    std::shared_ptr<Tuple> tuple(A&& a, B&& b) {
        return std::make_shared<Tuple>()
            ->append(std::forward<A>(a))
            ->append(std::forward<B>(b));
    }

    template<typename A, typename B, typename C, typename CHECKER = 
        std::enable_if_t<
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<A>>> &&
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<B>>> && 
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<C>>>>>
    std::shared_ptr<Tuple> tuple(A&& a, B&& b, C&& c) {
        return std::make_shared<Tuple>()
            ->append(std::forward<A>(a))
            ->append(std::forward<B>(b))
            ->append(std::forward<C>(c));
    }

    template<typename A, typename B, typename C, typename D, typename CHECKER = 
        std::enable_if_t<
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<A>>> &&
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<B>>> && 
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<C>>> &&
            std::is_base_of_v<Node, shared_ptr_t<std::decay_t<D>>>>>
    std::shared_ptr<Tuple> tuple(A&& a, B&& b, C&& c, D&& d) {
        return std::make_shared<Tuple>()
            ->append(std::forward<A>(a))
            ->append(std::forward<B>(b))
            ->append(std::forward<C>(c))
            ->append(std::forward<D>(d));
    }
}

#endif