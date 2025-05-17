#include <type_traits>
#include <iostream>

#include "target.hpp"

template <typename type, type value, typename Tag>
class AccessBypass {
    friend type get(Tag) { return value;}
};

struct BackStagePass {};
int32_t Target::* get(BackStagePass);

template class AccessBypass<
    int32_t Target::*,
    & Target::x_,
    BackStagePass>;

int main() {
    Target t;
    // Now we can use t.* backstage_pass to access the private data member
    // pure magic!!!!
    t.* get(BackStagePass()) = 42;
    std::cout << t.getX() << std::endl;
    return 0;
}