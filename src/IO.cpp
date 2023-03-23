
#include "../include/IO.h"
#include <algorithm>
#include <iostream>
using namespace std;

IO::IOPack::IOPack(int l, int r, Genfun gen) {
    this->l = l;
    this->r = r;
    this->gen = gen;
}

IO* IO::testcase(int l, int r, Genfun gen) {
    packs.push_back(IOPack(l, r, gen));
    return this;
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
    for (int i = 0; i < packs.size() - 1; i++) {
        IOPack& now = packs[i];
        for (int idx = now.l; idx <= now.r; idx++) {
            string in_path = working_dir + "/" + input_pre + to_string(idx) + "." + input_suf;
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
    cout << "use_builtin_checker on\n";
    cout << "input_pre " << input_pre << "\n";
    cout << "input_suf " << input_suf << "\n";
    cout << "output_pre " << output_pre << "\n";
    cout << "output_suf " << output_suf << "\n";
}