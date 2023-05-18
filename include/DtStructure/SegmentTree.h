// #ifndef SEGMENTTREE_H
// #define SEGMENTTREE_H

// #include <memory>
// #include <vector>
// #include <functional>

// #include "Logger.h"
// #include "Dtstructure.h"

// namespace mk {

// extern int SEGMENT_L;
// extern int SEGMENT_R;

// template<typename T, typename L = bool>
// class SegmentTree {
// public:
//     SegmentTree(int l, int r) : val_l(l), val_r(r) {
//         roots.push_back(std::make_shared<SegmentNode>());
//         cur_version = 0;
//     }
//     int insert(int pos, T info);
//     int insert(int pos, T info, int ver);
//     int insert(int pos, L tag);
//     int insert(int pos, L tag, int ver);
//     int insert(int l, int r, L tag);
//     int insert(int l, int r, L tag, int ver);
//     std::shared_ptr<T> query_p(int pos);
//     std::shared_ptr<T> query_p(int pos, int ver);
//     std::shared_ptr<T> query(int l, int r);
//     std::shared_ptr<T> query(int l, int r, int ver);
// private:
//     struct SegmentNode {
//         SegmentNode() = default;
//         SegmentNode(std::shared_ptr<T> info, std::shared_ptr<T> sum, std::shared_ptr<L> tag, std::shared_ptr<SegmentNode> lch, std::shared_ptr<SegmentNode> rch) :
//         info(std::move(info)), sum(std::move(sum)), tag(std::move(tag)), lch(std::move(lch)), rch(std::move(rch)) {}
//         std::shared_ptr<SegmentNode> clone() {return std::make_shared<SegmentNode>(info, sum, tag, lch, rch);}
//         std::shared_ptr<T> info;
//         std::shared_ptr<T> sum;
//         std::shared_ptr<L> tag;
//         std::shared_ptr<SegmentNode> lch;
//         std::shared_ptr<SegmentNode> rch;
//     };
//     void insert(std::shared_ptr<SegmentNode> cur, std::shared_ptr<T> info, int l, int r, int ql, int qr);
//     void insert(std::shared_ptr<SegmentNode> cur, std::shared_ptr<L> tag, int l, int r, int ql, int qr);
//     void query(std::shared_ptr<SegmentNode> cur, std::shared_ptr<T> ans, int l, int r, int ql, int qr);

//     std::vector<std::shared_ptr<SegmentNode>> roots;
//     int cur_version;
//     int val_l;
//     int val_r;

//     template<typename V, bool CHECKER = std::is_base_of_v<Mergeable, V>>
//     struct MergeableHelper;
//     template<typename V>
//     struct MergeableHelper<V, true> {
//         void push_up(std::shared_ptr<SegmentNode> cur, int l, int r) {
//             SEGMENT_L = l;
//             SEGMENT_R = r;
//             if (l == r) {
//                 cur->sum = cur->info;
//             } else {
//                 cur->sum = std::make_shared<T>(Zero());
//                 if (cur->lch && cur->lch->sum) cur->sum->merge(cur->lch->sum);
//                 if (cur->rch && cur->rch->sum) cur->sum->merge(cur->rch->sum);
//             }
//         }
//     };
//     template<typename V>
//     struct MergeableHelper<V, false> {
//         void push_up(std::shared_ptr<SegmentNode> cur, int l, int r) {}
//     };

//     template<typename V, bool CHECKER = std::is_base_of_v<Lazytag, V>>
//     struct LazytagHelper;
//     template<typename V>
//     struct LazytagHelper<V, true> {
//         void push_down(std::shared_ptr<SegmentNode> node, int l, int r) {
//             if (!node->tag) return;
//             if (node->lch) node->lch = node->lch->clone(); else node->lch = std::make_shared<SegmentNode>();
//             if (node->rch) node->rch = node->rch->clone(); else node->rch = std::make_shared<SegmentNode>();
//             int mid = (l + r) / 2;
//             push_tag(node->tag, node->lch, l, mid);
//             push_tag(node->tag, node->rch, mid+1, r);
//             node->tag = nullptr;
//         }
//         void push_tag(std::shared_ptr<L> tag, std::shared_ptr<SegmentNode> node, int l, int r) {
//             if (!node->tag) node->tag = std::make_shared<L>(Zero());
//             if (!node->sum) node->sum = std::make_shared<T>(Zero());
//             if (!node->info && l == r) node->info = std::make_shared<T>(Zero());
//             SEGMENT_L = l;
//             SEGMENT_R = r;
//             tag->push(node->tag);
//             tag->push(node->sum);
//             if (l == r)tag->push(node->info);
//         }
//     };
//     template<typename V>
//     struct LazytagHelper<V, false> {
//         void push_down(std::shared_ptr<SegmentNode> node, int l, int r) {}
//         void push_tag(std::shared_ptr<L> tag, std::shared_ptr<SegmentNode> node, int l, int r) {}
//     };
// };

// template<typename T, typename L>
// void SegmentTree<T, L>::insert(std::shared_ptr<SegmentNode> cur, std::shared_ptr<T> info, int l, int r, int ql, int qr) {
//     if (ql <= l && r <= qr) {
//         cur->info = info;
//         MergeableHelper<T>().push_up(cur, l, r);
//         return;
//     }
//     bool pushed = (cur->tag != nullptr);
//     LazytagHelper<L>().push_down(cur, l, r);
//     int mid = (l + r) / 2;
//     if (ql <= mid) {
//         if (!pushed && !cur->lch) cur->lch = std::make_shared<SegmentNode>();
//         else if (!pushed) cur->lch = cur->lch->clone();
//         insert(cur->lch, info, l, mid, ql, qr);
//     }
//     if (qr > mid) {
//         if (!pushed && !cur->rch) cur->rch = std::make_shared<SegmentNode>();
//         else if (!pushed) cur->rch = cur->rch->clone();
//         insert(cur->rch, info, mid+1, r, ql, qr);
//     }
//     MergeableHelper<T>().push_up(cur, l, r);
// }

// template<typename T, typename L>
// void SegmentTree<T, L>::insert(std::shared_ptr<SegmentNode> cur, std::shared_ptr<L> tag, int l, int r, int ql, int qr) {
//     if (ql <= l && r <= qr) {
//         LazytagHelper<L>().push_tag(tag, cur, l, r);
//         return;
//     }
//     bool pushed = (cur->tag != nullptr);
//     LazytagHelper<L>().push_down(cur, l, r);
//     int mid = (l + r) / 2;
//     if (ql <= mid) {
//         if (!pushed && !cur->lch) cur->lch = std::make_shared<SegmentNode>();
//         else if (!pushed) cur->lch = cur->lch->clone();
//         insert(cur->lch, tag, l, mid, ql, qr);
//     }
//     if (qr > mid) {
//         if (!pushed && !cur->rch) cur->rch = std::make_shared<SegmentNode>();
//         else if (!pushed) cur->rch = cur->rch->clone();
//         insert(cur->rch, tag, mid+1, r, ql, qr);
//     }
//     MergeableHelper<T>().push_up(cur, l, r);
// }

// template<typename T, typename L>
// void SegmentTree<T, L>::query(std::shared_ptr<SegmentNode> cur, std::shared_ptr<T> ans, int l, int r, int ql, int qr) {
//     if (!cur) return;
//     if (ql <= l && r <= qr) { if (cur->sum) ans->merge(cur->sum); return;}
//     bool pushed = (cur->tag != nullptr);
//     LazytagHelper<L>().push_down(cur, l, r);
//     int mid = (l + r) / 2;
//     if (ql <= mid) {
//         if (!pushed && !cur->lch) cur->lch = std::make_shared<SegmentNode>();
//         else if (!pushed) cur->lch = cur->lch->clone();
//         query(cur->lch, ans, l, mid, ql, qr); 
//     }
//     if (qr > mid) {
//         if (!pushed && !cur->rch) cur->rch = std::make_shared<SegmentNode>();
//         else if (!pushed) cur->rch = cur->rch->clone();
//         query(cur->rch, ans, mid+1, r, ql, qr);
//     }
//     MergeableHelper<T>().push_up(cur, l, r);
// }

// template<typename T, typename L>
// int SegmentTree<T, L>::insert(int pos, T info) {
//     return insert(pos, std::move(info), cur_version);
// }

// template<typename T, typename L>
// int SegmentTree<T, L>::insert(int pos, T info, int ver) {
//     if (ver < 0 || ver >= roots.size()) MESSAGE("SegmentTree", INDEX_OUT_OF_BOUNDARY);
//     auto cur = roots[ver]->clone();
//     roots.push_back(cur); cur_version++;
//     insert(cur, std::make_shared<T>(std::move(info)), val_l, val_r, pos, pos);
//     return cur_version;
// }

// template<typename T, typename L>
// int SegmentTree<T, L>::insert(int pos, L tag) {
//     return insert(pos, std::move(tag), cur_version);
// }

// template<typename T, typename L>
// int SegmentTree<T, L>::insert(int pos, L tag, int ver) {
//     if (ver < 0 || ver >= roots.size()) MESSAGE("SegmentTree", INDEX_OUT_OF_BOUNDARY);
//     auto cur = roots[ver]->clone();
//     roots.push_back(cur); cur_version++;
//     insert(cur, std::make_shared<L>(std::move(tag)), val_l, val_r, pos, pos);
//     return cur_version;
// }

// template<typename T, typename L>
// int SegmentTree<T, L>::insert(int l, int r, L tag) {
//     return insert(l, r, std::move(tag), cur_version);
// }

// template<typename T, typename L>
// int SegmentTree<T, L>::insert(int l, int r, L tag, int ver) {
//     if (ver < 0 || ver >= roots.size()) MESSAGE("SegmentTree", INDEX_OUT_OF_BOUNDARY);
//     auto cur = roots[ver]->clone();
//     roots.push_back(cur); cur_version++;
//     insert(cur, std::make_shared<L>(std::move(tag)), val_l, val_r, l, r);
//     return cur_version;
// }

// template<typename T, typename L>
// std::shared_ptr<T> SegmentTree<T, L>::query(int l, int r) {
//     return query(l, r, cur_version);
// }

// template<typename T, typename L>
// std::shared_ptr<T> SegmentTree<T, L>::query(int l, int r, int ver) {
//     if (ver < 0 || ver >= roots.size()) MESSAGE("SegmentTree", INDEX_OUT_OF_BOUNDARY);
//     std::shared_ptr<T> ans = std::make_shared<T>(Zero());
//     query(roots[ver], ans, val_l, val_r, l, r);
//     return ans;
// }

// }

// #endif