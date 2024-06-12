#include <rapidcheck.h>

// Some very simple RapidCheck properties.
//
// We will quickly go over some simple properties, before we start writing our
// own code.
void demo01()
{
    // 1. Adding a number to itself results in an even number.
    //    It might be a bit surprising that this property holds for all integers,
    //    even when overflow occurs, but, yes, this is actually true.
    rc::check("Adding a number to itself results in an even number.",
        [](const int a)
        {
            RC_ASSERT((a + a) % 2 == 0);
        });

    // 2. This property illustrates what a failing property looks like. This is
    //    obviously not true for negative numbers and, indeed, RapidCheck will
    //    find -1 as a minimal failing example, at least in most cases.
    rc::check("Adding a number to itself results in a number that is greater or equal.",
        [](const int a)
        {
            RC_ASSERT(a <= a + a);
        });

    // 3. Okay, the property above fails for negative numbers. But surely it holds
    //    for non-negative ones, right? We use a pre-condition to discard test cases
    //    with negative numbers. This has some important implications, that we will
    //    discuss later.
    //
    //    When checking this property, RapidCheck will also find a counter-example
    //    due to an integer overflow. After shrinking, it should find 1073741824 as
    //    a minimal failing example.
    rc::check("Adding a positive number to itself results in a bigger number.",
        [](const int a)
        {
            RC_PRE(a >= 0);
            RC_ASSERT(a <= a + a);
        });
}
