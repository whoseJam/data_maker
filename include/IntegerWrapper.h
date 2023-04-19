
#include "Node.h"
#include "Integer.h"

class IntegerWrapper : public Node {
public:
    IntegerWrapper(const std::string& class_name, const std::string& var_name);
    IntegerWrapper(const IntegerWrapper& other);
    int get();
    void set(int val);
    void set(Integer* val);
    virtual void generate() override;
    virtual Node* clone() override;
    virtual void destroy() override;
    virtual void out() override;
private:
    std::string cls;
    std::string var;
    Integer* integer_value;
    int int_value;
};