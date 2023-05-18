
#include <memory>
#include <vector>
#include <iostream>

#include "Debug.h"
#include "Format.h"
#include "Define.h"
#include "Logger.h"

using namespace std;

namespace mk {

Formatable::Formatable() {
    CALL(FUNCTION);
    fmt = "UNSET";
}

Formatable::Formatable(const Formatable& other) {
    CALL(FUNCTION);
    fmt = other.fmt;
}

void Formatable::parse(const string& spec, int n, ...) {
    CALL(FUNCTION);
    va_list valist;
    va_start(valist, n);
    if (spec == SPEC_LAST) {
        if (!IN_RANGE(1, n, 1)) MESSAGE("Tree", FUNC_ARGS_MISMATCH(n, 1, 1));
        shared_ptr<Iterable> obj = dynamic_pointer_cast<Iterable>(shared_from_this());
        if ((obj && obj->iter_at_last()) || (!obj)) {
            char* msg = va_arg(valist, char*);
            cout << msg;
        }
    } else if (spec == SPEC_NLAST) {
        if (!IN_RANGE(1, n, 1)) MESSAGE("Tree", FUNC_ARGS_MISMATCH(n, 1, 1));
        shared_ptr<Iterable> obj = dynamic_pointer_cast<Iterable>(shared_from_this());
        if (obj && !obj->iter_at_last()) {
            char* msg = va_arg(valist, char*);
            cout << msg;
        }
    } else {
        throw SpecNotFoundException(spec);
    }
}

SpecNotFoundException::SpecNotFoundException(const string& spec) {
    msg = spec + " not found";
}

string get_specifier(const string& str, int start) {
    string ans = "";
    for (int i = start + 1; i < str.length(); i++) {
        if (islower(str[i]) || isupper(str[i])) {
            ans.push_back(str[i]);
        } else break;
    }
    return ans;
}

// 获取中括号中的内容，即[content]，其中start从'['的下标处开始
string get_content_in_brackets(const string& str, int start) {
    string ans = "";
    for (int i = start + 1; i < str.length(); i++) {
        if (str[i] == ']') break;
        ans.push_back(str[i]);
    }
    return ans;
}

bool in_charset(char c, const string& cset) {
    for (int i = 0; i < cset.length(); i++) {
        if (cset[i] == c) return true;
    }
    return false;
}

string get_till(const string& str, int start, const string& cset) {
    string ans = "";
    for (int i = start; i < str.length(); i++) {
        if (in_charset(str[i], cset)) break;
        ans.push_back(str[i]);
    }
    return ans;
}

void parse_args(const string& fmt, int& pos, vector<string>& args) {
    if (pos + 1 >= fmt.length() || fmt[pos + 1] != '[') return;
    int top = 0;
    string arg;
    for (int i = pos + 1; i <= fmt.length(); i++) {
        if (fmt[i] == '[') {
            top++;
            if (top > 1) arg.push_back(fmt[i]);
        } else if (fmt[i] == ']') {
            top--;
            if (top > 0) arg.push_back(fmt[i]);
            else if (top == 0) {
                args.push_back(arg);
                pos = i;
                return;
            }
        } else if (fmt[i] == ',') {
            if (top == 1) {
                args.push_back(arg);
                arg.clear();
            } else {
                arg.push_back(fmt[i]);
            }
        } else arg.push_back(fmt[i]);
    }
    MESSAGE("Formatable", "parse format error");
}

/*
Description:
    解析一个$specifier[...]，并将参数丢给Formatter.parse(spec,...)
Pre-Condition:
    pos位于$上
Post-Condition:
    pos移动到$specifier[...]刚好结束的位置
*/
void parse_specifier(const string& fmt, int& pos, shared_ptr<Formatable> obj) {
    string specifier = get_specifier(fmt, pos);
    pos += specifier.length();
    vector<string> args;
    parse_args(fmt, pos, args);
    if (args.size() == 0) obj->parse(specifier, 0);
    if (args.size() == 1) obj->parse(specifier, 1, args[0].c_str());
    if (args.size() == 2) obj->parse(specifier, 2, args[0].c_str(), args[1].c_str());
}

/*
Description:
    解析一个Formatter
Pre-Condition:
    fmt不应该为UNSET
*/
void Formatable::parse(shared_ptr<Formatable> obj, const string& fmt, const string& cls) {
    CALL(FUNCTION);
    if (fmt == "UNSET") MESSAGE(cls, "format is unset");
    shared_ptr<Iterable> iter_obj = dynamic_pointer_cast<Iterable>(obj);
    if (iter_obj) {
        iter_obj->iter_reset();
        while (!iter_obj->iter_loop_finish()) {
            for (int i = 0; i < fmt.length(); i++) {
                if (fmt[i] == '$') parse_specifier(fmt, i, obj);
                else cout << fmt[i];
            }
            iter_obj->iter_next();
        }
    } else {
        for (int i = 0; i < fmt.length(); i++) {
            if (fmt[i] == '$') parse_specifier(fmt, i, obj);
            else cout << fmt[i];
        }
    }
}

}