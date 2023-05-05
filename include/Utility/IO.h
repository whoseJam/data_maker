#ifndef IO_H
#define IO_H

#include <memory>
#include <string>
#include <vector>
#include <functional>

using Genfun = std::function<void()>;

class IO :
    public std::enable_shared_from_this<IO> {
public:
    IO();
    std::shared_ptr<IO> testcase(int l, int r, Genfun gen);
    std::shared_ptr<IO> testcase(int x, Genfun gen);
    std::shared_ptr<IO> input_prefix(const std::string& str);
    std::shared_ptr<IO> input_suffix(const std::string& str);
    std::shared_ptr<IO> output_prefix(const std::string& str);
    std::shared_ptr<IO> output_suffix(const std::string& str);
    std::shared_ptr<IO> working_directory(const std::string& str);
    std::shared_ptr<IO> skip_generate_existing_data();
    void generate();
private:
    struct IOPack {
        IOPack(int l, int r, Genfun gen);
        int l;
        int r;
        Genfun gen;
    };
    std::vector<IOPack> packs;
    std::string input_pre;
    std::string input_suf;
    std::string output_pre;
    std::string output_suf;
    
    std::string working_dir;
    bool skip_exist_data;
};

namespace mk {
    std::shared_ptr<IO> standard_io();
}

#endif