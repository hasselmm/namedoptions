#include "taschentest.h"

using namespace std::string_view_literals;

namespace taschentest {
namespace {

const auto s_passed  = "\033[0;32m"sv;
const auto s_failed  = "\033[0;31m"sv;
const auto s_neutral = "\033[0m"sv;

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
        println("{0}PASS{1}   : {0}{2} test(s) passed{1}",
                s_passed, s_neutral, m_total_passed);

        return EXIT_SUCCESS;
    } else {
        println("{1}FAIL!{2}  : {0}{3} test(s) passed{2}, {1}{4} test(s) failed{2}",
                s_passed, s_failed, s_neutral, m_total_passed, m_total_failed);

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
        println("{1}PASS{2}   : {0}() assertion confirmed: {3}",
                m_current_test->name(), s_passed, s_neutral,
                expression);

        ++m_current_test->passed;
    } else {
        println("{1}FAIL!{2}  : {0}() assertion failed: {1}{3}{2}\n"
                "   Actual:   {4}\n"
                "   Expected: {5}\n"
                "{6}({7}) : failure location",
                m_current_test->name(), s_failed, s_neutral,
                expression, actual_value, expected_value,
                source.file_name(), source.line());

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
        println("{1}PASS{2}   : {0}() {1}{3} test(s) passed{2}",
                m_name, s_passed, s_neutral, passed);
    } else {
        println("{2}FAIL!{3}  : {0}() {1}{4} test(s) passed{3}, {2}{5} test(s) failed{3}",
                m_name, s_passed, s_failed, s_neutral, passed, failed);
    }
}

} // namespace taschentest
