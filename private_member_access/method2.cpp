#include <type_traits>
#include <iostream>

#include "target.hpp"

template <typename Tag, typename Tag::type MemberPtr>
struct AccessCast {
    friend typename Tag::type get(Tag) { return MemberPtr; }
};

template <typename Tag, typename MemberPtr>
struct AccessTag {
    using type = MemberPtr;
    friend type get(Tag);
};

struct TargetMemTag : AccessTag<TargetMemTag, int32_t Target::*> {};

template struct AccessCast<TargetMemTag, &Target::x_>;

int main() {
    Target t;
    // Now we can use t.* backstage_pass to access the private data member
    // pure magic!!!!
    t.* get(TargetMemTag()) = 42;
    std::cout << t.getX() << std::endl;
    return 0;
}