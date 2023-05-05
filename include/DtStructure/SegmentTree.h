#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

#include <memory>
#include <vector>
#include <functional>

class Lazytag {
};

class Mergeable {
};

class SegmentTree {
public:
    SegmentTree(int l, int r);
    std::shared_ptr<SegmentTree> on_push_down(std::function<void(std::shared_ptr<Lazytag>, std::shared_ptr<Mergeable>)> callback);
    std::shared_ptr<SegmentTree> on_push_up(std::function<void(std::shared_ptr<Mergeable>, std::shared_ptr<Mergeable>, std::shared_ptr<Mergeable>)>);
    
    void insert(int pos, std::shared_ptr<Mergeable> info);
    void insert(int pos, std::shared_ptr<Lazytag> tag);
    void insert(int l, int r, std::shared_ptr<Lazytag> tag);
    std::shared_ptr<Mergeable> query(int pos);
    std::shared_ptr<Mergeable> query(int l, int r);
private:
    struct SegmentNode {
        std::shared_ptr<SegmentNode> lch;
        std::shared_ptr<SegmentNode> rch;
        std::shared_ptr<Lazytag> tag;
        std::shared_ptr<Mergeable> info;
    };
    std::vector<std::shared_ptr<SegmentNode>> roots;
    int val_l;
    int val_r;
};

#endif