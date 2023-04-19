
#include <iostream>

#include "../include/Tree.h"
#include "../include/Tuple.h"
#include "../include/Array.h"
#include "../include/Format.h"
#include "../include/Define.h"

using namespace std;

namespace Format {
    string get_specifier(const string& str, int start) {
        string ans = "";
        for (int i = start + 1; i < str.length(); i++) {
            if (islower(str[i])) {
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
        if (args.size() == 0) obj->parse(specifier);
        if (args.size() == 1) obj->parse(specifier, args[0].c_str());
        if (args.size() == 2) obj->parse(specifier, args[0].c_str(), args[1].c_str());
    }

    /*
    Description:
        解析一个Formatter
    Pre-Condition:
        fmt不应该为UNSET
    */
    void parse(const string& fmt, Formatter* obj) {
        obj->parse_start();
        while (!obj->parse_finish()) {
            for (int i = 0; i < fmt.length(); i++) {
                if (fmt[i] == '$') {
                    string spec = get_specifier(fmt, i);
                    parse_specifier(fmt, i, obj);
                } else {
                    cout << fmt[i];
                }
            }
            obj->parse_next();
        }
    }
}