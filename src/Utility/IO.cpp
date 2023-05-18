
#include <fstream>
#include <iostream>
#include <algorithm>

#include "IO.h"
#include "Logger.h"
#include "Define.h"

using namespace std;

namespace mk {

bool check_file_exist(const string& name) {
    ifstream f(name.c_str());
    return f.good();
}

void close_freopen() {
#ifdef _WIN32
    freopen("CON", "w", stdout);
#elif __linux__
    freopen("/dev/tty", "w", stdout);
#endif
}

void ensure_folder_exist(const string& name) {
    char buffer[1<<10];
    sprintf(buffer, "mkdir -p %s", name.c_str());
    system(buffer);
}

IO::IOPack::IOPack(int l, int r, Genfun gen) {
    this->l = l;
    this->r = r;
    this->gen = gen;
}

IO::IO() {
    working_dir = "UNSET";
    output_pre = "UNSET";
    output_suf = "UNSET";
    input_pre = "UNSET";
    skip_exist_data = false;
}

shared_ptr<IO> IO::testcase(int l, int r, Genfun gen) {
    packs.push_back(IOPack(l, r, gen));
    return shared_from_this();
}

shared_ptr<IO> IO::testcase(int x, Genfun gen) {
    return testcase(x, x, gen);
}

shared_ptr<IO> IO::input_prefix(const string& str) {
    input_pre = str;
    return shared_from_this();
}

shared_ptr<IO> IO::input_suffix(const string& str) {
    input_suf = str;
    return shared_from_this();
}

shared_ptr<IO> IO::output_prefix(const string& str) {
    output_pre = str;
    return shared_from_this();
}

shared_ptr<IO> IO::output_suffix(const string& str) {
    output_suf = str;
    return shared_from_this();
}

shared_ptr<IO> IO::working_directory(const string& str) {
    working_dir = str;
    return shared_from_this();
}

shared_ptr<IO> IO::skip_generate_existing_data() {
    skip_exist_data = true;
    return shared_from_this();
}

void IO::generate() {
    if (packs.size() == 0) MESSAGE("IO", NEED("testcase"));
    sort(packs.begin(), packs.end(), [=](const IOPack& a, const IOPack& b){
        if (a.l != b.l) return a.l < b.l;
        return a.r < b.r;
    });
    for (int i = 0; i < packs.size() - 1; i++) {
        IOPack& now = packs[i];
        IOPack& nxt = packs[i+1];
        if (now.r + 1 != nxt.l) MESSAGE("IO", ENSURE("index of testcases is continuous"));
    }
    if (packs[0].l != 1) MESSAGE("IO", ENSURE("the first group of testcases start from 1"));
    if (input_pre == "UNSET") MESSAGE("IO", NEED("input_prefix"));
    if (input_suf == "UNSET") MESSAGE("IO", NEED("input_suffix"));
    if (output_pre == "UNSET") MESSAGE("IO", NEED("output_prefix"));
    if (output_suf == "UNSET") MESSAGE("IO", NEED("output_suffix"));
    if (working_dir == "UNSET") MESSAGE("IO", NEED("working_directory"));
    ensure_folder_exist(working_dir);
    
    for (int i = 0; i < packs.size(); i++) {
        IOPack& now = packs[i];
        for (int idx = now.l; idx <= now.r; idx++) {
            close_freopen();
            cout << "making test data " << idx << "\n";
            string in_path = working_dir + "/" + input_pre + to_string(idx) + "." + input_suf;
            if (skip_exist_data && check_file_exist(in_path)) {
                cout << "test data already exist, skip\n";
                continue;
            }
            freopen(in_path.c_str(), "w", stdout);
            string out_path = working_dir + "/" + output_pre + to_string(idx) + "." + output_suf;
            now.gen();
        }
    }

    string problem_conf_path = working_dir + "/problem.conf";
    freopen(problem_conf_path.c_str(), "w", stdout);
    int n_tests = packs[packs.size() - 1].r;
    cout << "n_tests " << n_tests << "\n";
    cout << "n_ex_tests 0\n";
    cout << "n_sample_tests 0\n";
    cout << "time_limit 1\n";
    cout << "memory_limit 256\n";
    cout << "output_limit 64\n";
    cout << "use_builtin_judger on\n";
    cout << "use_builtin_checker wcmp\n";
    cout << "input_pre " << input_pre << "\n";
    cout << "input_suf " << input_suf << "\n";
    cout << "output_pre " << output_pre << "\n";
    cout << "output_suf " << output_suf << "\n";
    close_freopen();

    string std_path = working_dir + "/std.cpp";
    string std_exec_path = working_dir + "/std";
    if (check_file_exist(std_path)) {
        char cmd[1<<10];
        for (int i = 1; i <= n_tests; i++) {
            close_freopen();
            cout << "running test data " << i << "\n";

            sprintf(cmd, "g++ %s -o %s", std_path.c_str(), std_exec_path.c_str());
            system(cmd);
            string in_path = working_dir + "/" + input_pre + to_string(i) + "." + input_suf;
            string out_path = working_dir + "/" + output_pre + to_string(i) + "." + output_suf;
            sprintf(cmd, "cat %s | %s > %s", 
                in_path.c_str(),
                std_exec_path.c_str(),
                out_path.c_str());
            system(cmd);
        }
        sprintf(cmd, "rm %s", std_exec_path.c_str());
        system(cmd);
    } else {
        cout << "Warning : std no found\n";
    }
}

shared_ptr<IO> standard_io() {
    return make_shared<IO>()
    ->input_prefix("input")
    ->input_suffix("txt")
    ->output_prefix("output")
    ->output_suffix("txt");
}

}
