#pragma once

#include <algorithm>
#include <list>
#include <optional>
#include <variant>

// This file provides an implementation of a binary search tree (BST).
// It is pretty naive and most ways and provides the following API:
//
//    EmptyBST<KeyT, ValueT>    -- a constant representing an empty tree
//    Insert(bst, key, value)   -- inserts a key-value pair into the tree
//    Delete(bst, key)          -- deletes a key from the tree
//    Union(lhs, rhs)           -- returns the union of two trees
//    Find(bst, key)            -- finds a key in the tree
//    ToList(bst)               -- returns a list of key-value pairs in the tree
//    FromRange<K, V>(rng)      -- generates a BST from a range of key-value
//    Keys(bst)                 -- returns a list of keys in the tree
//
// We use this class as a demonstration of how to write properties for a simple,
// but non-trivial, data structure. In order to make testing more interesting,
// there are some bugs deliberately placed in the implementation. You can enable
// them by setting the following preprocessor definitions
//
//    #define BST_INSERT_BUG    1
//    #define BST_INSERT_BUG    2
//	  #define BST_INSERT_BUG    3
//    #define BST_DELETE_BUG    1
//    #define BST_DELETE_BUG    2
//    #define BST_UNION_BUG     1
//    #define BST_UNION_BUG     2
//    #define BST_UNION_BUG     3
//
// Please do not look at this file in detail for now. We want to reason about
// the bugs without knowing the implementation details at first.

template <class KeyT, class ValueT>
struct BSTNode {
  BSTNode(KeyT key, ValueT value,
          std::shared_ptr<const BSTNode<KeyT, ValueT>> left,
          std::shared_ptr<const BSTNode<KeyT, ValueT>> right)
      : key(key),
        value(value),
        left(std::move(left)),
        right(std::move(right)) {}

  KeyT key;
  ValueT value;
  std::shared_ptr<const BSTNode<KeyT, ValueT>> left;
  std::shared_ptr<const BSTNode<KeyT, ValueT>> right;
};

template <class KeyT, class ValueT>
using BST = std::shared_ptr<const BSTNode<KeyT, ValueT>>;

template <class KeyT, class ValueT>
const BST<KeyT, ValueT> EmptyBST = nullptr;

template <class KeyT, class ValueT>
BST<KeyT, ValueT> MkNode(KeyT key, ValueT value, BST<KeyT, ValueT> left,
                         BST<KeyT, ValueT> right) {
  return std::make_shared<const BSTNode<KeyT, ValueT>>(key, value, left, right);
}

#if defined(BST_INSERT_BUG) && BST_INSERT_BUG == 1
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Insert(BST<KeyT, ValueT> bst, const KeyT& key,
                         const ValueT& value) {
  return MkNode(key, value, EmptyBST<KeyT, ValueT>, EmptyBST<KeyT, ValueT>);
}
#elif defined(BST_INSERT_BUG) && BST_INSERT_BUG == 2
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Insert(BST<KeyT, ValueT> bst, const KeyT& key,
                         const ValueT& value) {
  if (!bst) {
    return MkNode(key, value, EmptyBST<KeyT, ValueT>, EmptyBST<KeyT, ValueT>);
  }
  if (key <= bst->key) {
    return MkNode(bst->key, bst->value, Insert(bst->left, key, value),
                  bst->right);
  } else {
    return MkNode(bst->key, bst->value, bst->left,
                  Insert(bst->right, key, value));
  }
}
#elif defined(BST_INSERT_BUG) && BST_INSERT_BUG == 3
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Insert(BST<KeyT, ValueT> bst, const KeyT& key,
                         const ValueT& value) {
  if (!bst) {
    return MkNode(key, value, EmptyBST<KeyT, ValueT>, EmptyBST<KeyT, ValueT>);
  }
  if (key < bst->key) {
    return MkNode(bst->key, bst->value, Insert(bst->left, key, value),
                  bst->right);
  }
  if (key > bst->key) {
    return MkNode(bst->key, bst->value, bst->left,
                  Insert(bst->right, key, value));
  }
  return MkNode(bst->key, bst->value, bst->left, bst->right);
}
#else
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Insert(BST<KeyT, ValueT> bst, const KeyT& key,
                         const ValueT& value) {
  if (!bst) {
    return MkNode(key, value, EmptyBST<KeyT, ValueT>, EmptyBST<KeyT, ValueT>);
  }
  if (key < bst->key) {
    return MkNode(bst->key, bst->value, Insert(bst->left, key, value),
                  bst->right);
  }
  if (key > bst->key) {
    return MkNode(bst->key, bst->value, bst->left,
                  Insert(bst->right, key, value));
  }
  return MkNode(key, value, bst->left, bst->right);
}
#endif

#if defined(BST_DELETE_BUG) && BST_DELETE_BUG == 1
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Delete(BST<KeyT, ValueT> bst, const KeyT& key) {
  if (!bst) {
    return bst;
  }
  if (key < bst->key) {
    return Delete(bst->left, key);
  }
  if (key > bst->key) {
    return Delete(bst->right, key);
  }
  if (!bst->left && !bst->right) {
    return EmptyBST<KeyT, ValueT>;
  }

  if (!bst->left) {
    return bst->right;
  }
  if (!bst->right) {
    return bst->left;
  }
  auto min = bst->left.get();
  while (min->right) {
    min = min->right.get();
  }
  return MkNode(min->key, min->value, Delete(bst->left, min->key), bst->right);
}
#elif defined(BST_DELETE_BUG) && BST_DELETE_BUG == 2
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Delete(BST<KeyT, ValueT> bst, const KeyT& key) {
  if (!bst) {
    return bst;
  }
  if (key > bst->key) {
    return MkNode(bst->key, bst->value, Delete(bst->left, key), bst->right);
  }
  if (key < bst->key) {
    return MkNode(bst->key, bst->value, bst->left, Delete(bst->right, key));
  }
  if (!bst->left && !bst->right) {
    return EmptyBST<KeyT, ValueT>;
  }

  if (!bst->left) {
    return bst->right;
  }
  if (!bst->right) {
    return bst->left;
  }
  auto min = bst->left.get();
  while (min->right) {
    min = min->right.get();
  }
  return MkNode(min->key, min->value, Delete(bst->left, min->key), bst->right);
}
#else
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Delete(BST<KeyT, ValueT> bst, const KeyT& key) {
  if (!bst) {
    return bst;
  }
  if (key < bst->key) {
    return MkNode(bst->key, bst->value, Delete(bst->left, key), bst->right);
  }
  if (key > bst->key) {
    return MkNode(bst->key, bst->value, bst->left, Delete(bst->right, key));
  }
  if (!bst->left && !bst->right) {
    return EmptyBST<KeyT, ValueT>;
  }

  if (!bst->left) {
    return bst->right;
  }
  if (!bst->right) {
    return bst->left;
  }
  auto min = bst->left.get();
  while (min->right) {
    min = min->right.get();
  }
  return MkNode(min->key, min->value, Delete(bst->left, min->key), bst->right);
}
#endif

template <class KeyT, class ValueT>
std::optional<ValueT> Find(const BST<KeyT, ValueT>& bst, const KeyT& key) {
  if (!bst) {
    return std::nullopt;
  }
  if (key < bst->key) {
    return Find(bst->left, key);
  }
  if (key > bst->key) {
    return Find(bst->right, key);
  }
  return bst->value;
}

template <class KeyT, class ValueT>
std::list<std::pair<KeyT, ValueT>> ToList(const BST<KeyT, ValueT>& bst) {
  if (!bst) {
    return {};
  }

  auto left = ToList(bst->left);
  auto right = ToList(bst->right);
  left.push_back({bst->key, bst->value});
  left.splice(left.end(), std::move(right));
  return left;
}

template <class KeyT, class ValueT, std::ranges::input_range RngT>
  requires(
      std::same_as<std::ranges::range_value_t<RngT>, std::pair<KeyT, ValueT>>)
BST<KeyT, ValueT> FromRange(RngT&& list) {
  auto bst = EmptyBST<KeyT, ValueT>;
  auto it = std::ranges::begin(list);
  while (it != std::ranges::end(list)) {
    bst = Insert(bst, it->first, it->second);
    ++it;
  }
  return bst;
}

template <class KeyT, class ValueT>
std::optional<std::pair<KeyT, ValueT>> Max(const BST<KeyT, ValueT>& bst) {
  if (!bst) {
    return std::nullopt;
  }
  auto n = bst.get();
  while (n->right) {
    n = n->right.get();
  }
  return std::pair{n->key, n->value};
}

template <class KeyT, class ValueT>
std::optional<std::pair<KeyT, ValueT>> Min(const BST<KeyT, ValueT>& bst) {
  if (!bst) {
    return std::nullopt;
  }
  auto n = bst.get();
  while (n->left) {
    n = n->left.get();
  }
  return std::pair{n->key, n->value};
}

template <class KeyT, class ValueT>
BST<KeyT, ValueT> FromSortedIt(
    const typename std::vector<std::pair<KeyT, ValueT>>::const_iterator it,
    const typename std::vector<std::pair<KeyT, ValueT>>::const_iterator end) {
  if (it == end) {
    return EmptyBST<KeyT, ValueT>;
  }
  const auto mid = std ::next(it, std::distance(it, end) / 2);
  return MkNode(mid->first, mid->second, FromSortedIt<KeyT, ValueT>(it, mid),
                FromSortedIt<KeyT, ValueT>(std::next(mid), end));
}

template <class KeyT, class ValueT>
BST<KeyT, ValueT> FromSorted(const std::vector<std::pair<KeyT, ValueT>>& v) {
  return FromSortedIt<KeyT, ValueT>(v.cbegin(), v.cend());
}

#if defined(BST_UNION_BUG) && BST_UNION_BUG == 1
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Union(const BST<KeyT, ValueT>& lhs,
                        const BST<KeyT, ValueT>& rhs) {
  if (!lhs) {
    return rhs;
  }
  if (!rhs) {
    return lhs;
  }
  return MkNode(
      lhs->key, lhs->value, lhs->left,
      MkNode(rhs->key, rhs->value, Union(lhs->right, rhs->left), rhs->right));
}
#elif defined(BST_UNION_BUG) && BST_UNION_BUG == 2
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Union(const BST<KeyT, ValueT>& lhs,
                        const BST<KeyT, ValueT>& rhs) {
  if (!lhs) {
    return rhs;
  }
  if (!rhs) {
    return lhs;
  }
  if (lhs->key == rhs->key) {
    return MkNode(lhs->key, lhs->value, Union(lhs->left, rhs->left),
                  Union(lhs->right, rhs->right));
  }
  if (lhs->key < rhs->key) {
    return MkNode(
        lhs->key, lhs->value, lhs->left,
        MkNode(rhs->key, rhs->value, Union(lhs->right, rhs->left), rhs->right));
  }
  return Union(rhs, lhs);
}
#elif defined(BST_UNION_BUG) && BST_UNION_BUG == 3
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Below(KeyT k, BST<KeyT, ValueT> bst) {
  if (!bst) {
    return EmptyBST<KeyT, ValueT>;
  }
  if (k <= bst->key) {
    return Below(k, bst->left);
  }
  return MkNode(bst->key, bst->value, bst->left, Below(k, bst->right));
}

template <class KeyT, class ValueT>
BST<KeyT, ValueT> Above(KeyT k, BST<KeyT, ValueT> bst) {
  if (!bst) {
    return EmptyBST<KeyT, ValueT>;
  }
  if (k >= bst->key) {
    return Above(k, bst->right);
  }
  return MkNode(bst->key, bst->value, Above(k, bst->left), bst->right);
}

template <class KeyT, class ValueT>
BST<KeyT, ValueT> Union(const BST<KeyT, ValueT>& lhs,
                        const BST<KeyT, ValueT>& rhs) {
  if (!lhs) {
    return rhs;
  }
  if (!rhs) {
    return lhs;
  }
  if (lhs->key == rhs->key) {
    return MkNode(lhs->key, lhs->value, Union(lhs->left, rhs->left),
                  Union(lhs->right, rhs->right));
  }
  if (lhs->key < rhs->key) {
    return MkNode(
        lhs->key, lhs->value, lhs->left,
        MkNode(rhs->key, rhs->value,
               Union(lhs->left, Below(lhs->key, rhs->left)),
               Union(rhs, MkNode(rhs->key, rhs->value,
                                 Above(lhs->key, rhs->left), rhs->right))));
  }
  return Union(rhs, lhs);
}
#else
template <class KeyT, class ValueT>
BST<KeyT, ValueT> Union(const BST<KeyT, ValueT>& lhs,
                        const BST<KeyT, ValueT>& rhs) {
  auto l1 = ToList(lhs);
  auto l2 = ToList(rhs);
  auto iIt = l1.begin();
  for (const auto& [key, value] : l2) {
    while (iIt != l1.end() && iIt->first < key) {
      ++iIt;
    }
    if (iIt == l1.end() || iIt->first != key) {
      l1.insert(iIt, {key, value});
    }
  }
  auto v = std::vector<std::pair<KeyT, ValueT>>(l1.begin(), l1.end());
  auto rIt = std::unique(v.begin(), v.end(), [](const auto& a, const auto& b) {
    return a.first == b.first;
  });
  v.erase(rIt, v.end());
  return FromSorted(v);
}
#endif

template <class KeyT, class ValueT>
std::list<KeyT> Keys(const BST<KeyT, ValueT>& bst) {
  if (!bst) {
    return {};
  }

  auto left = Keys(bst->left);
  auto right = Keys(bst->right);
  left.push_back(bst->key);
  left.splice(left.end(), std::move(right));
  return left;
}
