#include "assignments/ev/euclidean_vector.h"

#include <algorithm>  // Look at these - they are helpful https://en.cppreference.com/w/cpp/algorithm
#include <cassert>
#include <cmath>
#include <vector>
#include <ostream>
#include <utility>
#include <iostream>
#include <sstream>

EuclideanVector::EuclideanVector(int dimensions)
    : size_(dimensions) {
  std::cout << "Vector created by dimensions" << std::endl;
  magnitudes_ = std::make_unique<double[]>(dimensions);
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] = 0.0;
  }
}

EuclideanVector::EuclideanVector(int dimensions, double magnitude)
    : size_(dimensions) {
  magnitudes_ = std::make_unique<double[]>(dimensions);
  for (int i = 0; i < size_; ++i) {
    magnitudes_[i] = magnitude;
  }
}

EuclideanVector::EuclideanVector(const std::vector<double>::const_iterator start,
                                 const std::vector<double>::const_iterator end)
    : size_(end-start) {
  magnitudes_ = std::make_unique<double[]>(size_);
  int i = 0;
  for (auto iter = start; iter != end; iter++, ++i) {
    magnitudes_[i] = *iter;
  }
}

EuclideanVector::EuclideanVector(const EuclideanVector& rhs)
    : size_(rhs.size_) {
  std::cout << "Called the copy constructor" << std::endl;
  magnitudes_ = std::make_unique<double[]>(size_);
  std::copy(rhs.magnitudes_.get(), rhs.magnitudes_.get() + rhs.size_, magnitudes_.get());
}

EuclideanVector::EuclideanVector(EuclideanVector&& o) noexcept
    : magnitudes_{std::move(o.magnitudes_)},
      size_{o.size_} {
  std::cout << "Called the move constructor" << std::endl;
  o.size_ = 0;
}

EuclideanVector EuclideanVector::CreateUnitVector() const {
  if (GetNumDimensions() == 0)
    throw EuclideanVectorError("EuclideanVector with no dimensions does not have a unit vector");
  EuclideanVector unitVector = *this;
  double vectorNorm = GetEuclideanNorm();
  for (int i = 0; i < unitVector.size_; ++i) {
    unitVector.magnitudes_[i] /= vectorNorm;
  }
  return unitVector;
}

double EuclideanVector::GetEuclideanNorm() const {
  if (GetNumDimensions() == 0)
    throw EuclideanVectorError("EuclideanVector with no dimensions does not have a unit vector");
  double result = 0;
  for (int i = 0; i < size_; ++i) {
    result += magnitudes_[i] * magnitudes_[i];
  }
  return std::sqrt(result);
}

int EuclideanVector::GetNumDimensions() const noexcept {
  return size_;
}

double EuclideanVector::at(int index) const {
  if (index < 0 || index >= size_) {
    std::ostringstream oss;
    oss << "Index " << index << " is not valid for this EuclideanVector object";
    throw EuclideanVectorError(oss.str());
  }
  return magnitudes_[index];
}

double& EuclideanVector::at(int index) {
  if (index < 0 || index >= size_) {
    std::ostringstream oss;
    oss << "Index " << index << " is not valid for this EuclideanVector object";
    throw EuclideanVectorError(oss.str());
  }
  return magnitudes_[index];
}

EuclideanVector& EuclideanVector::operator=(const EuclideanVector& o) noexcept {
  std::cout << "calling copy operator " << std::endl;
  size_ = o.size_;
  std::copy(o.magnitudes_.get(), o.magnitudes_.get() + o.size_, magnitudes_.get());
  return *this;
}

EuclideanVector& EuclideanVector::operator=(EuclideanVector&& o) noexcept {
  std::cout << "calling move operator " << std::endl;
  magnitudes_ = std::move(o.magnitudes_);
  size_ = o.size_;
  o.size_ = 0;
  return *this;
}

double& EuclideanVector::operator[](int index) noexcept {
  assert(index >= 0 && index < size_);
  return magnitudes_[index];
}

const double& EuclideanVector::operator[](int index) const noexcept {
  assert(index >= 0 && index < size_);
  return magnitudes_[index];
}

EuclideanVector& EuclideanVector::operator+=(const EuclideanVector& o) {
  if (this->size_ != o.size_) {
    std::ostringstream oss;
    oss << "Dimensions of LHS(" << this->size_ << ") and RHS(" << o.size_
        << ") do not match";
    throw EuclideanVectorError(oss.str());
  }
  for (int i = 0; i < size_; ++i)
    magnitudes_[i] += o.magnitudes_[i];
  return *this;
}

EuclideanVector& EuclideanVector::operator-=(const EuclideanVector& o) {
  if (this->size_ != o.size_) {
    std::ostringstream oss;
    oss << "Dimensions of LHS(" << this->size_ << ") and RHS(" << o.size_
        << ") do not match";
    throw EuclideanVectorError(oss.str());
  }
  for (int i = 0; i < size_; ++i)
    magnitudes_[i] -= o.magnitudes_[i];
  return *this;
}

EuclideanVector& EuclideanVector::operator*=(const double val) noexcept {
  std::cout << "Calling operator*=" << std::endl;
  for (int i = 0; i < size_; ++i)
    magnitudes_[i] *= val;
  return *this;
}

EuclideanVector& EuclideanVector::operator/=(const double val) {
  if (val == 0)
    throw EuclideanVectorError("Invalid vector division by 0");
  for (int i = 0; i < size_; ++i)
    magnitudes_[i] /= val;
  return *this;
}

EuclideanVector::operator std::vector<double>() noexcept {
  return std::vector<double>(magnitudes_.get(), magnitudes_.get() + size_);
}

EuclideanVector::operator std::list<double>() noexcept {
  return std::list<double>(magnitudes_.get(), magnitudes_.get() + size_);
}

bool operator==(const EuclideanVector& lhs, const EuclideanVector& rhs) noexcept {
  if (lhs.size_ != rhs.size_) return false;
  for (int i = 0; i < lhs.size_; ++i) {
    if (lhs.magnitudes_[i] != rhs.magnitudes_[i]) return false;
  }
  return true;
}

bool operator!=(const EuclideanVector& lhs, const EuclideanVector& rhs) noexcept {
  if (lhs.size_ != rhs.size_) return true;
  for (int i = 0; i < lhs.size_; ++i) {
    if (lhs.magnitudes_[i] != rhs.magnitudes_[i]) return true;
  }
  return false;
}

EuclideanVector operator+(const EuclideanVector& lhs, const EuclideanVector& rhs) {
  if (lhs.size_ != rhs.size_) {
    std::ostringstream oss;
    oss << "Dimensions of LHS(" << lhs.size_ << ") and RHS(" << rhs.size_
        << ") do not match";
    throw EuclideanVectorError(oss.str());
  }
  EuclideanVector res{lhs.size_};
  for (int i = 0; i < lhs.size_; ++i) {
    res.magnitudes_[i] = lhs.magnitudes_[i] + rhs.magnitudes_[i];
  }
  return res;
}

EuclideanVector operator-(const EuclideanVector& lhs, const EuclideanVector& rhs) {
  if (lhs.size_ != rhs.size_) {
    std::ostringstream oss;
    oss << "Dimensions of LHS(" << lhs.size_ << ") and RHS(" << rhs.size_
        << ") do not match";
    throw EuclideanVectorError(oss.str());
  }
  EuclideanVector res{lhs.size_};
  for (int i = 0; i < lhs.size_; ++i) {
    res.magnitudes_[i] = lhs.magnitudes_[i] - rhs.magnitudes_[i];
  }
  return res;
}

double operator*(const EuclideanVector& lhs, const EuclideanVector& rhs) {
  if (lhs.size_ != rhs.size_) {
    std::ostringstream oss;
    oss << "Dimensions of LHS(" << lhs.size_ << ") and RHS(" << rhs.size_
        << ") do not match";
    throw EuclideanVectorError(oss.str());
  }
  double res = 0;
  for (int i = 0; i < lhs.size_; ++i) {
    res += lhs.magnitudes_[i] * rhs.magnitudes_[i];
  }
  return res;
}

EuclideanVector operator*(const EuclideanVector& lhs, double rhs) noexcept {
  std::cout << "Calling * operator " << std::endl;
  EuclideanVector res{lhs.size_};
  for (int i = 0; i < lhs.size_; ++i) {
    res.magnitudes_[i] = lhs.magnitudes_[i] * rhs;
  }
  return res;
}

EuclideanVector operator*(double lhs, const EuclideanVector& rhs) noexcept {
  std::cout << "Calling * operator " << std::endl;
  EuclideanVector res{rhs.size_};
  for (int i = 0; i < rhs.size_; ++i) {
    res.magnitudes_[i] = rhs.magnitudes_[i] * lhs;
  }
  return res;
}

EuclideanVector operator/(const EuclideanVector& lhs, double rhs) {
  if (rhs == 0)
    throw EuclideanVectorError("Invalid vector division by 0");
  EuclideanVector res{lhs.size_};
  for (int i = 0; i < lhs.size_; ++i) {
    res.magnitudes_[i] = lhs.magnitudes_[i] / rhs;
  }
  return res;
}

std::ostream& operator<<(std::ostream& os, const EuclideanVector& v) noexcept {
  os << "[";
  for (int i = 0; i < v.size_ - 1; ++i) {
    os << v.magnitudes_[i] << " ";
  }
  if (v.size_ > 0)
    os << v.magnitudes_[v.size_-1];
  os << "]";
  return os;
}
