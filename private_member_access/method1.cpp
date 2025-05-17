#include <type_traits>
#include <iostream>

#include "target.hpp"

// & Target::x_ can be used a template parameter
// Then we return it through a friend function
template <typename type, type value, typename Tag>
class AccessBypass {
    // C++ allows us to define this friend function inside the class
    // and since the scope of the template parameters extends to the
    // entire template class, 
    // "get" can access the template parameter "value"
    friend type get(Tag) { return value;}
};

// This tag just serves as a placeholder for the function parameter
struct BackStagePass {};
int32_t Target::* get(BackStagePass);

// Instantiate the template class with the address of the private data member.
// It will define the friend function get(BackStagePass)
template class AccessBypass<
    int32_t Target::*,
    & Target::x_,
    BackStagePass>;

int main() {
    Target t;
    // We use get(BackStagePass()) to return "Target::x_"
    t.* get(BackStagePass()) = 42;
    std::cout << t.getX() << std::endl;
    return 0;
}