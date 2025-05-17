#include <type_traits>
#include <iostream>

#include "target.hpp"

/*
 *This is a template class that allows access to a private member of a class.
 * It is used to bypass the private access modifier and set the value of the private member.
 */
template <typename type, type value, type & receiver>
class AccessBypass {
    /**
     * A static data member which will be initialized when the class
     * is instantiated.
     * And it will assign "value" to "receiver".
     */
    static struct Mover {
        Mover() {
            receiver = value;
        }
    } m;
};

/**
 * Define the static data member. This is required by C++.
 */
template <typename type, type value, type & receiver>
typename AccessBypass<type, value, receiver>::Mover
    AccessBypass<type, value, receiver>::m;

// We declare a pointer to the int32_t data member of Target class
// Actually it won't lead to runtime overhead.
int32_t Target::* backstage_pass;

/**
 * Instantiate the specified class through template parameters.
 */
template class AccessBypass<int Target::*, 
    & Target::x_,
    backstage_pass>;

int main() {
    Target t;
    // Now we can use t.* backstage_pass to access the private data member
    // pure magic!!!!
    t.* backstage_pass = 42;
    std::cout << t.getX() << std::endl;
    return 0;
}