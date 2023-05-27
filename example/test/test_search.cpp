#include <gtest.h>

#include "Search.h"

using namespace mk;
using namespace std;

int n;

struct PermutationStatus {
    int len;
    int ans[10];
    PermutationStatus() { len = 0; }
    PermutationStatus(const PermutationStatus& other) {
        len = other.len;
        memcpy(ans, other.ans, sizeof(ans));
    }
    auto successors() -> vector<PermutationStatus*> {
        vector<PermutationStatus*> ret;
        bool vis[10] = {0};
        for (int i = 1; i <= len; i++)
            vis[ans[i]] = true;
        for (int i = 1; i <= n; i++) {
            if (!vis[i]) {
                PermutationStatus* nxt = new PermutationStatus(*this);
                nxt->ans[len + 1] = i;
                nxt->len = len + 1;
                ret.push_back(nxt);
            }
        }
        return ret;
    }
    uint hash_code() {
        uint ret = len;
        for (int i = 1; i <= len; i++) ret = ret * 10 + ans[i];
        return ret;
    }
    bool equal(const PermutationStatus& other) {
        if (len != other.len) return false;
        for (int i = 1; i <= len; i++)
            if (ans[i] != other.ans[i]) return false;
        return true;
    }
};

struct PermutationProblem : public SearchProblem {
    static PermutationStatus* start_status() { return new PermutationStatus(); }
    static void operate_on(PermutationStatus* stat) {
        for (int i = 1; i <= stat->len; i++)
            cout << stat->ans[i] << ' ';
        cout << "\n";
    }
    static bool is_end(PermutationStatus* stat) { return stat->len == n; }
    static bool finish() { return false; }
};

TEST(SearchTest, PermutationTest) {
    n = 3;
    SearchAgent<PermutationStatus, PermutationProblem> agent1(SearchType::DFS);
    agent1.solve();
    SearchAgent<PermutationStatus, PermutationProblem> agent2(SearchType::BFS);
    agent2.solve();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}