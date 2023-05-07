#ifndef OPERATOR_H
#define OPERATOR_H

#include <memory>
#include <vector>

#include "Node.h"
#include "Clone.h"
#include "Logger.h"
#include "Define.h"

class Integer;
class Character;

#define CHOOSE_TYPE_AS_V \
    typename V = \
    std::conditional_t< \
        std::is_same_v<std::decay_t<I>, Integer>, int, \
    std::conditional_t< \
        std::is_same_v<std::decay_t<I>, Character>, char, void>>

#define LC_RC_BODY(class) \
    virtual void generate(bool re, std::shared_ptr<Node> from) override { \
        CALL(#class, "generate"); \
        this->from_node = from; \
        if (this->generated && !re) return; \
        this->generated = true; \
        lc->generate(re, std::dynamic_pointer_cast<Node>(this->shared_from_this())); \
        rc->generate(re, std::dynamic_pointer_cast<Node>(this->shared_from_this())); \
    } \
    virtual std::shared_ptr<Node> clone() { \
        CALL(#class, "clone"); \
        Clone::get()->enter(std::dynamic_pointer_cast<Node>(this->shared_from_this())); \
        struct CloneGuard { ~CloneGuard() {Clone::get()->exit();}} cg; \
        if (Clone::get()->check_stay_with(this->parent)) { \
            if (!Clone::get()->check(this)) \
                Clone::get()->insert(this, std::make_shared<class>(*this)); \
            return Clone::get()->check(this); \
        } \
        return std::make_shared<class>(*this); \
    } \
    virtual void out() override {std::cout << get();} \
    std::shared_ptr<I> get_container() { \
        if (!container) container = std::make_shared<I>()->calculate(this->shared_from_this()); \
        return container; \
    } 

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
        CALL("AddOperator<I>", "AddOperator<I>");
        if (!other.lc) MESSAGE("AddOperator<I>", NEED("lvalue"));
        if (!other.rc) MESSAGE("AddOperator<I>", NEED("rvalue"));
        this->lc = std::dynamic_pointer_cast<I>(other.lc->clone());
        this->rc = std::dynamic_pointer_cast<I>(other.rc->clone());
    }
    CL_UPDATE_FUNC(AddOperator<I>, lvalue, lc, UF_assign, CK_base_is(Node), );
    CL_UPDATE_FUNC(AddOperator<I>, rvalue, rc, UF_assign, CK_base_is(Node), );
    virtual V get() override {
        return lc->get() + rc->get();
    }
    LC_RC_BODY(AddOperator<I>);
private:
    std::shared_ptr<I> lc;
    std::shared_ptr<I> rc;
    std::shared_ptr<I> container;
};


template<typename I, CHOOSE_TYPE_AS_V>
class SubOperator :
    public Operator<I>,
    public std::enable_shared_from_this<SubOperator<I>>  {
public:
    SubOperator() = default;
    SubOperator(SubOperator<I> & other) {
        CALL("SubOperator<I>", "SubOperator<I>");
        if (!other.lc) MESSAGE("SubOperator<I>", NEED("lvalue"));
        if (!other.rc) MESSAGE("SubOperator<I>", NEED("rvalue"));
        this->lc = std::dynamic_pointer_cast<I>(other.lc->clone());
        this->rc = std::dynamic_pointer_cast<I>(other.rc->clone());
    }
    CL_UPDATE_FUNC(SubOperator<I>, lvalue, lc, UF_assign, CK_base_is(Node), );
    CL_UPDATE_FUNC(SubOperator<I>, rvalue, rc, UF_assign, CK_base_is(Node), );
    virtual V get() override {return lc->get() - rc->get();}
    LC_RC_BODY(SubOperator<I>);
private:
    std::shared_ptr<I> lc;
    std::shared_ptr<I> rc;
    std::shared_ptr<I> container;
};


template<typename I, CHOOSE_TYPE_AS_V>
class MulOperator :
    public Operator<I>,
    public std::enable_shared_from_this<MulOperator<I>>  {
public:
    MulOperator() = default;
    MulOperator(const MulOperator<I> & other) {
        CALL("MulOperator<I>", "MulOperator<I>");
        if (!other.lc) MESSAGE("MulOperator<I>", NEED("lvalue"));
        if (!other.rc) MESSAGE("MulOperator<I>", NEED("rvalue"));
        this->lc = std::dynamic_pointer_cast<I>(other.lc->clone());
        this->rc = std::dynamic_pointer_cast<I>(other.rc->clone());
    }
    CL_UPDATE_FUNC(MulOperator<I>, lvalue, lc, UF_assign, CK_base_is(Node), );
    CL_UPDATE_FUNC(MulOperator<I>, rvalue, rc, UF_assign, CK_base_is(Node), );
    virtual V get() override {return lc->get() * rc->get();}
    LC_RC_BODY(MulOperator<I>);
private:
    std::shared_ptr<I> lc;
    std::shared_ptr<I> rc;
    std::shared_ptr<I> container;
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
    return add->get_container();
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
    return add->get_container();
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
    return add->get_container();
}

#endif