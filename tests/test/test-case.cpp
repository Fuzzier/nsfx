/**
 * @file
 *
 * @brief test TestCase.
 *
 * @version 1.0
 * @author  wei tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-07
 *
 * @copyright copyright (c) 2017.
 *   national key laboratory of science and technology on communications,
 *   university of electronic science and technology of china.
 *   all rights reserved.
 */

#include <nsfx/test.h>
#include <functional>
#include <iostream>


void Foo(void)
{
    std::cout << "Foo" << std::endl;
}

class A
{
public:
    A(int i) : i_(i) {}

    void Bar(void)
    {
        std::cout << "A::Bar " << i_ << std::endl;
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
        nsfx::test::runner::GetMasterSuite()->AddCase("case1", [] {
            std::cout << "Hello world!" << std::endl;
        });
    c1->Run();

    c1 = nsfx::test::runner::GetMasterSuite()->AddCase("case2", &Foo);
    c1->Run();

    A a(1);
    c1 = nsfx::test::runner::GetMasterSuite()->AddCase(
        "case3", std::bind(&A::Bar, &a));
    c1->Run();

    B b;
    c1 = nsfx::test::runner::GetMasterSuite()->AddCase("case4", b);
    c1->Run();

    return 0;
}

