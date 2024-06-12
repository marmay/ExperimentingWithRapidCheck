#include <rapidcheck.h>

// You can enable bugs in the implementation of the binary search tree.

// Plant bugs in the implementation of Insert.
// Select a number from 1 to 3 to enable different bugs.
// #define BST_INSERT_BUG 1

// Plant bugs in the implementation of Delete.
// Select a number from 1 to 2 to enable different bugs.
// #define BST_DELETE_BUG 1

// Plant bugs in the implementation of Union.
// Select a number from 1 to 3 to enable different bugs.
// #define BST_UNION_BUG 1

#include "BST/BST.h"
#include "BST/Generator.h"

// This is where you are going to write your code. We have to select specific
// types that we want to test our BST class for:
using KeyType = int;
using ValueType = int;
using MyBST = BST<KeyType, ValueType>;

void demo03() {
  // Test your tests!
  // Whenever we write a generator, we have to check that it generates valid
  // data. We also would want to check that the shrinking is correct, but this
  // is not as easy in RapidCheck, so we skip this for now.
  rc::check("The generator generates only valid BSTs.",
            [](MyBST bst) { RC_ASSERT(Valid(bst)); });

  // The first kind of properties are Invariant properties. We test that all
  // operations preserve the Valid invariant of BSTs.
  rc::check("Invariant: Insert", [](MyBST bst, KeyType key, ValueType value) {
    RC_ASSERT(Valid(Insert(bst, key, value)));
  });
  rc::check("Invariant: Delete",
            [](MyBST bst, KeyType key) { RC_ASSERT(Valid(Delete(bst, key))); });
  rc::check("Invariant: Union",
            [](MyBST lhs, MyBST rhs) { RC_ASSERT(Valid(Union(lhs, rhs))); });
}
