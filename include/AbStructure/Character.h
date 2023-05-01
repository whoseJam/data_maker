#ifndef CHARACTER_H
#define CHARACTER_H

#include "Node.h"
#include "Define.h"
#include "Format.h"
#include "HashMap.h"

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
    CL_UPDATE_FUNC(Character, lower_bound, l, UF_set);
    CL_UPDATE_FUNC(Character, upper_bound, r, UF_set);

    char get();
    std::shared_ptr<Character> get_lower_bound();
    std::shared_ptr<Character> get_upper_bound();
    std::shared_ptr<Character> set(char value);
    CL_UPDATE_FUNC(Character, set, ptr_val, UF_assign);

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone() override;
    virtual void out() override;
    
    virtual bool equal(std::shared_ptr<Hashable> other) override;
    virtual uint hash_code() override;

    virtual void parse(const std::string& spec, int n, ...) override;
private:
//  define stage
    std::shared_ptr<Character> l;
    std::shared_ptr<Character> r;
    bool char_val_is_set;
    
//  generate stage
    char char_val;
    std::shared_ptr<Character> ptr_val;
};

namespace mk {
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
        auto l_move = l;
        auto r_move = r;
        return std::make_shared()
            ->lower_bound(std::forward<T>(l_move))
            ->upper_bound(std::forward<T>(r_move));
    }
}

#endif