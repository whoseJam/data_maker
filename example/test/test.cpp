#include <functional>
#include <iostream>
#include <string>
#include <any>
#include <map>
using namespace std;

class Runtime {
public:

    template<typename T, typename ...Args>
    T call(string key, Args... args) {
        auto fk = funcs[key];
        auto func = any_cast<std::function<T(Args...)>>(fk);
        return func(args...);
    }

    map<string, any> funcs;
};

int sum=10086;
int getsum() {
    return sum;
}
void add(int x) {
    sum+=x;
}

int main() {
    Runtime a;
    auto get_func = std::function<int()>(getsum);
    a.funcs["int"] = 1;
    a.funcs["getsum"] = get_func;
    // a.funcs["get"] = std::function<void(int)>(add);
    auto b = a.funcs["int"];
    cout << any_cast<int>(b) << "\n";
    auto c = a.funcs["getsum"];
    cout<<"yesyes\n";
    auto d = any_cast<decltype(get_func)>(c);
    cout << d() << "\n";
    cout<<"yes\n";
    a.call<int>("getsum");
    // cout << a.call<int>("get");
    return 0;
}