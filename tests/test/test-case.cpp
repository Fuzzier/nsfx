#include <nsfx/test.h>
#include <functional>
#include <iostream>


void foo(void)
{
    std::cout << "foo" << std::endl;
}

class A
{
public:
    A(int i) : i_(i) {}

    void bar(void)
    {
        std::cout << "A::bar " << i_ << std::endl;
    }
    int i_;
};

class B
{
public:
    void operator()(void)
    {
        std::cout << "B::operator()" << std::endl;
    }
};

/**
 * @brief Test case.
 *
 * Three types of test cases:
 * case1: lamba expression.
 * case2: function pointer.
 * case3: bind expression.
 * case4: functor.
 */
int main(void)
{
    nsfx::test::Case* c1 =
        nsfx::test::runner::getMasterSuite()->addCase("case1", [] {
            std::cout << "Hello world!" << std::endl;
        });
    c1->run();

    c1 = nsfx::test::runner::getMasterSuite()->addCase("case2", &foo);
    c1->run();

    A a(1);
    c1 = nsfx::test::runner::getMasterSuite()->addCase(
        "case3", std::bind(&A::bar, &a));
    c1->run();

    B b;
    c1 = nsfx::test::runner::getMasterSuite()->addCase("case4", b);
    c1->run();

    return 0;
}

