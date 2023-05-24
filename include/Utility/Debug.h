#ifndef DEBUG_H
#define DEBUG_H

#include <list>
#include <stack>
#include <string>
#include <memory>

// #define DEBUG

inline std::string className(const std::string& prettyFunction) {
    size_t colons = prettyFunction.find("::");
    if (colons == std::string::npos)
        return"::";
    size_t begin = prettyFunction.substr(0,colons).rfind("") + 1;
    size_t end = colons - begin;
    return prettyFunction.substr(begin,end);
}

#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)
#define CLASS __CLASS_NAME__
#define FUNCTION __PRETTY_FUNCTION__
#ifdef DEBUG
    #define CALL(func) Call guard(func);
#else
    #define CALL(func)
#endif

namespace mk {

class Call {
public:
    Call(const std::string& fun);
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

}

#endif