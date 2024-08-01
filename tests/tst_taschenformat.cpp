#include "taschenformat.h"
#include "taschentest.h"

namespace taschenformat::tests {
namespace {

class taschenformat_test : public taschentest::testsuite
{
protected:
    TESTCASE(taschenformat_test, simple_format);
};

void taschenformat_test::simple_format()
{
    COMPARE(==, format("{0}", 1), "1");
    COMPARE(==, format("<{0}>", 2), "<2>");
    COMPARE(==, format("<{0}/{1}/{2}>", 3, "foo", false), "<3/foo/0>");
    COMPARE(==, format("{3}{1}{0}{1}{2}", 0, 1, 2, 3), "31012");
}

} // namespace
} // namespace taschenformat::tests

int main()
{
    return taschenformat::tests::taschenformat_test{}.run();
}
