#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "Node.h"
#include "Debug.h"
#include "Format.h"
#include "Logger.h"
#include "Integer.h"
#include "HashMap.h"
#include "Character.h"

class String : 
    public Node, 
    public Hashable,
    public Iterable,
    public Formatable {
public:
    String();
    String(const String& other);
    virtual ~String();
    std::shared_ptr<String> length(int len);
    CL_UPDATE_FUNC(String, length, len, UF_assign, CK_equal_to(Integer), );
    CL_UPDATE_FUNC(String, fill, template_char, UF_assign, CK_equal_to(Character), );
    std::shared_ptr<String> format(const std::string& fmt);
    std::shared_ptr<String> when_generating_per_element(std::function<void(std::shared_ptr<String>, int)>);
    std::shared_ptr<String> after_generate(std::function<void(std::shared_ptr<String>)>);

    int get_length();
    std::shared_ptr<Character> get(int idx);

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone() override;
    virtual void out() override;

    virtual bool equal(std::shared_ptr<Hashable> other);
    virtual uint hash_code();

    virtual void iter_reset() override;
    virtual void iter_next() override;
    virtual bool iter_loop_finish() override;
    virtual bool iter_at_last() override;

    virtual void parse(const std::string& spec, int n, ...);
private:
//  callback
    std::function<void(std::shared_ptr<String>, int)> callback_when_generating_per_element;
    std::function<void(std::shared_ptr<String>)> callback_after_generate;

//  define stage
    std::shared_ptr<Integer> len;
    std::shared_ptr<Character> template_char;
    
//  generate stage
    std::vector<std::shared_ptr<Character>> elements;

//  output stage
    int cur_iter;
};

namespace mk {
    std::shared_ptr<String> string();

    template<typename T, typename CHECKER = 
        std::enable_if_t<
            std::is_same_v<std::decay_t<T>, int> ||
            std::is_same_v<
                Integer,
                shared_ptr_t<std::decay_t<T>>>>>
    std::shared_ptr<String> string(T&& n) {
        auto n_move = n;
        return string()->length(std::forward<T>(n_move));
    }

    template<typename T, typename E, typename CHECKER = 
        std::enable_if_t<
           (std::is_same_v<std::decay_t<T>, int> ||
            std::is_same_v<
                Integer,
                shared_ptr_t<std::decay_t<T>>>) &&
            std::is_same_v<
                Character,
                shared_ptr_t<std::decay_t<E>>>>>
    std::shared_ptr<String> string(T&& n, E&& e) {
        auto n_move = n;
        auto e_move = e;
        return string(std::forward<T>(n_move))->fill(std::forward<E>(e_move));
    }
}


#endif