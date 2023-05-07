#ifndef CHARACTER_H
#define CHARACTER_H

#include "Node.h"
#include "Define.h"
#include "Format.h"
#include "HashMap.h"
#include "Operator.h"

class Character : 
    public Node,
    public Hashable,
    public Formatable {
public:
    Character();
    Character(const Character& other);
    virtual ~Character();
    std::shared_ptr<Character> lower_bound(char x);
    std::shared_ptr<Character> upper_bound(char x);
    std::shared_ptr<Character> format(const std::string& fmt);
    CL_UPDATE_FUNC(Character, lower_bound, l, UF_set, CK_equal_to(Character), status = BY_LR);
    CL_UPDATE_FUNC(Character, upper_bound, r, UF_set, CK_equal_to(Character), status = BY_LR);

    char get();
    std::shared_ptr<Character> get_lower_bound();
    std::shared_ptr<Character> get_upper_bound();
    std::shared_ptr<Character> set(char value);
    CL_UPDATE_FUNC(Character, set, ptr_val, UF_assign, CK_equal_to(Character), status = BY_SET_PTR);

    virtual void generate(bool re, std::shared_ptr<Node> from) override;
    virtual std::shared_ptr<Node> clone() override;
    virtual void out() override;
    
    virtual bool equal(std::shared_ptr<Hashable> other) override;
    virtual uint hash_code() override;

    virtual void parse(const std::string& spec, int n, ...) override;
private:
    std::shared_ptr<Character> l;
    std::shared_ptr<Character> r;
    std::shared_ptr<Operator<Character>> op;
    int status;
    enum {EMPTY, BY_LR, BY_OP, BY_SET_PTR, BY_SET_VAL};
    
    char char_val;
    std::shared_ptr<Character> ptr_val;
};

namespace mk {
    std::shared_ptr<Character> character(char x);

    template<typename T, typename V, typename CHECK = 
        std::enable_if_t<
           (std::is_same_v<T, char> || 
            std::is_same_v<
                Character,
                shared_ptr_t<std::decay_t<T>>>) &&
           (std::is_same_v<V, char> || 
            std::is_same_v<
                Character,
                shared_ptr_t<std::decay_t<T>>>)>>
    std::shared_ptr<Character> character(T&& l, V&& r) {
        return std::make_shared<Character>()
            ->lower_bound(l)
            ->upper_bound(r);
    }
}

#endif