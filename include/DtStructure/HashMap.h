// #ifndef HASHMAP_H
// #define HASHMAP_H

// #include <map>
// #include <memory>

// #include "Dtstructure.h"

// namespace mk {

// class HashMap {
// public:
//     HashMap();
//     void insert(std::shared_ptr<Hashable> obj);
//     void remove(std::shared_ptr<Hashable> obj);
//     bool in_hashmap(std::shared_ptr<Hashable> obj);
//     int query_total();
//     int query_different();
// private:
//     struct HashNode {
//         int cnt = 0;
//         std::shared_ptr<Hashable> hold;
//         std::shared_ptr<HashNode> next;
//     };
//     std::map<uint, std::shared_ptr<HashNode>> header;
//     int diff;
//     int total;
// };

// auto hash_map() -> std::shared_ptr<HashMap> ;

// }

// #endif