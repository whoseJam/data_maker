#ifndef TRIE_H
#define TRIE_H

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "Logger.h"
#include "Dtstructure.h"

namespace mk {

template<typename T, typename L = bool>
class Trie {
public:
    Trie();
    int insert(const std::string& str, T info);
    int insert(const std::string& str, T info, int ver);
    int insert(const std::string& str, L lazy);
    int insert(const std::string& str, L lazy, int ver);
    int remove(const std::string& str);
    int remove(const std::string& str, int ver);
    T* query(const std::string& str);
    T* query(const std::string& str, int ver);
private:
    struct TrieNode {
        TrieNode() = default;
        TrieNode(std::shared_ptr<T> info, std::shared_ptr<T> sum, std::shared_ptr<L> tag, std::map<char, std::shared_ptr<TrieNode>> children) : 
            info(std::move(info)), sum(std::move(sum)), tag(std::move(tag)), children(std::move(children)) {}
        std::shared_ptr<TrieNode> clone() {return std::make_shared<TrieNode>(info, sum, tag, children);}
        std::shared_ptr<T> info;
        std::shared_ptr<T> sum;
        std::shared_ptr<L> tag;
        std::map<char, std::shared_ptr<TrieNode>> children;
    };
    std::vector<std::shared_ptr<TrieNode>> roots;
    int cur_version;

    template<typename V, bool CHECKER = std::is_base_of_v<Mergeable, V>>
    struct MergeableHelper;
    template<typename V>
    struct MergeableHelper<V, true> {
        void update_the_path(std::vector<std::shared_ptr<TrieNode>>& path) {
            for (int i = path.size() - 1; i >= 0; i--) {
                auto cur = path[i];
                cur->sum = std::make_shared<T>(Zero());
                if (cur->info) cur->sum->merge(cur->info);
                for (auto child : cur->children) {
                    if (child.second->sum) {
                        cur->sum->merge(child.second->sum);
                    }
                }
            }
        }
    };
    template<typename V>
    struct MergeableHelper<V, false> {
        void update_the_path(std::vector<std::shared_ptr<TrieNode>>& path) {}
    };

    template<typename V, bool CHECKER = std::is_base_of_v<Lazytag, V>>
    struct LazytagHelper;
    template<typename V>
    struct LazytagHelper<V, true> {
        void update_the_path(std::vector<std::shared_ptr<TrieNode>>& path) {
            for (int i = 0; i < path.size(); i++) {
                auto cur = path[i];
                int sm = (cur->info ? cur->info->sum : 0);
                int smsm = (cur->sum ? cur->sum->sum : 0);
                if (!cur->tag) continue;
                auto nxt = (i == path.size() - 1) ? nullptr : path[i+1];
                for (auto& child : cur->children) {
                    if (child.second != nxt) child.second = child.second->clone();
                }
                push_tag_to_children(cur);
            }
        }
        void push_tag(std::shared_ptr<L> tag, std::shared_ptr<TrieNode> node) {
            if (!node->tag) node->tag = std::make_shared<L>(Zero());
            if (!node->sum) node->sum = std::make_shared<T>(Zero());
            if (!node->info) node->info = std::make_shared<T>(Zero());
            tag->push(node->tag);
            tag->push(node->sum);
            tag->push(node->info);
        }
        void push_tag_to_children(std::shared_ptr<TrieNode> fa) {
            if (!fa->tag) return;
            for (auto child : fa->children)
                push_tag(fa->tag, child.second);
            fa->tag = nullptr;
        }
    };
    template<typename V>
    struct LazytagHelper<V, false> {
        void update_the_path(std::vector<std::shared_ptr<TrieNode>>& path) {}
        void push_tag(std::shared_ptr<T> tag, std::shared_ptr<TrieNode> node) {}
        void push_tag_to_children(std::shared_ptr<TrieNode> fa) {}
    };
};

template<typename T, typename L>
Trie<T, L>::Trie() {
    roots.push_back(std::make_shared<TrieNode>());
    cur_version = 0;
}

template<typename T, typename L>
int Trie<T, L>::insert(const std::string& str, T info) {
    return insert(str, std::move(info), cur_version);
}

template<typename T, typename L>
int Trie<T, L>::insert(const std::string& str, T info, int ver) {
    auto info_ptr = std::make_shared<T>(std::move(info));
    auto lst = roots[ver];
    auto cur = lst->clone();
    cur_version++; roots.push_back(cur);
    std::vector<std::shared_ptr<TrieNode>> path;
    path.reserve(str.length());
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        path.push_back(cur);
        if (!lst || lst->children.find(c) == lst->children.end()) {
            auto tmp = std::make_shared<TrieNode>();
            cur->children[c] = tmp;
            cur = cur->children[c];
        } else {
            auto frm = lst->children[c];
            auto tmp = frm->clone();
            cur->children[c] = tmp;
            cur = cur->children[c];
            lst = lst->children[c];
        }
    }
    path.push_back(cur);
    LazytagHelper<L>().update_the_path(path);
    cur->info = info_ptr;
    MergeableHelper<T>().update_the_path(path);
    return cur_version;
}

template<typename T, typename L>
int Trie<T, L>::insert(const std::string& str, L lazy) {
    return insert(str, std::move(lazy), cur_version);
}

template<typename T, typename L>
int Trie<T, L>::insert(const std::string& str, L lazy, int ver) {
    auto lazy_ptr = std::make_shared<L>(std::move(lazy));
    auto lst = roots[ver];
    auto cur = lst->clone();
    cur_version++; roots.push_back(cur);
    std::vector<std::shared_ptr<TrieNode>> path;
    path.reserve(str.length());
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        path.push_back(cur);
        if (!lst || lst->children.find(c) == lst->children.end()) {
            auto tmp = std::make_shared<TrieNode>();
            cur->children[c] = tmp;
            cur = cur->children[c];
        } else {
            auto frm = lst->children[c];
            auto tmp = frm->clone();
            cur->children[c] = tmp;
            cur = cur->children[c];
            lst = lst->children[c];
        }
    }
    path.push_back(cur);
    LazytagHelper<L>().update_the_path(path);
    LazytagHelper<L>().push_tag(lazy_ptr, cur);
    LazytagHelper<L>().push_tag_to_children(cur);
    MergeableHelper<T>().update_the_path(path);
    return cur_version;
}

template<typename T, typename L>
int Trie<T, L>::remove(const std::string& str) {
    return remove(str, cur_version);
}

template<typename T, typename L>
int Trie<T, L>::remove(const std::string& str, int ver) {
    auto lst = roots[ver];
    auto cur = lst->clone();
    cur_version++; roots.push_back(cur);
    std::vector<std::shared_ptr<TrieNode>> path;
    path.reserve(str.length());
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        path.push_back(cur);
        if (!lst || lst->children.find(c) == lst->children.end())
            return cur_version;
        auto tmp = lst->children[c]->clone();
        cur->children[c] = tmp;
        cur = cur->children[c];
        lst = lst->children[c];
    }
    path.push_back(cur);
    LazytagHelper<L>().update_the_path(path);
    auto info = cur->info;
    cur->info = nullptr;
    MergeableHelper<T>().update_the_path(path);
    for (int i = path.size() - 1; i >= 1; i--) {
        cur = path[i];
        if (cur->children.size() == 0 && !cur->info) {
            auto fa = path[i - 1];
            char c = str[i - 1];
            fa->children.erase(c);
        } else break;
    }
    return cur_version;
}

template<typename T, typename L>
T* Trie<T, L>::query(const std::string& str) {
    return query(str, cur_version);
}

template<typename T, typename L>
T* Trie<T, L>::query(const std::string& str, int ver) {
    if (ver < 0 || ver > cur_version) MESSAGE("Trie", INVALID_VERSION(ver));
    auto cur = roots[ver];
    std::vector<std::shared_ptr<TrieNode>> path;
    path.reserve(str.length());
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        path.push_back(cur);
        if (cur->children.find(c) == cur->children.end())
            return nullptr;
        cur = cur->children[c];
    }
    path.push_back(cur);
    LazytagHelper<L>().update_the_path(path);
    if (cur->sum) return (cur->sum).get();
    return (cur->info).get();
}

}

#endif