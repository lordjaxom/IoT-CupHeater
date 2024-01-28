#ifndef ESP8266_IOT_VALUETABLE_HPP
#define ESP8266_IOT_VALUETABLE_HPP

#include <cstdlib>
#include <limits>
#include <tuple>

namespace detail
{

    template<typename Function>
    struct FunctionTraits;

    template<typename Result, typename ...Args>
    struct FunctionTraits<Result(Args...)>
    {
        using result_type = Result;
        using argument_types = std::tuple<Args...>;
    };

    template<typename Generator>
    using ValueTableResult = typename FunctionTraits<decltype(Generator::apply)>::result_type;

    template<typename Generator>
    using ValueTableInput = typename std::tuple_element<
            0, typename FunctionTraits<decltype(Generator::apply)>::argument_types>::type;

    template<
            typename Generator,
            ValueTableInput<Generator> I,
            ValueTableInput<Generator> To,
            ValueTableResult<Generator> ...Values
    >
    struct ValueTable
            : ValueTable<Generator, I + 1, To, Values..., Generator::apply(I)>
    {
    };

    template<
            typename Generator,
            ValueTableInput<Generator> To,
            ValueTableResult<Generator> ...Values
    >
    struct ValueTable<Generator, To, To, Values...>
    {
        static constexpr ValueTableResult<Generator> values[]{Values..., Generator::apply(To)};
    };

    template<
            typename Generator,
            ValueTableInput<Generator> To,
            ValueTableResult<Generator> ...Values
    >
    constexpr ValueTableResult<Generator> ValueTable<Generator, To, To, Values...>::values[];

} // namespace detail

template<
        typename Generator,
        detail::ValueTableInput<Generator> From = std::numeric_limits<detail::ValueTableInput<Generator> >::min(),
        detail::ValueTableInput<Generator> To = std::numeric_limits<detail::ValueTableInput<Generator> >::max()
>
struct ValueTable
        : detail::ValueTable<Generator, From, To>
{
    using Table = ValueTable;
    using Result = detail::ValueTableResult<Generator>;
    using Input = detail::ValueTableInput<Generator>;

    static constexpr auto from = From;
    static constexpr auto to = To;

    static constexpr std::size_t size()
    {
        return sizeof(ValueTable::values) / sizeof(ValueTable::values[0]);
    }

    static constexpr Result get(Input input)
    {
        return ValueTable::values[input - from];
    }
};

#endif // ESP8266_IOT_VALUETABLE_HPP
