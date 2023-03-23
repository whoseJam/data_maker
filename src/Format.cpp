
#include "../include/Format.h"
#include "../include/Array.h"
#include "../include/Tree.h"
#include <iostream>

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

    // 在条件被满足时，输出content
    void out_on_condition(const string& content, bool cond) {
        if (cond) {
            cout << content;
        }
    }

    void out_general_specifier(const string& fmt, int& pos, const FormatPackage& pack) {
        string specifier = get_specifier(fmt, pos);
        if (specifier == "nlast") {
            string nlast_content = get_content_in_brackets(fmt, pos + specifier.length() + 1);
            pos += 2 + nlast_content.length();
            out_on_condition(nlast_content, !pack.is_last_element);
        } else if (specifier == "last") {
            string last_content = get_content_in_brackets(fmt, pos + specifier.length() + 1);
            pos += 2 + last_content.length();
            out_on_condition(last_content, pack.is_last_element);
        }
    }

    // 判断specifier是否是一个通用specifier
    bool is_general_specifier(const string& specifier) {
        if (specifier == "last") return true;
        if (specifier == "nlast") return true;
        return false;
    }

    FormatPackage::FormatPackage(Array* node) {
        is_last_element = (node->__cur_iter == node->len - 1);
    }

    FormatPackage::FormatPackage(Tree* node) {
        is_last_element = (node->__cur_iter == node->vertex_num);
    }
}