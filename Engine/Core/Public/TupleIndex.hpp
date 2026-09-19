#if !defined( B33_TUPLE_INDEX_HPP )
#    define B33_TUPLE_INDEX_HPP

namespace B33::Core
{

template <typename... T>
using Tuple = ::std::tuple<T...>;

template <typename T, typename Tuple>
struct TupleIndex;

template <typename T, typename... Ts>
struct TupleIndex<T, Tuple<T, Ts...>>
{
    static constexpr size_t value = 0;
};

template <typename T, typename U, typename... Ts>
struct TupleIndex<T, Tuple<U, Ts...>>
{
    static constexpr size_t value = 1 + TupleIndex<T, Tuple<Ts...>>::value;
};

} // namespace B33::Core
#endif
