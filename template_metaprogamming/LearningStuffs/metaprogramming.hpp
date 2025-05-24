#pragma once

#include <type_traits>
#include <concepts>
#include <string_view>
#include <algorithm>

namespace metaprog
{

/* Identity */
// Consider this to be a meta function that returns a type
template <typename T>
struct TypeIdentity
{
    using type = T;
};

// Consider this to be a meta function that returns a value
// auto template parameters! sounds like magic
template <auto X>
struct ValueIdentity
{
    static constexpr auto value = X;
};

template <typename T>
using TypeIdentity_t = typename TypeIdentity<T>::type;

template <auto X>
inline constexpr auto ValueIdentity_v = ValueIdentity<X>::value;

/* AddLvalueReference */
// Add lvalue reference excluding "T == void"
// Return "void" if "T == void"
template <typename T, typename = void>
struct AddLvalueReference : TypeIdentity<T> {};

// "T == void" cannot match this case
// because we don't allow std::void_t<void &>
template <typename T>
struct AddLvalueReference<T, std::void_t<T &>> : TypeIdentity<T &> {};

static_assert(std::is_same<int &, typename AddLvalueReference<int>::type>{});
static_assert(std::is_same<void, typename AddLvalueReference<void>::type>{});

/* IntegralConstant */
template <auto v>
struct IntegralConstant
{
    using value_type = decltype(v);
    static constexpr value_type value = v;
    using type = IntegralConstant<v>;

    // static cast
    constexpr operator value_type() const noexcept
    {
        return value;
    }

    // call operator
    constexpr value_type operator()() const noexcept
    {
        return value;
    }
};

/* BoolConstant */
template <bool B>
using BoolConstant = IntegralConstant<B>;

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

/* IsVoid */
template <typename T>
struct IsVoid : FalseType {};

template <>
struct IsVoid<void> : TrueType {};

template <>
struct IsVoid<void const> : TrueType {};

template <>
struct IsVoid<void volatile> : TrueType {};

template <>
struct IsVoid<void const volatile> : TrueType {};

/* RemoveConst */
template <typename T>
struct RemoveConst : TypeIdentity<T> {};

// paritial specialization
template <typename T>
struct RemoveConst<T const> : TypeIdentity<T> {};

template <typename T>
using RemoveConst_t = typename RemoveConst<T>::type;

/* Remove Volatile */
template <typename T>
struct RemoveVolatile : TypeIdentity<T> {};

template <typename T>
struct RemoveVolatile<T volatile> : TypeIdentity<T> {};

template <typename T>
using RemoveVolatile_t = typename RemoveVolatile<T>::type;

/* Remove CV */
template <typename T>
using RemoveCV = RemoveConst<RemoveVolatile_t<T>>;

template <typename T>
using RemoveCV_t = typename RemoveCV<T>::type;

static_assert(std::is_same<int, RemoveCV_t<int const volatile>>{});

/* Condition */
template <bool Condition, typename T, typename F>
struct Conditional : TypeIdentity<T> {};

template <typename T, typename F>
struct Conditional<false, T, F> : TypeIdentity<F> {};

template <bool Condition, typename T, typename F>
using Conditional_t = Conditional<Condition, T, F>::type;

/* is_same */
template <typename T1, typename T2>
struct IsSame : FalseType {};

template <typename T>
struct IsSame<T, T> : TrueType {};
 
template <typename T1, typename T2>
constexpr bool IsSame_v = IsSame<T1, T2>::value;

template <typename T1, typename T2>
using IsSameRaw = IsSame<RemoveCV_t<T1>, RemoveCV_t<T2>>;

template <typename T1, typename T2>
using IsSameRaw_v = IsSameRaw<T1, T2>::value;

/* IsFloatingPoint */
template <typename T>
using IsFloatingPoint = BoolConstant<
    std::disjunction_v<
        IsSameRaw<float, T>,
        IsSameRaw<double, T>,
        IsSameRaw<long double, T>
    >
>;

/* IsArray */
template <typename T>
struct IsArray : FalseType {};

template <typename T, std::size_t N>
struct IsArray<T[N]> : TrueType {};

template <typename T>
struct IsArray<T[]> : TrueType {};

static_assert(IsArray<int[5]>::value);

static_assert(IsArray<int[]>::value);

/* IsPointer */
template <typename T>
struct IsPointerImpl : FalseType {};

template <typename T>
struct IsPointerImpl<T *> : TrueType {};

template <typename T>
using IsPointer = IsPointerImpl<RemoveCV_t<T>>;

/* IsUnion */
template <typename T>
using IsUnion = BoolConstant<__is_union(T)>;

/* IsNullptr . Function Overload Resolution */
namespace detail {
    TrueType IsNullptr(std::nullptr_t);
    FalseType IsNullptr(...);
}

template <typename T>
using IsNullPointer = decltype(detail::IsNullptr(std::declval<T>()));

static_assert(!IsNullPointer<int>::value);
static_assert(IsNullPointer<std::nullptr_t>::value);

/* IsConst */
namespace detail 
{
    template <typename T>
    TrueType IsConst(TypeIdentity<T const>);
    template <typename T>
    FalseType IsConst(TypeIdentity<T>);
}

template <typename T>
using IsConst = decltype(detail::IsConst(std::declval<TypeIdentity<T>>()));

static_assert(!IsConst<std::nullptr_t>::value);
static_assert(IsConst<int const>::value);

/* CanHaveMemberPtr */
template <typename T>
TrueType CanHavePointerToMember(int T:: *);
template <typename T>
FalseType CanHavePointerToMember(...);

struct Foo {};
static_assert(decltype(CanHavePointerToMember<Foo>(nullptr))::value);

template <typename T>
using CanHaveMemberPtr = decltype(CanHavePointerToMember<T>(nullptr));

/* IsClass */
namespace detail 
{
template <typename T>
BoolConstant<!IsUnion<T>::value> IsClassOrUnion(int T:: *);
template <typename T>
FalseType IsClassOrUnion(...);

}

template <typename T>
using IsClass = decltype(detail::IsClassOrUnion<T>(nullptr));

/* IsClass (2nd version) */
namespace detail 
{
template <typename T>
constexpr bool IsClassOrUnion2nd(int T:: *) {
    return !IsUnion<T>::value;
} 
template <typename T>
constexpr bool IsClassOrUnion2nd(...) {
    return false;
}
}

template <typename T>
using IsClass2nd = BoolConstant<detail::IsClassOrUnion2nd<T>(nullptr)>;

/* Recursive Find IsInPack */

template <typename TargetT, typename ... Ts>
struct RecursiveIsInPack : FalseType {};

template <typename TargetT, typename ... RestTs>
struct RecursiveIsInPack<TargetT, TargetT, RestTs...> : TrueType {};

template <typename TargetT, typename FirstT, typename ... RestTs>
struct RecursiveIsInPack<TargetT, FirstT, RestTs...> : RecursiveIsInPack<TargetT, RestTs...> {};

/* Linear Search IsInPack */
template <typename T>
static constexpr std::string_view NameOf()
{
    return std::string_view(__PRETTY_FUNCTION__);
    // Different T yields different pretty function name 
    // provided by the compiler
}

template <typename ... Ts>
struct PackImpl
{
    using Fn = std::string_view(*)();
    static constexpr Fn ids[] = { NameOf<Ts>... };
    // ids is an array of function pointer
    // Notice that we don't call NameOf
    // But NameOf already stores the std::string_view
    
    static constexpr bool Contains(Fn target)
    {
        for (auto && x : ids)
        {
            if (x == target)
            {
                return true;
            }
        }
        return false;
    }
};

/* Inheritance : IsInPack */
namespace detail
{
template <typename T>
static constexpr TrueType Check(TypeIdentity<T> const *);
template <typename T>
static constexpr FalseType Check(void const *);
}

// We cannot inheritate directly from Ts ... because there may be raw types
// But we can inheritate from TypeIdentity<Ts>...
template <typename ... Ts>
struct IsInPackImpl : TypeIdentity<Ts>... {};

// To see if IsInPackImpl<Ts...> * can be converted to TypeIdentity<T> *
// However, this method does not support duplicate types in Ts... 
template <typename TargetT, typename ... Ts>
using Inheritance_IsInPack = decltype(detail::Check<TargetT>((IsInPackImpl<Ts...> *)nullptr));

// Sorting types
template <template <typename> class Orderable, typename ListT>
class SortTypes;

template <template <typename> class Orderable, template <typename ...> class List,
    typename... Ts>
class SortTypes<Orderable, List<Ts...>>
{
    template <typename SeqT> 
    struct SortTypesImpl;

    template <std::size_t... Is>
    struct SortTypesImpl<std::index_sequence<Is...>>
    {
        // NOTE: lambda expression and directly calling here
        // The grammar is mis-leading
        static constexpr std::array<std::size_t, sizeof...(Ts)> indexes = []
        {
            // capture the common type. E.g. if one value is double, another is int,
            // the common type would be double
            using Ord = std::common_type_t<decltype(Orderable<Ts>::value)...>;
            struct Pair
            {
                Ord ord;
                std::size_t index;
            };
            std::array<Pair, sizeof...(Ts)> array = { Pair{ Orderable<Ts>::value, Is }... };
            std::sort(array.begin(), array.end(), [](auto x, auto y)
            {
                return x.ord < y.ord;
            });
            // Purely wonderful! We can call std::sort at compile time in C++20
            return std::array<std::size_t, sizeof...(Ts)>{ array[Is].index... };
            // Thus we fill in the indices array, indicating the sorted order
        }();
        using type = List<std::tuple_element_t<indexes[Is], List<Ts...>>...>;
        // Fill the sorted type list using std::tuple_element_t
    };

public:
    using type = typename SortTypesImpl<std::make_index_sequence<sizeof...(Ts)>>::type;
};

// Sorting type use case
template <typename T>
using Orderable = IntegralConstant<alignof(T)>;

using Sorted = typename SortTypes<
    Orderable,
    std::tuple<int, double, unsigned int, char>>::type;

static_assert(std::is_same<
    std::tuple<char, int, unsigned int, double>,
    Sorted>{});

template <typename T>
struct RetrieveElementType;

template <typename T, std::size_t N>
struct RetrieveElementType<std::array<T, N>> {
    using type = T;
};

template <typename T>
struct RetrieveElementType<std::vector<T>> {
    using type = T;
};

template <typename T, std::size_t N>
struct RetrieveElementType<T[N]> {
    using type = T;
};

template <typename T>
using RetrieveElementType_t = typename RetrieveElementType<T>::type;

}