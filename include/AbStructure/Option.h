#ifndef OPTION_H
#define OPTION_H

#include <vector>
#include <memory>

#include "Random.h"
#include "Integer.h"

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
            total += robin[i]->get();
        if (total == 0) MESSAGE("Option", "No Option");
        int rank = Random::rand_int(1, total);
        for (int i = 0; i < robin.size(); i++) {
            if (rank - robin[i]->get() > 0)
                rank -= robin[i]->get();
            else {
                result = opts[i];
                return std::dynamic_pointer_cast<T>(result);
            }
        }
        MESSAGE("Option", UNEXCEPT);
    }

    virtual void generate(bool re, std::shared_ptr<Node> from) override;
    virtual std::shared_ptr<Node> clone() override;

    virtual void parse(const std::string& spec, int n, ...) override;
    virtual void out() override;
private:
    std::shared_ptr<Node> result;
    std::vector<std::shared_ptr<Node>> opts;
    std::vector<std::shared_ptr<Integer>> robin;
};

namespace mk {
    std::shared_ptr<Option> option();

    template<typename O, typename T, typename ...Args>
    std::shared_ptr<Option> option(O&& opt, T&& i, Args... args) {
        return option(args...)->add_option(opt, i);
    }
}

#endif