#ifndef SEARCH_H
#define SEARCH_H

#include <memory>
#include <queue>

#include "HashMap.h"

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

#endif