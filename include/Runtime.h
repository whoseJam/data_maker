#ifndef RUNTIME_H
#define RUNTIME_H

#include <any>
#include <map>
#include <string>

class Runtime {
public:
    template<typename T, typename ...Args>
    T call(std::string key, Args... args) {
        auto key_func = funcs[key];
        auto func = any_cast<std::function<T(Args...)>>(key_func);
        return func(args...);
    }
private:
    std::map<std::string, std::any> funcs;
};

#endif