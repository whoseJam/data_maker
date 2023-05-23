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

namespace mk {

extern int COUNT_ARRAY;

class Array : 
    public Node, 
    public Iterable,
    public Formatable {
public:
    Array();
    Array(const Array& other);
    virtual ~Array();
    auto length(int len) -> std::shared_ptr<Array>;
    auto length(std::shared_ptr<Integer> len) -> std::shared_ptr<Array>;
    auto fill(std::shared_ptr<Node> ele) -> std::shared_ptr<Array>;
    auto format(const char* fmt) -> std::shared_ptr<Array>;
    auto before_generate(std::function<void(std::shared_ptr<Array>)>) -> std::shared_ptr<Array>;
    auto when_generating(std::function<void(std::shared_ptr<Array>, int)>) -> std::shared_ptr<Array>;
    auto after_generate(std::function<void(std::shared_ptr<Array>)>) -> std::shared_ptr<Array>;

    int length();
    template<typename T> 
    std::shared_ptr<T> get(int idx) {
        CALL(FUNCTION);
        if (idx < 0 || idx >= elements.size()) MESSAGE("Array", INDEX_OUT_OF_BOUNDARY);
        std::shared_ptr<T> ans = std::dynamic_pointer_cast<T>(elements[idx]);
        if (!ans) MESSAGE("Array", TYPE_ERROR);
        return ans;
    }

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone(bool first) override;

    virtual void iter_reset() override;
    virtual void iter_next() override;
    virtual bool iter_loop_finish() override;
    virtual bool iter_at_last() override;

    virtual void parse(const std::string& spec, int n, ...) override;
    virtual void out() override;
private:
    std::function<void(std::shared_ptr<Array>)> callback_before_generate;
    std::function<void(std::shared_ptr<Array>, int)> callback_when_generating;
    std::function<void(std::shared_ptr<Array>)> callback_after_generate;
    std::shared_ptr<Integer> len;
    std::shared_ptr<Node> template_ele;
    std::vector<std::shared_ptr<Node>> elements;
    int cur_iter;
};

auto array() -> std::shared_ptr<Array>;

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