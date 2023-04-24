
#include <fstream>
#include <iostream>
#include <algorithm>

#include "../include/IO.h"
#include "../include/Define.h"

using namespace std;

bool check_file_exist(const string& name) {
    ifstream f(name.c_str());
    return f.good();
}

void close_freopen() {
    freopen("/dev/tty","w",stdout);
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

IO* IO::testcase(int l, int r, Genfun gen) {
    packs.push_back(IOPack(l, r, gen));
    return this;
}

IO* IO::testcase(int x, Genfun gen) {
    return testcase(x, x, gen);
}

IO* IO::input_prefix(const string& str) {
    input_pre = str;
    return this;
}

IO* IO::input_suffix(const string& str) {
    input_suf = str;
    return this;
}

IO* IO::output_prefix(const string& str) {
    output_pre = str;
    return this;
}

IO* IO::output_suffix(const string& str) {
    output_suf = str;
    return this;
}

IO* IO::working_directory(const string& str) {
    working_dir = str;
    return this;
}

IO* IO::skip_generate_existing_data() {
    skip_exist_data = true;
    return this;
}

void IO::generate() {
    if (packs.size() == 0) {
        cout << "IO::testcase is empty\n";
        exit(-1);
    }
    sort(packs.begin(), packs.end(), [=](const IOPack& a, const IOPack& b){
        if (a.l != b.l) return a.l < b.l;
        return a.r < b.r;
    });
    for (int i = 0; i < packs.size() - 1; i++) {
        IOPack& now = packs[i];
        IOPack& nxt = packs[i+1];
        if (now.r + 1 != nxt.l) {
            cout << "IO::testcases not tight\n";
            exit(-1);
        }
    }
    if (packs[0].l != 1) {
        cout << "IO::the first group of testcases must start from 1\n";
        exit(-1);
    }
    CHECK_STRING_UNSET(IO, input_pre);
    CHECK_STRING_UNSET(IO, input_suf);
    CHECK_STRING_UNSET(IO, output_pre);
    CHECK_STRING_UNSET(IO, output_suf);
    CHECK_STRING_UNSET(IO, working_dir);
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