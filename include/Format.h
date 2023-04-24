
#ifndef FORMAT_H
#define FORMAT_H

#include <string>

class Array;
class Tuple;
class Tree;

class Formatter {
public:
    virtual void parse(const std::string& specifier, int n, ...);
    virtual void parse_start() = 0;
    virtual void parse_next() = 0;
    virtual bool parse_finish() = 0;
    virtual bool is_last() = 0;
protected:
    std::string fmt;
};

class SpecNotFoundException : public std::exception {
public:
    SpecNotFoundException(const std::string& spec);
private:
    std::string msg;
};

class ParseStack {
public:
    ParseStack(const std::string& cls);
    ~ParseStack();
private:
};

namespace Format {
    std::string class_parsing();
    std::string variable_parsing();
    void parse(Formatter* obj, const std::string& fmt, const std::string& class_name, bool ignore);
}

#endif