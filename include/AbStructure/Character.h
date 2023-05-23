#ifndef CHARACTER_H
#define CHARACTER_H

#include "Node.h"
#include "Define.h"
#include "Format.h"
#include "Operator.h"

namespace mk {

extern int COUNT_CHARACTER;

class Character : 
    public Node,
    public Formatable {
public:
    Character();
    Character(const Character& other);
    virtual ~Character();
    auto calculate(std::shared_ptr<Operator<Character>> op) -> std::shared_ptr<Character>;
    auto lower_bound(char x) -> std::shared_ptr<Character>;
    auto upper_bound(char x) -> std::shared_ptr<Character>;
    auto lower_bound(std::shared_ptr<Character> l) -> std::shared_ptr<Character>;
    auto upper_bound(std::shared_ptr<Character> r) -> std::shared_ptr<Character>;
    auto format(const char* fmt) -> std::shared_ptr<Character>;
    
    auto value() -> char;
    auto lower_bound() -> std::shared_ptr<Character>;
    auto upper_bound() -> std::shared_ptr<Character>;
    auto value(char val) -> std::shared_ptr<Character>;
    auto value(std::shared_ptr<Character> val) -> std::shared_ptr<Character>;

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone(bool first) override;
    
    virtual void parse(const std::string& spec, int n, ...) override;
    virtual void out() override;
private:
    std::shared_ptr<Character> l;
    std::shared_ptr<Character> r;
    std::shared_ptr<Operator<Character>> op;
    int status;
    enum {EMPTY, BY_LR, BY_OP, BY_SET_PTR, BY_SET_VAL};
    
    char char_val;
    std::shared_ptr<Character> ptr_val;
};

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