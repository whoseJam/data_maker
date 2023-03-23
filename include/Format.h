
#ifndef FORMAT_H
#define FORMAT_H

#include <string>

class Array;
class Tree;

namespace Format {
    struct FormatPackage {
    public:
        FormatPackage(Array* node);
        FormatPackage(Tree* node);
        bool is_last_element;
    };

    std::string get_specifier(const std::string& str, int start);
    std::string get_content_in_brackets(const std::string& str, int start);
    void out_on_condition(const std::string& content ,bool cond);
    void out_general_specifier(const std::string& fmt, int& pos, const FormatPackage& pack);
    bool is_general_specifier(const std::string& specifier);
}

#endif