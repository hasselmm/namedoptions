#ifndef NAMEDOPTIONS_P_H
#define NAMEDOPTIONS_P_H

#include <tuple>
#include <variant>

namespace named_options::internal {

template<std::size_t N, typename ...Ts>
using pack_element_t = std::tuple_element_t<N, std::tuple<Ts...>>;

template<std::size_t N, typename T, typename ...Ts>
constexpr std::size_t pack_index_of()
{
    if constexpr (std::is_same_v<T, pack_element_t<N, Ts...>>) {
        return N;
    } else {
        return pack_index_of<N + 1, T, Ts...>();
    }
}

template<typename T, typename ...Ts>
constexpr std::size_t pack_element_index_v = pack_index_of<0, T, Ts...>();

template<std::size_t I, typename ...Os>
constexpr std::size_t find_option_offset()
{
    if constexpr (I > 0) {
        constexpr auto option_count = pack_element_t<I - 1, Os...>::size();
        return option_count + find_option_offset<I - 1, Os...>();
    } else {
        return 0;
    }
}

template<std::size_t I, typename ...Os>
constexpr std::size_t option_offset_v = find_option_offset<I, Os...>();

template<std::size_t I, typename ...Os>
constexpr std::size_t find_variant_offset()
{
    if constexpr (I > 0) {
        constexpr auto option_size = sizeof(pack_element_t<I - 1, Os...>);
        return option_size + find_option_offset<I - 1, Os...>();
    } else {
        return 0;
    }
}

template<std::size_t I, typename ...Os>
constexpr std::size_t variant_offset_v = find_variant_offset<I, Os...>();

template<typename O, typename ...Os>
struct option_traits
{
    static constexpr std::size_t multi_option_index = pack_element_index_v<O, Os...>;
    static constexpr std::size_t option_offset = option_offset_v<multi_option_index, Os...>;
    static constexpr std::size_t variant_offset = variant_offset_v<multi_option_index, Os...>;
};

template<typename ...Ts>
using options_variant = std::variant<std::monostate, Ts...>;

} // namespace named_options::internal

#endif // NAMEDOPTIONS_P_H
