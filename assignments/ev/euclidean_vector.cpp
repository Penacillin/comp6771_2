#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <utility>

EuclideanVector::EuclideanVector(EuclideanVector&& o) noexcept :
magnitudes_{std::move(o.magnitudes_)}, size_{o.size_} {
  o.size_ = 0;
}
