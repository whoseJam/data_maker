#ifndef DTSTRUCTURE_H
#define DTSTRUCTURE_H

#include <memory>
#include <string>
#include <type_traits>

namespace mk {

class Lazytag;
class Mergeable;

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
};

template<typename T, typename L, typename H, typename V = void>
struct info_is_pushable_with_handle : std::false_type {};
template<typename T, typename L, typename H>
struct info_is_pushable_with_handle<T, L, H, std::void_t<
    decltype(std::declval<T>().push(std::declval<L*>(), std::declval<H*>()))
>> : std::true_type {};
template<typename T, typename L, typename H>
inline constexpr bool info_is_pushable_with_handle_v = info_is_pushable_with_handle<T, L, H>::value;

template<typename T, typename L, typename V = void>
struct info_is_pushable_without_handle : std::false_type {};
template<typename T, typename L>
struct info_is_pushable_without_handle<T, L, std::void_t<
    decltype(std::declval<T>().push(std::declval<L*>()))
>> : std::true_type {};
template<typename T, typename L>
inline constexpr bool info_is_pushable_without_handle_v = info_is_pushable_without_handle<T, L>::value;


/*
Descrption:
Mergeable可以实现两种接口:
void merge(ThisType*, HandleType*);
void merge(ThisType*);
如果需要接受push的消息，还可以选择实现:
void push(LazytagType*, HandleType*);
void push(LazytagType*);
*/
class Mergeable {
public:
    Mergeable() = default;
    virtual ~Mergeable() = default;
};

template<typename T, typename H, typename V = void>
struct info_is_mergeable_with_handle : std::false_type {};
template<typename T, typename H>
struct info_is_mergeable_with_handle<T, H, std::void_t<
    decltype(std::declval<T>().merge(std::declval<T*>(), std::declval<H*>()))
>> : std::true_type {};
template<typename T, typename H>
inline constexpr bool info_is_mergeable_with_handle_v = info_is_mergeable_with_handle<T, H>::value;

template<typename T, typename V = void>
struct info_is_mergeable_without_handle : std::false_type {};
template<typename T>
struct info_is_mergeable_without_handle<T, std::void_t<
    decltype(std::declval<T>().merge(std::declval<T*>()))
>> : std::true_type {};
template<typename T>
inline constexpr bool info_is_mergeable_without_handle_v = info_is_mergeable_without_handle<T>::value;

/*
Description:
Lazytag可以实现两种接口:
void push(ThisType*, HandleType*);
vodi push(ThisType*);
*/
class Lazytag {
public:
    Lazytag() = default;
    virtual ~Lazytag() = default;
};

template<typename L, typename H, typename V = void>
struct lazytag_is_pushable_with_handle : std::false_type {};
template<typename L, typename H>
struct lazytag_is_pushable_with_handle<L, H, std::void_t<
    decltype(std::declval<L>().push(std::declval<L*>(), std::declval<H*>()))
>> : std::true_type {};
template<typename L, typename H>
inline constexpr bool lazytag_is_pushable_with_handle_v = lazytag_is_pushable_with_handle<L, H>::value;

template<typename L, typename V = void>
struct lazytag_is_pushable_without_handle : std::false_type {};
template<typename L>
struct lazytag_is_pushable_without_handle<L, std::void_t<
    decltype(std::declval<L>().push(std::declval<L*>()))
>> : std::true_type {};
template<typename L>
inline constexpr bool lazytag_is_pushable_without_handle_v = lazytag_is_pushable_without_handle<L>::value;

typedef unsigned int uint;
class Hashable {
public:
    Hashable() = default;
    virtual ~Hashable() = default;
    virtual bool equal(Hashable* other) = 0;
    virtual uint hash_code() = 0;
};

/*
Description:
需要实现:
int compara_to(ThisType*);
*/
class Comparable {
public:
    Comparable() = default;
    virtual ~Comparable() = default;
};

template<typename T, typename V = void>
struct is_comparable : std::false_type {};
template<typename T>
struct is_comparable<T, std::void_t<
    decltype(std::declval<T>().compare_to(std::declval<T*>))
>> : std::true_type {};
template<typename T>
inline constexpr bool is_comparable_v = is_comparable<T>::value;


}

#endif