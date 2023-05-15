#ifndef DTSTRUCTURE_H
#define DTSTRUCTURE_H

#include <memory>

class Zero final {
public:
    Zero() = default;
};

class Info {
public:
    Info() = default;
    virtual ~Info() = default;
};

class Mergeable {
public:
    Mergeable() = default;
    Mergeable(Zero); 
    virtual ~Mergeable() = default;
    virtual void merge(std::shared_ptr<Mergeable> other) = 0;
};

class Lazytag {
public:
    Lazytag() = default;
    Lazytag(Zero);
    virtual ~Lazytag() = default;
    virtual void push(std::shared_ptr<Lazytag>) = 0;
    virtual void push(std::shared_ptr<Mergeable>) = 0;
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

#define PUSHUP(x) MergeableHelper<T>().push_up(x)
#define PUSHDOWN(x) LazytagHelper<T>().push_down(x)

#endif