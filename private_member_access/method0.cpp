#include <type_traits>
#include <iostream>

#include "target.hpp"

/**
 * Our goal is to assign "backstage_pass = &Target::x_"
 * But we cannot do this directly.
 * We need to use template tricks to bypass the private access modifier.
 */

/*
 *This is a template class that allows access to a private member of a class.
 * It is used to bypass the private access modifier and set the value of the private member.
 */
template <auto value, auto & receiver>
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
template <auto value, auto & receiver>
typename AccessBypass<value, receiver>::Mover
    AccessBypass<value, receiver>::m;

// We declare a pointer to the int32_t data member of Target class.
// It will store the address of the private data member "x_" inside one object.
// Actually it won't lead to runtime overhead. 
// Everything is done at compile time.
int32_t Target::* backstage_pass;

/**
 * Instantiate the specified class through template parameters.
 * Inside template parameters, we can do assignment regardless of the access qualifications.
 */
template class AccessBypass<& Target::x_, backstage_pass>;

int main() {
    Target t;
    // Now we can use t.* backstage_pass to access the private data member
    // pure magic!!!!
    t.* backstage_pass = 42;
    std::cout << t.getX() << std::endl;
    return 0;
}