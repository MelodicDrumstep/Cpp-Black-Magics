#include <type_traits>
#include <iostream>

#include "target.hpp"

template <typename Tag, typename Tag::type MemberPtr>
struct AccessCast {
    // C++ allows us to define this friend function inside the class
    // and since the scope of the template parameters extends to the
    // entire template class, 
    // "get" can access the template parameter "MemberPtr"
    friend typename Tag::type get(Tag) { return MemberPtr; }
};


template <typename Tag, typename MemberPtr>
struct AccessTag {
    using type = MemberPtr;
    friend type get(Tag);
    // forward declare the friend function get(Tag)
    // otherwise we will get a compile error : "get was not declared in this scope"
};

// CRTP for getting the tag type inside AccessTag (It cannot get the type of itself!)
struct TargetMemTag : AccessTag<TargetMemTag, int32_t Target::*> {};

// Instantiate the template class with the address of the private data member.
template struct AccessCast<TargetMemTag, &Target::x_>;

int main() {
    Target t;
    // get(TargetMemTag()) returns "&Target::x_"
    t.* get(TargetMemTag()) = 42;
    std::cout << t.getX() << std::endl;
    return 0;
}