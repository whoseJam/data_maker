
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



#include <map>
#include <memory>


namespace mk {

namespace Hidden {
template<typename T>
struct HashNode {
    int cnt = 0;
    T* hold;
    std::shared_ptr<HashNode> next;
};

}

template<typename T>
class HashMap {
public:
    HashMap();
    void insert(const T& obj);
    void insert(T&& obj);
    void remove(const T& obj);
    bool contain(const T& obj);
    int query_total();
    int query_different();
private:
    using MyHashNode = Hidden::HashNode<T>;
    std::map<uint, std::shared_ptr<MyHashNode>> header;
    int diff;
    int total;
};

template<typename T>
HashMap<T>::HashMap() {
    diff = 0;
    total = 0;
}

template<typename T>
void HashMap<T>::insert(const T& obj_) {
    auto obj = const_cast<T&>(obj_);
    uint code = obj.hash_code();
    if (header.find(code) == header.end() || !header[code]) {
        auto new_node = std::make_shared<MyHashNode>();
        header[code] = new_node;
        new_node->cnt = 1;
        new_node->hold = new T(obj);
        diff++; total++;
    } else {
        auto iter = header[code];
        while (true) {
            if (iter->hold->equal(obj)) {
                iter->cnt++;
                total++;
                return;
            }
            if (iter->next) iter = iter->next;
            else break;
        }
        iter->next = std::make_shared<MyHashNode>();
        iter->next->cnt = 1;
        iter->next->hold = new T(obj);
        diff++; total++;
    }
}

template<typename T>
void HashMap<T>::insert(T&& obj) {
    uint code = obj.hash_code();
    if (header.find(code) == header.end() || !header[code]) {
        auto new_node = std::make_shared<MyHashNode>();
        header[code] = new_node;
        new_node->cnt = 1;
        new_node->hold = new T(std::move(obj));
        diff++; total++;
    } else {
        auto iter = header[code];
        while (true) {
            if (iter->hold->equal(obj)) {
                iter->cnt++;
                total++;
                return;
            }
            if (iter->next) iter = iter->next;
            else break;
        }
        iter->next = std::make_shared<MyHashNode>();
        iter->next->cnt = 1;
        iter->next->hold = new T(std::move(obj));
        diff++; total++;
    }
}

template<typename T>
void HashMap<T>::remove(const T& obj_) {
    T& obj = const_cast<T&>(obj_);
    uint code = obj.hash_code();
    if (header.find(code) == header.end() || !header[code]) return;
    auto iter = header[code];
    if (iter->hold->equal(obj)) {
        iter->cnt--; total--;
        if (iter->cnt == 0) {
            header[code] = iter->next;
            diff--;
        }
        return;
    }
    iter = iter->next;
    if (!iter) return;
    auto last = header[code];
    while (true) {
        if (iter->hold->equal(obj)) {
            iter->cnt--; total--;
            if (iter->cnt == 0) {
                last->next = iter->next;
                delete iter->hold;
                diff--;
            }
            return;
        }
        iter = iter->next;
        last = last->next;
        if (!iter) break;
    }
}

template<typename T>
bool HashMap<T>::contain(const T& obj_) {
    T& obj = const_cast<T&>(obj_);
    uint code = obj.hash_code();
    if (header.find(code) == header.end() || !header[code]) return false;
    auto iter = header[code];
    while (true) {
        if (iter->hold->equal(obj))
            return true;
        iter = iter->next;
        if (!iter) break;
    }
    return false;
}

template<typename T>
int HashMap<T>::query_total() {
    return total;
}

template<typename T>
int HashMap<T>::query_different() {
    return diff;
}

}



#include <memory>
#include <queue>


namespace mk {

extern int COUNT_SEARCHSTATUS;
extern int COUNT_SEARCHNODE;

/*
Description:
需要实现以下函数:
StatusType* start_status()，用于给出搜索问题的起始状态.
bool is_end(StatusType*)，用于判断某个状态是否是一个终止状态.
bool finish()，用于判断搜索问题是否已经结束.
void operate_on(StatusType*)，用于处理搜索到的终止状态.
*/
struct SearchProblem {
public:
    SearchProblem() = default;
    virtual ~SearchProblem() = default;
};

/*
Description:
需要实现以下函数:
std::vector<StatusType*> successors()，用于给出一个状态的所有后继状态.
uint hash_code()，用于给出一个状态的hash编码.
bool equal(const StatusType&)，用于判断该状态是否和另外某个状态相等.
*/
struct SearchStatus {
public:
    SearchStatus() = default;
    virtual ~SearchStatus() = default;
};

namespace Hidden {

template<typename S>
struct SearchStatus {
    SearchStatus(S* s) : stat(s) {}
    uint hash_code() { return stat->hash_code(); }
    bool equal(const SearchStatus& other) { return stat->equal(*other.stat); }
    S* stat;
};

template<typename S>
struct SearchNode {
    SearchNode(int pri, S* s) :
        priority(pri), stat(s) {}
    int priority;
    S* stat;
};

template<typename S>
bool operator <(const SearchNode<S>& a, const SearchNode<S>& b) {
    return a.priority < b.priority;
}

}

enum SearchType {
    DFS, BFS, AStar
};

template<typename S, typename P>
class SearchAgent {
public:
    SearchAgent(int type) : type(type) {};
    void solve();
private:
    using MySearchNode = Hidden::SearchNode<S>;
    using MySearchStatus = Hidden::SearchStatus<S>;
    int type;
};

template<typename S, typename P>
void SearchAgent<S, P>::solve() {
    std::priority_queue<MySearchNode> Q;
    HashMap<MySearchStatus> visited;

    auto start = P::start_status();
    Q.push(MySearchNode(0, start));
    visited.insert(MySearchStatus(start));

    while(Q.size()) {
        MySearchNode cur = Q.top(); Q.pop();
        S* stat = cur.stat;
        if (P::is_end(stat)) {
            P::operate_on(stat);
            if (P::finish()) break;
        }

        for (S* nxt : stat->successors()) {
            if (visited.contain(MySearchStatus(nxt))) continue;
            visited.insert(MySearchStatus(nxt));
            if (type == SearchType::DFS) {
                Q.push(MySearchNode(cur.priority + 1, nxt));
            } else if (type == SearchType::BFS) {
                Q.push(MySearchNode(cur.priority - 1, nxt));
            }
        }
    }
}

}


#include <iostream>
#include <cstring>


using namespace mk;
using namespace std;

int n;

struct PermutationStatus {
    int len;
    int ans[10];
    PermutationStatus() { len = 0; }
    PermutationStatus(const PermutationStatus& other) {
        len = other.len;
        memcpy(ans, other.ans, sizeof(ans));
    }
    auto successors() -> vector<PermutationStatus*> {
        vector<PermutationStatus*> ret;
        bool vis[10] = {0};
        for (int i = 1; i <= len; i++)
            vis[ans[i]] = true;
        for (int i = 1; i <= n; i++) {
            if (!vis[i]) {
                PermutationStatus* nxt = new PermutationStatus(*this);
                nxt->ans[len + 1] = i;
                nxt->len = len + 1;
                ret.push_back(nxt);
            }
        }
        return ret;
    }
    uint hash_code() {
        uint ret = len;
        for (int i = 1; i <= len; i++) ret = ret * 10 + ans[i];
        return ret;
    }
    bool equal(const PermutationStatus& other) {
        if (len != other.len) return false;
        for (int i = 1; i <= len; i++)
            if (ans[i] != other.ans[i]) return false;
        return true;
    }
};

struct PermutationProblem : public SearchProblem {
    static PermutationStatus* start_status() { return new PermutationStatus(); }
    static void operate_on(PermutationStatus* stat) {
        for (int i = 1; i <= stat->len; i++)
            cout << stat->ans[i] << ' ';
        cout << "\n";
    }
    static bool is_end(PermutationStatus* stat) { return stat->len == n; }
    static bool finish() { return false; }
};

void test() {
    n = 3;
    SearchAgent<PermutationStatus, PermutationProblem> agent1(SearchType::DFS);
    agent1.solve();
    SearchAgent<PermutationStatus, PermutationProblem> agent2(SearchType::BFS);
    agent2.solve();
}

int main(int argc, char **argv) {
    test();
    return 0;
}

namespace mk {

}


namespace mk {

}
