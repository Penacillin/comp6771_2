#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <vector>
#include <utility>

EuclideanVector::EuclideanVector(int dimensions)
    : size_(dimensions) {
  magnitudes_ = std::make_unique<double[]>(dimensions);
  for (int i = 0; i < size_; i++) {
    magnitudes_[i] = 0.0;
  }
}


EuclideanVector::EuclideanVector(int dimensions, double magnitude)
    : size_(dimensions) {
  magnitudes_ = std::make_unique<double[]>(dimensions);
  for (int i = 0; i < size_; i++) {
    magnitudes_[i] = magnitude;
  }
}

EuclideanVector::EuclideanVector(std::vector<double>::const_iterator start,
                                 std::vector<double>::const_iterator end)
    : size_(end-start) {
  magnitudes_ = std::make_unique<double[]>(size_);
  size_t i = 0;
  for (auto iter = start; iter != end; iter++, i++) {
    magnitudes_[i] = *iter;
  }
}

EuclideanVector::EuclideanVector(const EuclideanVector& rhs)
    : size_(rhs.size_) {
  magnitudes_ = std::make_unique<double[]>(size_);
  std::copy(rhs.magnitudes_.get(), rhs.magnitudes_.get() + rhs.size_, magnitudes_.get());
}

EuclideanVector::EuclideanVector(EuclideanVector&& o) noexcept
    : magnitudes_{std::move(o.magnitudes_)},
      size_{o.size_} {
  o.size_ = 0;
}

int EuclideanVector::GetNumDimensions() noexcept {
  return size_;
}

EuclideanVector& EuclideanVector::operator=(EuclideanVector&& o) noexcept {
  magnitudes_ = std::move(o.magnitudes_);
  size_ = o.size_;
  o.size_ = 0;

  return *this;
}
