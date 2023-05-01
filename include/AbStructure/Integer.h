#ifndef INTEGER_H
#define INTEGER_H

#include <memory>

#include "Node.h"
#include "Define.h"
#include "Format.h"
#include "HashMap.h"

class Integer : 
    public Node,
    public Hashable,
    public Formatable {
public:
    Integer();
    Integer(const Integer& other);
    virtual ~Integer();
    std::shared_ptr<Integer> lower_bound(int x);
    std::shared_ptr<Integer> upper_bound(int x);
    std::shared_ptr<Integer> format(const std::string& fmt);
    CL_UPDATE_FUNC(Integer, lower_bound, l, UF_set);
    CL_UPDATE_FUNC(Integer, upper_bound, r, UF_set);
    
    int get();
    std::shared_ptr<Integer> get_lower_bound();
    std::shared_ptr<Integer> get_upper_bound();
    std::shared_ptr<Integer> set(int value);
    CL_UPDATE_FUNC(Integer, set, ptr_val, UF_assign);

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone() override;
    virtual void out() override;
    
    virtual bool equal(std::shared_ptr<Hashable> other) override;
    virtual uint hash_code() override;

    virtual void parse(const std::string& spec, int n, ...) override;
private:
//  define stage
    std::shared_ptr<Integer> l;
    std::shared_ptr<Integer> r;
    bool int_val_is_set;

//  generate stage
    int int_val;
    std::shared_ptr<Integer> ptr_val;
};

namespace mk {
    template<typename T, typename V, typename CHECKER = 
        std::enable_if_t<
           (std::is_same_v<T, int> ||
            std::is_same_v<
                Integer,
                shared_ptr_t<std::decay_t<T>>>) &&
           (std::is_same_v<V, int> ||
            std::is_same_v<
                Integer,
                shared_ptr_t<std::decay_t<V>>>)>>
    std::shared_ptr<Integer> integer(T&& l, V&& r) {
        auto l_move = l;
        auto r_move = r;
        return std::make_shared<Integer>()
            ->lower_bound(std::forward<T>(l_move))
            ->upper_bound(std::forward<V>(r_move));
    }
}

#endif