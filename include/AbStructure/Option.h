#ifndef OPTION_H
#define OPTION_H

#include <vector>
#include <memory>

#include "Random.h"
#include "Integer.h"

namespace mk {

class Option :
    public Node,
    public Formatable {
public:
    Option();
    Option(const Option& other);
    std::shared_ptr<Option> add_option(std::shared_ptr<Node> opt, int x);
    std::shared_ptr<Option> add_option(std::shared_ptr<Node> opt, std::shared_ptr<Integer> x);
    
    template<typename T>
    std::shared_ptr<T> get() {
        CALL(FUNCTION);
        if (result) return std::dynamic_pointer_cast<T>(result);
        int total = 0;
        for (int i = 0; i < robin.size(); i++)
            total += robin[i]->value();
        if (total == 0) MESSAGE("Option", "No Option");
        int rank = Random::rand_int(1, total);
        for (int i = 0; i < robin.size(); i++) {
            if (rank - robin[i]->value() > 0)
                rank -= robin[i]->value();
            else {
                result = opts[i];
                return std::dynamic_pointer_cast<T>(result);
            }
        }
        MESSAGE("Option", UNEXCEPT);
    }

    virtual void generate(bool re) override;
    virtual std::shared_ptr<Node> clone(bool first) override;

    virtual void parse(const std::string& spec, int n, ...) override;
    virtual void out() override;
private:
    std::shared_ptr<Node> result;
    std::vector<std::shared_ptr<Node>> opts;
    std::vector<std::shared_ptr<Integer>> robin;
};

std::shared_ptr<Option> option();

template<typename O, typename T, typename ...Args>
std::shared_ptr<Option> option(O&& opt, T&& i, Args... args) {
    return option(args...)->add_option(opt, i);
}

}

#endif