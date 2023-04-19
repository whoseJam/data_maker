
#ifndef FORMAT_H
#define FORMAT_H

#include <string>

class Array;
class Tuple;
class Tree;

namespace Format {

    class Formatter {
    public:
        virtual void parse(const std::string& specifier, ...) = 0;
        virtual void parse_start() = 0;
        virtual void parse_next() = 0;
        virtual bool parse_finish() = 0;
    protected:
        std::string fmt;
    };
    std::string get_specifier(const std::string& str, int start);
    std::string get_content_in_brackets(const std::string& str, int start);
    void parse(const std::string& format, Formatter* obj);
}

#endif