#include <utility>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>

template <typename T>
class Cont {
private:
    T * elems;
public:
    Cont(T * elems) : elems(elems) {}
    // Add this "template <typename D = T>" here for defering the evalution of the return type
    // Otherwise, if we write the Node class below, the compiler woule think that the return type is incomplete
    template <typename D = T>
    typename std::conditional_t<std::is_move_constructible_v<D>, D&&, D> foo() { return std::move(*elems); }
};

struct Node {
    Node(std::string_view value, Node * next) : value_(value), next_(next) {}
    std::string value_;
    Cont<Node> next_;
};

int main() {
    Node a("a", nullptr);
    Node b("b", &a);
    b.next_.foo();
    return 0;
}