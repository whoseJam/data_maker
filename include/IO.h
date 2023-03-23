#ifndef IO_H
#define IO_H

#include <functional>
#include <string>
#include <vector>
using Genfun = std::function<void()>;

class IO {
public:
    IO* testcase(int l, int r, Genfun gen);
    IO* input_prefix(const std::string& str);
    IO* input_suffix(const std::string& str);
    IO* output_prefix(const std::string& str);
    IO* output_suffix(const std::string& str);
    IO* working_directory(const std::string& str);
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
};

#endif