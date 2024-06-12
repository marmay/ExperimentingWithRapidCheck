# Setup

This is a regular Visual Studio solution. I have tested compiling and running
it with Visual Studio 17.9.6, but it should work with other versions. It has
a dependency on the rapidcheck project, which is provided via conan.

If you want to set up the solution or switch between Debug and Release builds,
please run

    conan install . -s build_type=Debug

if you want to create debug builds or

    conan install .

for release builds.

Afterwards, you can use this just like any other Visual Studio solution. If
you get linker errors, make sure that the conan dependency matches your build
type.

## File structure

The `ExperimentingWithRapidCheck.cpp` file contains the main function, which
runs either the `demo01`, `demo02` or `demo03` function.

The demo01 example explores how integer arithmetics work. You can take a look
at it for some very basic RapidCheck syntax.

The demo02 example checks how Reverse works. Please note, it serves more as
a counter-example, as it establishes a property, which is too weak to detect
different kinds of bugs.

The demo03 example is, what we will be working on. We will write tests for
a binary search tree. That tree is implemented in the BST/ directory, which
you can take a look into, but you do not really need to.

If you can run the project, you are set up for our workshop.
