
#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <memory>

namespace mk {

class Iterable {
public:
    virtual void iter_reset() = 0;
    virtual void iter_next() = 0;
    virtual bool iter_loop_finish() = 0;
    virtual bool iter_at_last() = 0;
};

class Formatable :
    public std::enable_shared_from_this<Formatable> {
public:
    Formatable();
    Formatable(const Formatable& other);
    virtual void parse(const std::string& spec, int n, ...);
    static void parse(std::shared_ptr<Formatable> obj, const std::string& fmt, const std::string& cls);
    virtual void out() = 0;
protected:
    const char* fmt;
};

class SpecNotFoundException : public std::exception {
public:
    SpecNotFoundException(const std::string& spec);
private:
    std::string msg;
};

}

#endif