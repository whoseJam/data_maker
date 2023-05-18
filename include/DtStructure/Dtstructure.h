#ifndef DTSTRUCTURE_H
#define DTSTRUCTURE_H

#include <memory>
#include <string>

namespace mk {

class Lazytag;
class Mergeable;
class Info;

class Handle {
public:
    Handle(const std::string& name);
    virtual ~Handle() = default;
private:
    std::string type;
};

class Pushable {
    Pushable() = default;
    virtual ~Pushable() = default;
    virtual void push(std::shared_ptr<Lazytag>, std::shared_ptr<Handle>) = 0;
};

class Info {
public:
    Info() = default;
    virtual ~Info() = default;
};

class Mergeable : public Info {
public:
    Mergeable() = default;
    virtual ~Mergeable() = default;
    virtual void merge(std::shared_ptr<Mergeable> other) = 0;
};

class Lazytag {
public:
    Lazytag() = default;
    virtual ~Lazytag() = default;
};

typedef unsigned int uint;
class Hashable {
public:
    Hashable() = default;
    virtual ~Hashable() = default;
    virtual bool equal(std::shared_ptr<Hashable> other) = 0;
    virtual uint hash_code() = 0;
};

class Comparable {
public:
    Comparable() = default;
    virtual ~Comparable() = default;
    virtual int compare_to(std::shared_ptr<Comparable>) = 0;
};

}

#endif