#ifndef OPERATOR_H
#define OPERATOR_H

#include <memory>
#include <vector>

#include "Node.h"
#include "Debug.h"
#include "Logger.h"
#include "Define.h"

#define CHOOSE_TYPE_AS_V \
    typename V = \
    std::conditional_t< \
        std::is_same_v<std::decay_t<I>, Integer>, int, \
    std::conditional_t< \
        std::is_same_v<std::decay_t<I>, Character>, char, void>>

namespace mk {

class Integer;
class Character;

template<typename I, CHOOSE_TYPE_AS_V>
class Operator :
    public Node {
public:
    virtual V get() = 0;
};

template<typename I, CHOOSE_TYPE_AS_V>
class AddOperator :
    public Operator<I>,
    public std::enable_shared_from_this<AddOperator<I>>  {
public:
    AddOperator() = default;
    AddOperator(AddOperator<I> & other) {
        CALL(FUNCTION);
        if (!other.lc) MESSAGE("AddOperator<I>", NEED("lvalue"));
        if (!other.rc) MESSAGE("AddOperator<I>", NEED("rvalue"));
        this->lc = std::dynamic_pointer_cast<I>(other.lc->clone());
        this->rc = std::dynamic_pointer_cast<I>(other.rc->clone());
    }
    auto lvalue(std::shared_ptr<I> val) -> std::shared_ptr<AddOperator<I>> { CALL(FUNCTION); lc = val; return this->shared_from_this(); }
    auto rvalue(std::shared_ptr<I> val) -> std::shared_ptr<AddOperator<I>> { CALL(FUNCTION); rc = val; return this->shared_from_this(); }
    virtual V get() override { return lc->value() + rc->value(); }
    CL_CLONE_IN_CLASS(AddOperator<I>);
    virtual auto generate(bool re) -> void {
        CALL(FUNCTION);
        if (this->generated && !re) return;
        this->generated = true;
        lc->generate(re);
        rc->generate(re);
    }
    auto container() -> std::shared_ptr<I> {
        auto ref = container_.lock();
        if (!ref) {
            ref = std::make_shared<I>()->calculate(this->shared_from_this());
            container_ = ref;
        }
        return container_.lock();
    }
private:
    std::shared_ptr<I> lc;
    std::shared_ptr<I> rc;
    std::weak_ptr<I> container_;
};


template<typename I, CHOOSE_TYPE_AS_V>
class SubOperator :
    public Operator<I>,
    public std::enable_shared_from_this<SubOperator<I>>  {
public:
    SubOperator() = default;
    SubOperator(SubOperator<I> & other) {
        CALL(FUNCTION);
        if (!other.lc) MESSAGE("SubOperator<I>", NEED("lvalue"));
        if (!other.rc) MESSAGE("SubOperator<I>", NEED("rvalue"));
        this->lc = std::dynamic_pointer_cast<I>(other.lc->clone());
        this->rc = std::dynamic_pointer_cast<I>(other.rc->clone());
    }
    auto lvalue(std::shared_ptr<I> val) -> std::shared_ptr<SubOperator<I>> { CALL(FUNCTION); lc = val; return this->shared_from_this(); }
    auto rvalue(std::shared_ptr<I> val) -> std::shared_ptr<SubOperator<I>> { CALL(FUNCTION); rc = val; return this->shared_from_this(); }
    virtual V get() override {return lc->value() - rc->value();}
    CL_CLONE_IN_CLASS(SubOperator<I>);
    virtual auto generate(bool re) -> void {
        CALL(FUNCTION);
        if (this->generated && !re) return;
        this->generated = true;
        lc->generate(re);
        rc->generate(re);
    }
    auto container() -> std::shared_ptr<I> {
        auto ref = container_.lock();
        if (!ref) {
            ref = std::make_shared<I>()->calculate(this->shared_from_this());
            container_ = ref;
        }
        return container_.lock();
    }
private:
    std::shared_ptr<I> lc;
    std::shared_ptr<I> rc;
    std::weak_ptr<I> container_;
};


template<typename I, CHOOSE_TYPE_AS_V>
class MulOperator :
    public Operator<I>,
    public std::enable_shared_from_this<MulOperator<I>>  {
public:
    MulOperator() = default;
    MulOperator(const MulOperator<I> & other) {
        CALL(FUNCTION);
        if (!other.lc) MESSAGE("MulOperator<I>", NEED("lvalue"));
        if (!other.rc) MESSAGE("MulOperator<I>", NEED("rvalue"));
        this->lc = std::dynamic_pointer_cast<I>(other.lc->clone());
        this->rc = std::dynamic_pointer_cast<I>(other.rc->clone());
    }
    auto lvalue(std::shared_ptr<I> val) -> std::shared_ptr<MulOperator<I>> { CALL(FUNCTION); lc = val; return this->shared_from_this(); }
    auto rvalue(std::shared_ptr<I> val) -> std::shared_ptr<MulOperator<I>> { CALL(FUNCTION); rc = val; return this->shared_from_this(); }
    virtual V get() override { return lc->value() * rc->value(); }
    CL_CLONE_IN_CLASS(MulOperator<I>);
    virtual auto generate(bool re) -> void {
        CALL(FUNCTION);
        if (this->generated && !re) return;
        this->generated = true;
        lc->generate(re);
        rc->generate(re);
    }
    auto container() -> std::shared_ptr<I> {
        auto ref = container_.lock();
        if (!ref) {
            ref = std::make_shared<I>()->calculate(this->shared_from_this());
            container_ = ref;
        }
        return container_.lock();
    }
private:
    std::shared_ptr<I> lc;
    std::shared_ptr<I> rc;
    std::weak_ptr<I> container_;
};

template<typename T, typename V, typename I = shared_ptr_t<std::decay_t<T>>, typename CHECKER = 
    std::enable_if_t<
        std::is_same_v<
            shared_ptr_t<std::decay_t<T>>, 
            shared_ptr_t<std::decay_t<V>>> &&
       (std::is_same_v<Integer, I> || 
        std::is_same_v<Character, I>)>>
std::shared_ptr<I> operator +(T&& a, V&& b) {
    auto add = std::make_shared<AddOperator<I>>();
    auto a_move = a; add->lvalue(std::forward<T>(a_move));
    auto b_move = b; add->rvalue(std::forward<T>(b_move));
    return add->container();
}

template<typename T, typename V, typename I = shared_ptr_t<std::decay_t<T>>, typename CHECKER = 
    std::enable_if_t<
        std::is_same_v<
            shared_ptr_t<std::decay_t<T>>, 
            shared_ptr_t<std::decay_t<V>>> &&
       (std::is_same_v<Integer, I> ||
        std::is_same_v<Character, I>)>>
std::shared_ptr<I> operator -(T&& a, V&& b) {
    auto add = std::make_shared<SubOperator<I>>();
    auto a_move = a; add->lvalue(std::forward<T>(a_move));
    auto b_move = b; add->rvalue(std::forward<T>(b_move));
    return add->container();
}

template<typename T, typename V, typename I = shared_ptr_t<std::decay_t<T>>, typename CHECKER = 
    std::enable_if_t<
        std::is_same_v<
            shared_ptr_t<std::decay_t<T>>, 
            shared_ptr_t<std::decay_t<V>>> && 
       (std::is_same_v<Integer, I> || 
        std::is_same_v<Character, I>)>>
std::shared_ptr<I> operator *(T&& a, V&& b) {
    auto add = std::make_shared<MulOperator<I>>();
    auto a_move = a; add->lvalue(std::forward<T>(a_move));
    auto b_move = b; add->rvalue(std::forward<T>(b_move));
    return add->container();
}

}

#endif