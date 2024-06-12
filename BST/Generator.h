#pragma once

#include <rapidcheck.h>

#include <algorithm>

#include "BST.h"

// The implementation of the Valid() function is intentionally inefficient.
// We want this implementation to be obviously correct, so we live with it
// being quadratic complexity or so, even though we could do better.
template <class KeyT, class ValueT>
bool SlowValid(const BST<KeyT, ValueT>& bst) {
  if (!bst) {
    return true;
  }
  return Valid(bst->left) && Valid(bst->right) &&
         std::ranges::all_of(Keys(bst->left),
                             [&](int key) { return key < bst->key; }) &&
         std::ranges::all_of(Keys(bst->right),
                             [&](int key) { return key > bst->key; });
}
template <class KeyT, class ValueT>
bool FastValid(const BST<KeyT, ValueT>& bst) {
  if (!bst) {
    return true;
  }
  if (!Valid(bst->left) || !Valid(bst->right)) {
    return false;
  }
  if (auto m = Max(bst->left); m && m->first >= bst->key) {
    return false;
  }
  if (auto m = Min(bst->right); m && m->first <= bst->key) {
    return false;
  }
  return true;
}

template <class KeyT, class ValueT>
bool Valid(const BST<KeyT, ValueT>& bst) {
#ifdef BST_SLOW_VALID
  return SlowValid(bst);
#else
  return FastValid(bst);
#endif
}

template <class KeyT, class ValueT>
class BSTGen {
 public:
  rc::Shrinkable<BST<KeyT, ValueT>> operator()(const rc::Random& random,
                                               int size) const {
    auto r = random;
    return rc::shrinkable::map(
        rc::gen::arbitrary<std::vector<std::pair<KeyT, ValueT>>>()(random,
                                                                   size),
        [](std::vector<std::pair<KeyT, ValueT>> kvs) {
          BST<KeyT, ValueT> bst = EmptyBST<KeyT, ValueT>;
          for (auto [k, v] : kvs) {
            bst = Insert(bst, k, v);
          }
          return bst;
        });
  }
};

template <class KeyT, class ValueT>
struct rc::Arbitrary<BST<KeyT, ValueT>> {
  static Gen<BST<KeyT, ValueT>> arbitrary() { return BSTGen<KeyT, ValueT>{}; }
};

template <class KeyT, class ValueT>
void showValue(const BST<KeyT, ValueT>& bst, std::ostream& os) {
  if (!bst) {
    os << "Leaf";
  } else {
    os << "{\"key\": " << bst->key << ", \"value\": " << bst->value
       << ", \"left\": ";
    showValue(bst->left, os);
    os << ", \"right\": ";
    showValue(bst->right, os);
    os << "}";
  }
}

template <class KeyT, class ValueT>
std::ostream& operator<<(std::ostream& os, const BST<KeyT, ValueT>& bst) {
  showValue(bst, os);
  return os;
}

namespace std {
template <class T>
void showValue(const std::optional<T>& t, std::ostream& os) {
  if (t.has_value()) {
    os << "optional{" << *t << "}";
  } else {
    os << "nullopt";
  }
}

void showValue(const std::nullopt_t, std::ostream& os) { os << "nullopt"; }
}  // namespace std

template <class T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& t) {
  showValue(t, os);
  return os;
}
