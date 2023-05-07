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

class Array : 
    public Node, 
    public Hashable,
    public Iterable,
    public Formatable {
public:
    Array();
    Array(const Array& other);
    virtual ~Array();
    std::shared_ptr<Array> length(int len);
    CL_UPDATE_FUNC(Array, length, len, UF_assign, CK_equal_to(Integer), );
    CL_UPDATE_FUNC(Array, fill, template_ele, UF_assign, CK_base_is(Node), );
    std::shared_ptr<Array> format(const std::string& fmt);
    std::shared_ptr<Array> when_generating_per_element(std::function<void(std::shared_ptr<Array>, int)>);
    std::shared_ptr<Array> after_generate(std::function<void(std::shared_ptr<Array>)>);

    int get_length();
    template<typename T> 
    std::shared_ptr<T> get(int idx) {
        CALL("Array", "get");
        if (idx < 0 || idx >= elements.size()) MESSAGE("Array", INDEX_OUT_OF_BOUNDARY);
        std::shared_ptr<T> ans = std::dynamic_pointer_cast<T>(elements[idx]);
        if (!ans) MESSAGE("Array", TYPE_ERROR);
        return ans;
    }

    virtual void generate(bool re, std::shared_ptr<Node> from) override;
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
    std::function<void(std::shared_ptr<Array>, int)> callback_when_generating_per_element;
    std::function<void(std::shared_ptr<Array>)> callback_after_generate;
    std::shared_ptr<Integer> len;
    std::shared_ptr<Node> template_ele;
    std::vector<std::shared_ptr<Node>> elements;
    int cur_iter;
};

namespace mk {
    std::shared_ptr<Array> array();

    template<typename T, typename CHECKER = 
        std::enable_if_t<
            std::is_same_v<std::decay_t<T>, int> ||
            std::is_same_v<
                Integer,
                shared_ptr_t<std::decay_t<T>>>>>
    std::shared_ptr<Array> array(T&& n) {
        return array()->length(n);
    }

    template<typename T, typename E, typename CHECKER = 
        std::enable_if_t<
           (std::is_same_v<std::decay_t<T>, int> ||
            std::is_same_v<
                Integer,
                shared_ptr_t<std::decay_t<T>>>) &&
            std::is_base_of_v<
                Node,
                shared_ptr_t<std::decay_t<E>>>>>
    std::shared_ptr<Array> array(T&& n, E&& e) {
        return array(n)->fill(e);
    }
}


#endif