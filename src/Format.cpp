
#include <stack>
#include <iostream>

#include "../include/Tree.h"
#include "../include/Tuple.h"
#include "../include/Array.h"
#include "../include/Format.h"
#include "../include/Define.h"

using namespace std;
using namespace Format;

void Formatter::parse(const string& spec, int n, ...) {
    va_list valist;
    va_start(valist, n);
    if (spec == SPEC_LAST) {
        CHECK_FUNCTION_ARGS_MATCH(class_parsing(), spec, n, 1);
        if (is_last()) {
            char* msg = va_arg(valist, char*);
            cout << msg;
        }
    } else if (spec == SPEC_NLAST) {
        CHECK_FUNCTION_ARGS_MATCH(class_parsing(), spec, n, 1);
        if (!is_last()) {
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

namespace Format {
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

    /*
    Description:
        解析一个$specifier[...]，并将参数丢给Formatter.parse(spec,...)
    Pre-Condition:
        pos位于$上
    Post-Condition:
        pos移动到$specifier[...]刚好结束的位置
    */
    void parse_specifier(const string& fmt, int& pos, Formatter* obj) {
        string specifier = get_specifier(fmt, pos);
        pos += specifier.length();
        vector<string> args;
        if (fmt[pos + 1] == '[') {
            pos += 2;
            while (true) {
                string arg = get_till(fmt, pos, ",]");
                pos += arg.length();
                args.push_back(arg);
                if (fmt[pos] == ']') break;
            }
        }
        if (args.size() == 0) obj->parse(specifier, 0);
        if (args.size() == 1) obj->parse(specifier, 1, args[0].c_str());
        if (args.size() == 2) obj->parse(specifier, 2, args[0].c_str(), args[1].c_str());
    }

    

    stack<string> class_stack;
    stack<string> variable_stack;

    string class_parsing() {
        if (class_stack.empty()) return "UNSET";
        return class_stack.top();
    }

    string variable_parsing() {
        if (variable_stack.empty()) return "UNSET";
        return variable_stack.top();
    }

    /*
    Description:
        解析一个Formatter
    Pre-Condition:
        fmt不应该为UNSET
    */
    void parse(Formatter* obj, const string& fmt, const string& class_name, bool ignore_non_spec) {
        CHECK_STRING_UNSET(class_name, fmt);
        class_stack.push(class_name);
        obj->parse_start();
        while (!obj->parse_finish()) {
            for (int i = 0; i < fmt.length(); i++) {
                if (fmt[i] == '$') {
                    parse_specifier(fmt, i, obj);
                } else if (!ignore_non_spec) {
                    cout << fmt[i];
                }
            }
            obj->parse_next();
        }
        class_stack.pop();
    }
}

ParseStack::ParseStack(const string& cls) {
    Format::class_stack.push(cls);
}

ParseStack::~ParseStack() {
    Format::class_stack.pop();
}