#ifndef DEBUG_H
#define DEBUG_H

#include <list>
#include <stack>
#include <string>
#include <memory>

#define DEBUG

#ifdef DEBUG
    #define CALL(class, func) Call guard(class, func);
#else
    #define CALL(class, func)
#endif

class Call {
public:
    Call(const std::string& cls, const std::string& fun);
    ~Call();
};

class CallStack {
public:
    static CallStack* get();
    void enter(const std::string& cls, const std::string& fun);
    void exit();
    void print();
private:
    static CallStack instance;
    std::list<std::string> cls;
    std::list<std::string> func;
};

#endif