#ifndef EDGE_H
#define EDGE_H

#include <vector>

#include "Node.h"
#include "Format.h"
#include "Attribute.h"

class Edge : public Node, public Format::Formatter {
public:
    Edge();
    Edge(const Edge& other);
    Edge* add_attribute(Attribute* attr);
    Edge* format(const std::string& fmt);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
    virtual void parse(const std::string& specifier, ...) override;
    virtual void parse_start() override;
    virtual void parse_next() override;
    virtual bool parse_finish() override;
    
    void __set_start_and_end(int start, int end);
private:
//  define stage;
    std::vector<Attribute*> attrs;

//  generate stage
    int start;
    int end;

//  output stage
    int cur_iter;
};

#endif