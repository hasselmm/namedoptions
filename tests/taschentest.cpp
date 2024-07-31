#include "taschentest.h"

using namespace std::string_view_literals;

namespace taschentest {
namespace {

const auto s_passed  = "\033[0;32m"sv;
const auto s_failed  = "\033[0;31m"sv;
const auto s_weak    = "\033[0;37m"sv;
const auto s_neutral = "\033[0m"sv;

std::string short_function_name(const std::string &name)
{
    const auto n = name.rfind(':');
    return name.substr(n + 1, name.length() - n - 3);
}

} // namespace

int testsuite::run()
{
    for (auto test: m_tests) {
        m_current_test = test;
        m_current_test->run();
        m_current_test = nullptr;

        m_total_passed += test->passed;
        m_total_failed += test->failed;
    }

    if (m_total_failed == 0) {
        println("{0}TEST SUITE PASSED{1} ({2} test(s) executed)",
                s_passed, s_neutral, m_total_passed);

        return EXIT_SUCCESS;
    } else {
        println("{0}TEST SUITE FAILED{1} ({2} test(s) passed, {3} test(s) failed)",
                s_failed, s_neutral, m_total_passed, m_total_failed);

        return EXIT_FAILURE;
    }
}

void testsuite::assert(bool succeded,
                       const std::string &expression,
                       const std::string &actual_value,
                       const std::string &expected_value,
                       const std::source_location &source)
{
    if (succeded) {
        println("{0}: {3}assertion confirmed{5}: {6} {4}({1}, line {2}){5}",
                short_function_name(source.function_name()),
                source.file_name(), source.line(),
                s_passed, s_weak, s_neutral,
                expression);

        ++m_current_test->passed;
    } else {
        println("{0}: {3}assertion failed{5}: {6} {4}({1}, line {2}){5}\n"
                "  - actual:   {7}\n"
                "  - expected: {8}",
                short_function_name(source.function_name()),
                source.file_name(), source.line(),
                s_failed, s_weak, s_neutral,
                expression, actual_value, expected_value);

        ++m_current_test->failed;
    }
}

void testsuite::add_test(testcase *test)
{
    m_tests.emplace_back(test);
}

void testsuite::testcase::run()
{
    passed = 0;
    failed = 0;

    m_function();

    if (failed == 0) {
        println("{0}: {1}TEST PASSED{2} ({3} test(s) executed)",
                m_name, s_passed, s_neutral, passed);
    } else {
        println("{0}: {1}TEST FAILED{2} ({3} test(s) passed, {4} test(s) failed)",
                m_name, s_failed, s_neutral, passed, failed);
    }
}

} // namespace taschentest
