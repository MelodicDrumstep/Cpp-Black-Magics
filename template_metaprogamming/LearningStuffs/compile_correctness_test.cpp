#include <iostream>
#include "metaprogramming.hpp"

int main() {
    // Test TypeIdentity
    // Verify that TypeIdentity correctly returns the type T
    static_assert(std::is_same_v<int, metaprog::TypeIdentity<int>::type>);
    static_assert(std::is_same_v<double, metaprog::TypeIdentity<double>::type>);

    // Test ValueIdentity
    // Verify that ValueIdentity correctly stores and returns the value X
    static_assert(metaprog::ValueIdentity<42>::value == 42);

    // Test TypeIdentity_t
    // Verify that TypeIdentity_t is an alias for TypeIdentity<T>::type
    static_assert(std::is_same_v<int, metaprog::TypeIdentity_t<int>>);
    static_assert(std::is_same_v<double, metaprog::TypeIdentity_t<double>>);

    // Test ValueIdentity_v
    // Verify that ValueIdentity_v is an alias for ValueIdentity<X>::value
    static_assert(metaprog::ValueIdentity_v<42> == 42);

    // Test AddLvalueReference
    // Verify that AddLvalueReference correctly adds an lvalue reference to T
    // and returns void if T is void
    static_assert(std::is_same_v<int&, metaprog::AddLvalueReference<int>::type>);
    static_assert(std::is_same_v<void, metaprog::AddLvalueReference<void>::type>);

    // Test IntegralConstant
    // Verify that IntegralConstant correctly stores and provides access to the value v
    metaprog::IntegralConstant<5> five;
    static_assert(five.value == 5);          // Check the static value
    static_assert(five() == 5);             // Check the call operator
    static_assert(static_cast<int>(five) == 5); // Check the type conversion operator

    static_assert(std::is_same<int, metaprog::RetrieveElementType_t<std::array<int, 5>>>{});
    static_assert(std::is_same<int, metaprog::RetrieveElementType_t<std::vector<int>>>{});
    static_assert(std::is_same<int, metaprog::RetrieveElementType_t<int[5]>>{});

    static_assert(std::is_same<int, metaprog::Decay_t<int>>{});
    static_assert(std::is_same<int *, metaprog::Decay_t<int[]>>{});
    static_assert(std::is_same<int *, metaprog::Decay_t<int[5]>>{});
    static_assert(std::is_same<int(*)(int), metaprog::Decay_t<int(int)>>{});

    // If all assertions pass, print a success message
    std::cout << "All tests passed!" << std::endl;
    return 0;
}