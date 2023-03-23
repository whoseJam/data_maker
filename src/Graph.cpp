#include "../include/Graph.h"
#include <iostream>
using namespace std;

Graph::Graph() {
    vertex_num = 0;
    edge_num = 0;
    is_connected = true;
}

Graph* Graph::vertex(int num) {
    vertex_num = num;
    return this;
}

Graph* Graph::edge(int num) {
    edge_num = num;
    return this;
}

Graph* Graph::connected(bool flag) {
    is_connected = flag;
    return this;
}

void Graph::generate() {

}

void Graph::out() {

}