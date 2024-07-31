#ifndef NAMEDOPTIONS_H
#define NAMEDOPTIONS_H

#include "namedoptions_p.h"

#include <cstdint>

namespace named_options {

template<typename ...Ts>
class options : public internal::options_variant<Ts...>
{
private:
    static constexpr std::size_t variant_index(std::size_t n) noexcept { return n + 1; }

public:
    using variant_type = internal::options_variant<Ts...>;

    template<std::size_t N>
    using option_value_type = std::variant_alternative_t<variant_index(N), variant_type>;

    template<std::size_t N, typename T, typename O = options<Ts...>,
             typename OT = internal::option_traits<O, O>>
    class option
    {
    public:
        using options_type  = O;
        using option_traits = OT;
        using value_type    = T;
        using variant_type  = options::variant_type;

        option() = default;
        option(T &&value) { variant()->template emplace<variant_index()>(std::forward<T>(value)); }

        static constexpr std::size_t option_index() noexcept { return N - OT::option_offset; }
        static constexpr std::size_t variant_index() noexcept { return options::variant_index(option_index()); }

        constexpr bool has_value() const { return variant()->index() == variant_index(); }
        constexpr explicit operator bool() const { return has_value(); }

        constexpr const T *value_if() const { return std::get_if<variant_index()>(variant()); }
        constexpr const T *operator->() const { return value_if(); }

        constexpr T value() const { return std::get<variant_index()>(*variant()); }
        constexpr T operator*() const { return value(); }

        template<typename U>
        constexpr T value_or(U &&default_value) const
        { return *this ? **this : static_cast<T>(std::forward<U>(default_value)); }

        const variant_type *variant() const noexcept { return reinterpret_cast<const variant_type *>(p()); }
        variant_type *variant() noexcept { return reinterpret_cast<variant_type *>(p()); }

        static_assert(option_index() >= 0, "Option index `N` must be within range for `options_type`.");
        static_assert(option_index() < options::size(), "Option index `N` must be within range for `options_type`.");

        static_assert(std::is_same_v<value_type, option_value_type<option_index()>>,
                      "Template parameter `T` must match N-th element type of `options_type`.");

        static_assert(variant_index() >= options::variant_index(0),
                      "Option index `N` must be within range for `variant_type`.");
        static_assert(variant_index() < std::variant_size_v<variant_type>,
                      "Option index `N` must be within range for `variant_type`.");

    private:
        std::intptr_t p() const noexcept
        {
            return reinterpret_cast<std::intptr_t>(this)
                    - offset<O>(N) + option_traits::variant_offset;
        }
    };

    constexpr static std::size_t size()
    {
        return std::variant_size_v<variant_type> - variant_index(0);
    }

private:
    template<typename OT>
    static std::intptr_t offset(std::size_t n) noexcept
    {
        static_assert(alignof(option<0, option_value_type<0>>) == 1);
        static_assert(sizeof(option<0, option_value_type<0>>) == 1);

        struct alignment_dummy : public OT
        {
            option<0, option_value_type<0>> arg;
        };

        const auto alignment = alignment_dummy{};
        const auto begin = reinterpret_cast<std::intptr_t>(&alignment);
        const auto end = reinterpret_cast<std::intptr_t>(&alignment.arg);

        return (end - begin) + n * sizeof(decltype(alignment.arg));
    }
};

template<std::size_t N, typename ...Ts>
using option_type_t = options<Ts...>::template option_value_type<N>;

template<std::size_t N, typename ...Ts>
using option_t = options<Ts...>::template option<N, option_type_t<N, Ts...>>;

} // namespace named_options

#endif // NAMEDOPTIONS_H
