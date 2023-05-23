#ifndef DTSTRUCTURE_H
#define DTSTRUCTURE_H

#include <memory>
#include <string>

namespace mk {

class Lazytag;
class Mergeable;
class Info;

class Empty {
public:
};

class Handle {
public:
    Handle() = default;
    virtual ~Handle() = default;
private:
};

class Pushable {
public:
    Pushable() = default;
    virtual ~Pushable() = default;
    virtual void push(Lazytag*, Handle*) = 0;
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
    virtual auto merge(Mergeable* other) -> void = 0;
};

class Lazytag : public Pushable {
public:
    Lazytag() = default;
    virtual ~Lazytag() = default;
};

typedef unsigned int uint;
class Hashable {
public:
    Hashable() = default;
    virtual ~Hashable() = default;
    virtual bool equal(Hashable* other) = 0;
    virtual uint hash_code() = 0;
};

class Comparable {
public:
    Comparable() = default;
    virtual ~Comparable() = default;
    virtual int compare_to(Comparable* other) = 0;
};

}

#endif