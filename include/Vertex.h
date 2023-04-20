#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

#include "Node.h"
#include "Format.h"
#include "Attribute.h"

class Vertex : public Node, public Format::Formatter {
public:
    Vertex(int idx);
    Vertex(const Vertex& other);
    virtual ~Vertex();
    Vertex* add_attribute(Attribute* attr);
    Vertex* format(const std::string& fmt);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
    virtual void parse(const std::string& specifier, ...) override;
    virtual void parse_start() override;
    virtual void parse_next() override;
    virtual bool parse_finish() override;
private:
//  define stage
    int idx;
    std::vector<Attribute*> attrs;

//  output stage
    int cur_iter;
};

#endif