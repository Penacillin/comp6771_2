#ifndef ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
#define ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_

#include <exception>
#include <string>
#include <memory>
#include <vector>
#include <utility>


class EuclideanVectorError : public std::exception {
 public:
  explicit EuclideanVectorError(const std::string& what) : what_(what) {}
  const char* what() const noexcept { return what_.c_str(); }
 private:
  std::string what_;
};

class EuclideanVector {
 public:
  explicit EuclideanVector(int i);
  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& v);
  explicit EuclideanVector(int dimensions = 0);
  EuclideanVector(int dimensions, double magnitude);
  EuclideanVector(std::vector<double>::const_iterator start,
                  std::vector<double>::const_iterator end);
  EuclideanVector(const EuclideanVector&);
  EuclideanVector(EuclideanVector&&);

  EuclideanVector operator=(EuclideanVector&& o) noexcept {
    magnitudes_ = std::move(o.magnitudes_);
    size_ = o.size_;
    o.size_ = 0;
  }
 private:
  std::unique_ptr<double[]> magnitudes_;
  int size_;
};

#endif  // ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
