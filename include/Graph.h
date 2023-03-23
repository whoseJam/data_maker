#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include "IExportable.h"

class Graph : public Node {
public:
    int vertex_num;
    int edge_num;
    bool is_connected;

    Graph();
    Graph* vertex(int num);
    Graph* edge(int num);
    Graph* connected(bool flag);
    void generate() override;
    void out() override;
};

#endif