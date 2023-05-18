#include "String.h"
#include "Character.h"

using namespace std;
using namespace mk;

int main() {
    auto ch = character('a', 'z');
    auto str = mk::string(integer(1, 10), character('a', 'z'));
    BUILD(str);
    return 0;
}