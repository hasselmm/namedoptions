#ifndef TASCHENFORMAT_H
#define TASCHENFORMAT_H

#include <cstdio>
#include <sstream>
#include <tuple>

namespace taschenformat {

template<typename ...Ts>
using format_string = std::string;

namespace detail {

template<size_t N, typename ...Ts>
inline void apply_format(std::string &result, std::tuple<Ts...> &&args)
{
    const auto &subst = (std::ostringstream{} << get<N>(args)).str();
    const auto &marker = '{' + std::to_string(N) + '}';

    for (auto pos = 0u; (pos = result.find(marker, pos)) < result.size(); ) {
        result.replace(pos, marker.length(), subst);
        pos += subst.length();
    }

    if constexpr (N < sizeof...(Ts) - 1) {
        apply_format<N + 1>(result, std::move(args));
    }
}

} // namespace detail

/// This is an absolutely minimal and highly inefficient implementation of std::format(),
/// just for the purposes of the Taschentest library on ancient compilers.
/// It lacks more features of the C++ standard than it implements.
/// Most likely you want to use a different implementation.
template<typename ...Ts>
inline std::string format(const format_string<Ts...> &format, Ts &&...args)
{
    auto result = format;
    detail::apply_format<0>(result, std::tie(args...));
    return result;
}

/// This is an absolutely minimal and highly inefficient implementation of std::println().
/// Most likely you want to use a different implementation.
template<typename ...Ts>
inline void println(std::FILE *stream, const format_string<Ts...> &format, Ts &&...args)
{
    const auto &str = format(format, std::forward<Ts>(args)...);
    std::fwrite(stream, str.data(), str.length());
}

/// This is an absolutely minimal and highly inefficient implementation of std::println().
/// Most likely you want to use a different implementation.
template<typename ...Ts>
inline void println(const format_string<Ts...> &format, Ts &&...args)
{
    println(stdout, format, std::forward<Ts>(args)...);
}

} // namespace taschenformat

#ifndef __cpp_lib_format

namespace std {
using taschenformat::format_string;
using taschenformat::format;
using taschenformat::println;
}

#endif // __cpp_lib_format

#endif // TASCHENFORMAT_H
