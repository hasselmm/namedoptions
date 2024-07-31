#include "namedoptions.h"

#include "taschentest.h"

namespace named_options::tests {
namespace {

class named_options_test : public taschentest::testsuite
{
protected:
    TESTCASE(named_options_test,   simple_options);
    TESTCASE(named_options_test,    mixed_options);
    TESTCASE(named_options_test, composed_options);
    TESTCASE(named_options_test,    multi_options);
};

void named_options_test::simple_options()
{
    struct simple_options : public options<bool, int, double>
    {
        option<0, bool>   boolean = {};
        option<1, int>    integer = {};
        option<2, double> decimal = {};
    };

    {
        const auto test0 = simple_options{};

        VERIFY(!test0.boolean.has_value());
        VERIFY(!test0.boolean);

        VERIFY(!test0.integer.has_value());
        VERIFY(!test0.integer);

        VERIFY(!test0.decimal.has_value());
        VERIFY(!test0.decimal);
    }

    {
        const auto test1 = simple_options{.boolean = true};

        VERIFY( test1.boolean.has_value());
        VERIFY(!test1.integer.has_value());
        VERIFY(!test1.decimal.has_value());

        COMPARE(==, test1.boolean.value(), true);
    }

    {
        const auto test2 = simple_options{.integer = 2};

        VERIFY(!test2.boolean.has_value());
        VERIFY( test2.integer.has_value());
        VERIFY(!test2.decimal.has_value());

        COMPARE(==, test2.integer.value(), 2);
    }

    {
        const auto test3 = simple_options{.decimal = 3};

        VERIFY(!test3.boolean.has_value());
        VERIFY(!test3.integer.has_value());
        VERIFY( test3.decimal.has_value());

        COMPARE(==, test3.decimal.value(), 3);
    }

    {
        const auto test4 = simple_options{.boolean = true, .integer = 4};

        VERIFY(!test4.boolean.has_value());
        VERIFY( test4.integer.has_value());
        VERIFY(!test4.decimal.has_value());

        COMPARE(==, test4.integer.value(), 4);
    }

    {
        const auto test5 = simple_options{.boolean = true, .integer = 5, .decimal = 6};

        VERIFY(!test5.boolean.has_value());
        VERIFY(!test5.integer.has_value());
        VERIFY( test5.decimal.has_value());

        COMPARE(==, test5.decimal.value(), 6);
    }
}

void named_options_test::mixed_options()
{
    struct mixed_options : public options<bool, int>
    {
        option<0, bool>         boolean = {};
        option<1, int>          integer = {};
        std::optional<double>   decimal = {};
    };

    {
        const auto test0 = mixed_options{};

        VERIFY(!test0.boolean.has_value());
        VERIFY(!test0.integer.has_value());
        VERIFY(!test0.decimal.has_value());
    }

    {
        const auto test1 = mixed_options{.boolean = true};

        VERIFY( test1.boolean.has_value());
        VERIFY(!test1.integer.has_value());
        VERIFY(!test1.decimal.has_value());

        COMPARE(==, test1.boolean.value(), true);
    }

    {
        const auto test2 = mixed_options{.integer = 2};

        VERIFY(!test2.boolean.has_value());
        VERIFY( test2.integer.has_value());
        VERIFY(!test2.decimal.has_value());

        COMPARE(==, test2.integer.value(), 2);
    }

    {
        const auto test3 = mixed_options{.decimal = 3};

        VERIFY(!test3.boolean.has_value());
        VERIFY(!test3.integer.has_value());
        VERIFY( test3.decimal.has_value());

        COMPARE(==, test3.decimal.value(), 3);
    }

    {
        const auto test4 = mixed_options{.boolean = true, .integer = 4};

        VERIFY(!test4.boolean.has_value());
        VERIFY( test4.integer.has_value());
        VERIFY(!test4.decimal.has_value());

        COMPARE(==, test4.integer.value(), 4);
    }

    {
        const auto test5 = mixed_options{.boolean = true, .integer = 5, .decimal = 6};

        VERIFY(!test5.boolean.has_value());
        VERIFY( test5.integer.has_value());
        VERIFY( test5.decimal.has_value());

        COMPARE(==, test5.integer.value(), 5);
        COMPARE(==, test5.decimal.value(), 6);
    }
}

void named_options_test::composed_options()
{
    struct simple_options : public options<bool, int, double>
    {
        option<0, bool>   boolean = {};
        option<1, int>    integer = {};
    };

    struct composed_options
    {
        simple_options a = {};
        simple_options b = {};
    };

    {
        const auto test0 = composed_options{};

        VERIFY(!test0.a.boolean.has_value());
        VERIFY(!test0.a.integer.has_value());
        VERIFY(!test0.b.boolean.has_value());
        VERIFY(!test0.b.integer.has_value());
    }

    {
        const auto test1 = composed_options{.a = {.boolean = true}};

        VERIFY( test1.a.boolean.has_value());
        VERIFY(!test1.a.integer.has_value());
        VERIFY(!test1.b.boolean.has_value());
        VERIFY(!test1.b.integer.has_value());

        COMPARE(==, test1.a.boolean.value(), true);
    }

    {
        const auto test2 = composed_options{.a = {.integer = 1}};

        VERIFY(!test2.a.boolean.has_value());
        VERIFY( test2.a.integer.has_value());
        VERIFY(!test2.b.boolean.has_value());
        VERIFY(!test2.b.integer.has_value());

        COMPARE(==, test2.a.integer.value(), 1);
    }

    {
        const auto test3 = composed_options{.b = {.boolean = true}};

        VERIFY(!test3.a.boolean.has_value());
        VERIFY(!test3.a.integer.has_value());
        VERIFY( test3.b.boolean.has_value());
        VERIFY(!test3.b.integer.has_value());

        COMPARE(==, test3.b.boolean.value(), true);
    }

    {
        const auto test4 = composed_options{.b = {.integer = 2}};

        VERIFY(!test4.a.boolean.has_value());
        VERIFY(!test4.a.integer.has_value());
        VERIFY(!test4.b.boolean.has_value());
        VERIFY( test4.b.integer.has_value());

        COMPARE(==, test4.b.integer.value(), 2);
    }

    {
        const auto test5 = composed_options {
            .a = {.boolean = true},
            .b = {.integer = 2}
        };

        VERIFY( test5.a.boolean.has_value());
        VERIFY(!test5.a.integer.has_value());
        VERIFY(!test5.b.boolean.has_value());
        VERIFY( test5.b.integer.has_value());

        COMPARE(==, test5.b.integer.value(), 2);
    }

    {
        const auto test6 = composed_options {
            .a = {.boolean = true, .integer = 3},
            .b = {.boolean = true, .integer = 4}
        };

        VERIFY(!test6.a.boolean.has_value());
        VERIFY( test6.a.integer.has_value());
        VERIFY(!test6.b.boolean.has_value());
        VERIFY( test6.b.integer.has_value());

        COMPARE(==, test6.a.integer.value(), 3);
        COMPARE(==, test6.b.integer.value(), 4);
    }
}

template<typename ...Os>
struct multi : public Os...
{
    template<std::size_t I, typename O, typename T>
    using option = O::template option<I, T, multi, internal::option_traits<O, Os...>>;
};

void named_options_test::multi_options()
{
    using test_options = options<bool, int>;

    struct a : public test_options {};
    struct b : public test_options {};

    struct multi_options : public multi<a, b>
    {
        option<0, a, bool>      a_boolean = {};
        option<1, a, int>       a_integer = {};
        option<2, b, bool>      b_boolean = {};
        option<3, b, int>       b_integer = {};
        std::optional<double>   c_decimal = {};
    };

    static_assert(decltype(multi_options::a_boolean)::option_index() == 0);
    static_assert(decltype(multi_options::a_integer)::option_index() == 1);
    static_assert(decltype(multi_options::b_boolean)::option_index() == 0);
    static_assert(decltype(multi_options::b_integer)::option_index() == 1);

    static_assert(decltype(multi_options::a_boolean)::variant_index() == 1);
    static_assert(decltype(multi_options::a_integer)::variant_index() == 2);
    static_assert(decltype(multi_options::b_boolean)::variant_index() == 1);
    static_assert(decltype(multi_options::b_integer)::variant_index() == 2);

    static_assert(decltype(multi_options::a_boolean)::option_traits::multi_option_index == 0);
    static_assert(decltype(multi_options::a_integer)::option_traits::multi_option_index == 0);
    static_assert(decltype(multi_options::b_boolean)::option_traits::multi_option_index == 1);
    static_assert(decltype(multi_options::b_integer)::option_traits::multi_option_index == 1);

    static_assert(decltype(multi_options::a_boolean)::option_traits::variant_offset == 0);
    static_assert(decltype(multi_options::a_integer)::option_traits::variant_offset == 0);
    static_assert(decltype(multi_options::b_boolean)::option_traits::variant_offset == sizeof(a));
    static_assert(decltype(multi_options::b_integer)::option_traits::variant_offset == sizeof(a));

    {
        const auto testm = multi_options{};

        const auto iptr = [base = &testm](auto p) {
            return reinterpret_cast<std::uintptr_t>(p)
                    - reinterpret_cast<std::uintptr_t>(base);
        };

        const auto testa = static_cast<const a *>(&testm);
        const auto testb = static_cast<const b *>(&testm);

        constexpr auto options_padding = sizeof(void *);

        COMPARE(==, iptr(&testm), 0u);
        COMPARE(==, iptr( testa), 0u);
        COMPARE(==, iptr( testb), sizeof(a));
        COMPARE(==, iptr(&testm.c_decimal), sizeof(a) + sizeof(b) + options_padding);

        COMPARE(==, iptr(testm.a_boolean.variant()), iptr(testa));
        COMPARE(==, iptr(testm.a_integer.variant()), iptr(testa));
        COMPARE(==, iptr(testm.b_boolean.variant()), iptr(testb));
        COMPARE(==, iptr(testm.b_integer.variant()), iptr(testb));
    }

    {
        const auto test0 = multi_options{};

        VERIFY(!test0.a_boolean.has_value());
        VERIFY(!test0.a_integer.has_value());
        VERIFY(!test0.b_boolean.has_value());
        VERIFY(!test0.b_integer.has_value());
        VERIFY(!test0.c_decimal.has_value());
    }

    {
        const auto test1 = multi_options{.a_boolean = true};

        VERIFY( test1.a_boolean.has_value());
        VERIFY(!test1.a_integer.has_value());
        VERIFY(!test1.b_boolean.has_value());
        VERIFY(!test1.b_integer.has_value());
        VERIFY(!test1.c_decimal.has_value());

        COMPARE(==, test1.a_boolean.value(), true);
    }

    {
        const auto test2 = multi_options{.a_integer = 1};

        VERIFY(!test2.a_boolean.has_value());
        VERIFY( test2.a_integer.has_value());
        VERIFY(!test2.b_boolean.has_value());
        VERIFY(!test2.b_integer.has_value());
        VERIFY(!test2.c_decimal.has_value());

        COMPARE(==, test2.a_integer.value(), 1);
    }

    {
        const auto test3 = multi_options{.b_boolean = true};

        VERIFY(!test3.a_boolean.has_value());
        VERIFY(!test3.a_integer.has_value());
        VERIFY( test3.b_boolean.has_value());
        VERIFY(!test3.b_integer.has_value());
        VERIFY(!test3.c_decimal.has_value());

        COMPARE(==, test3.b_boolean.value(), true);
    }

    {
        const auto test4 = multi_options{.b_integer = 2};

        VERIFY(!test4.a_boolean.has_value());
        VERIFY(!test4.a_integer.has_value());
        VERIFY(!test4.b_boolean.has_value());
        VERIFY( test4.b_integer.has_value());
        VERIFY(!test4.c_decimal.has_value());

        COMPARE(==, test4.b_integer.value(), 2);
    }

    {
        const auto test5 = multi_options{.c_decimal = 3};

        VERIFY(!test5.a_boolean.has_value());
        VERIFY(!test5.a_integer.has_value());
        VERIFY(!test5.b_boolean.has_value());
        VERIFY(!test5.b_integer.has_value());
        VERIFY( test5.c_decimal.has_value());

        COMPARE(==, test5.c_decimal.value(), 3);
    }

    {
        const auto test6 = multi_options{.a_boolean = true, .b_boolean = true};

        VERIFY( test6.a_boolean.has_value());
        VERIFY(!test6.a_integer.has_value());
        VERIFY( test6.b_boolean.has_value());
        VERIFY(!test6.b_integer.has_value());
        VERIFY(!test6.c_decimal.has_value());

        COMPARE(==, test6.a_boolean.value(), true);
        COMPARE(==, test6.b_boolean.value(), true);
    }

    {
        const auto test7 = multi_options{.a_integer = 4, .b_integer = 5};

        VERIFY(!test7.a_boolean.has_value());
        VERIFY( test7.a_integer.has_value());
        VERIFY(!test7.b_boolean.has_value());
        VERIFY( test7.b_integer.has_value());
        VERIFY(!test7.c_decimal.has_value());

        COMPARE(==, test7.a_integer.value(), 4);
        COMPARE(==, test7.b_integer.value(), 5);
    }

    {
        const auto test8 = multi_options{.a_boolean = true, .a_integer = 6, .b_integer = 7, .c_decimal = 8};

        VERIFY(!test8.a_boolean.has_value());
        VERIFY( test8.a_integer.has_value());
        VERIFY(!test8.b_boolean.has_value());
        VERIFY( test8.b_integer.has_value());
        VERIFY( test8.c_decimal.has_value());

        COMPARE(==, test8.a_integer.value(), 6);
        COMPARE(==, test8.b_integer.value(), 7);
        COMPARE(==, test8.c_decimal.value(), 8);
    }
}

} // namespace
} // namespace named_options::tests

int main()
{
    return named_options::tests::named_options_test{}.run();
}
