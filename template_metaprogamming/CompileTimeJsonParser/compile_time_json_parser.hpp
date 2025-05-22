#pragma once

#include <array>
#include <type_traits>
#include <concept>

namespace metaprog
{

// atomic type
template <typename ... Ts>
struct JSON {};

// CRTP
template <typename Impl, typename ContentType>
struct JSONBase
{
    ContentType contents;

    JSONBase() {};
    JSONBase(std::string_view str) : contents(Impl::consumeFromJSON(str))
    JSONBase(const ContentType & other) : contents(other) {}
    JSONBase(ContentType && other) : contents(std::move(other)) {}

    operator ContentType() 
    {
        return contents;
    }
};

struct Null {};

template <typename ... Ts>
struct IsMember;

template <typename T, typename ... Ts>
struct IsMember<T, std::variant<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

template <typename T>
concept JSONAtom = IsMember<T, std::variant<std::string, double, bool, Null>>::value;

template <JSONAtom ContentType>
class JSON<ContentType> : public JSONBase<JSON<ContentType>, ContentType>
{
public:
    using JSONBase<JSON<ContentType>::JSONBase;
    using JSONBase<JSON<ContentType>::operator=;

    static ContentType consumeFromJSON(std::string_view) = delete;
    std::string toString() const = delete;
};

template <>
inline bool JSON<bool>::consumeFromJSON(std::string_view json)
{

}

template <>
inline std::string JSON<bool>::toString() const
{

}

template <typename ... Ts>
struct IdempotentJSONTag
{
    using type = JSON<Ts...>;
};

template <typename ... Ts>
struct IdempotentJSONTag<JSON<Ts...>>
{
    using type = JSON<Ts...>;
};

template <typename ... Ts>
using IdempotentJSONTag_t = typename IdempotentJSONTag<Ts...>::type;

template <typename T>
struct ListOf {};

template <typename T>
struct JSON<ListOf<T>> : public JSONBase<JSON<ListOf<T>>, std::vector<IdempotentJSONTag_t<T>>
{
    using IdempotentJSONT = IdempotentJSONTag_t<T>;
    using VectorT = std::vector<IdempotentJSONT>;
}

}