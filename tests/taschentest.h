#ifndef TASCHENTEST_H
#define TASCHENTEST_H

#include <format>
#include <functional>
#include <iostream>
#include <source_location>
#include <string>

namespace taschentest {

#define TASCHENTEST_COMPARE(Operator, Actual, Expected) \
    do { \
        const auto &actual = (Actual); \
        const auto &expected = (Expected); \
        assert(actual Operator expected, #Actual " " #Operator " " #Expected, actual, expected); \
    } while(false)

#define TASCHENTEST_VERIFY(Condition) \
    do { \
        auto actual = bool{(Condition)}; \
        assert(actual, #Condition, actual, true); \
    } while(false)

#define TASCHENTEST_TESTCASE(Class, Name) \
    void Name(); \
    const testcase m_test_##Name{this, #Name, &Class::Name}

#ifdef TASCHENTEST_USING_MACROS
#define COMPARE(Operator, Actual, Expected) TASCHENTEST_COMPARE(Operator, (Actual), (Expected))
#define VERIFY(Condition)                   TASCHENTEST_VERIFY((Condition))
#define TESTCASE(Class, Name)               TASCHENTEST_TESTCASE(Class, Name)
#endif

class testsuite
{
public:
    int run();

protected:
    class testcase
    {
    public:
        template<typename T>
        inline explicit testcase(testsuite *fixture, const std::string &name, void (T::* function)());

        void run();

        int passed = 0;
        int failed = 0;

    private:
        std::function<void()> m_function;
        std::string           m_name;
    };

    template<typename T, typename U>
    inline void assert(bool succeded, const std::string &expression,
                       const T &actual_value, const U &expected_value,
                       const std::source_location &source = std::source_location::current());

    void assert(bool succeded, const std::string &expression,
                const std::string &actual_value, const std::string &expected_value,
                const std::source_location &source = std::source_location::current());

    template<typename ...Args>
    inline static void println(const std::format_string<Args...> &format, Args &&...args);

private:
    void add_test(testcase *test);

    int m_total_passed = 0;
    int m_total_failed = 0;

    std::vector<testcase *> m_tests;
    testcase *m_current_test = nullptr;
};

template<typename T>
testsuite::testcase::testcase(testsuite *fixture, const std::string &name, void (T::*function)())
    : m_function{std::bind(function, static_cast<T *>(fixture))}
    , m_name{name}
{
    fixture->add_test(this);
}

template<typename T, typename U>
void testsuite::assert(bool succeded, const std::string &expression,
                       const T &actual_value, const U &expected_value,
                       const std::source_location &source)
{
    using C = std::common_type_t<T, U>;

    assert(succeded, expression,
           std::format("{}", static_cast<C>(actual_value)),
           std::format("{}", static_cast<C>(expected_value)),
           source);
}

template<typename ...Args>
void testsuite::println(const std::format_string<Args...> &format, Args &&...args)
{
    std::cout << std::format(format, std::forward<Args>(args)...) << std::endl;
}

} // namespace taschentest

#endif // TASCHENTEST_H
