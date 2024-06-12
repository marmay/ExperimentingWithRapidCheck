#include <rapidcheck.h>

#include <vector>

template <class T>
void Reverse(std::vector<T>& v) {
  std::reverse(v.begin(), v.end());
}

void demo02() {
  rc::check("Reversing a vector twice gives the original vector.",
            [](std::vector<int> v) {
              const std::vector<int> original = v;
              Reverse(v);
              Reverse(v);
              RC_ASSERT(v == original);
            });
}
