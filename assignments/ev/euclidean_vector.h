#ifndef ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
#define ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_

#include <exception>
#include <string>
#include <memory>
#include <vector>
#include <list>
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
  explicit EuclideanVector(int dimensions = 1);
  EuclideanVector(int dimensions, double magnitude);
  EuclideanVector(std::vector<double>::const_iterator start,
                  std::vector<double>::const_iterator end);
  EuclideanVector(const EuclideanVector&);
  EuclideanVector(EuclideanVector&&) noexcept;

  double at(int) const;
  double& at(int);
  EuclideanVector CreateUnitVector() const;
  double GetEuclideanNorm() const;
  int GetNumDimensions() const noexcept;


  EuclideanVector& operator=(const EuclideanVector& o) noexcept;
  EuclideanVector& operator=(EuclideanVector&& o) noexcept;
  double& operator[](int index) noexcept;
  const double& operator[](int index) const noexcept;
  EuclideanVector& operator+=(const EuclideanVector&);
  EuclideanVector& operator*=(const double) noexcept;
  EuclideanVector& operator-=(const EuclideanVector&);
  EuclideanVector& operator/=(const double);
  explicit operator std::vector<double>() noexcept;
  explicit operator std::list<double>() noexcept;

  friend bool operator==(const EuclideanVector& lhs, const EuclideanVector& rhs) noexcept;
  friend bool operator!=(const EuclideanVector& lhs, const EuclideanVector& rhs) noexcept;
  friend EuclideanVector operator+(const EuclideanVector& lhs, const EuclideanVector& rhs);
  friend EuclideanVector operator-(const EuclideanVector& lhs, const EuclideanVector& rhs);
  friend double operator*(const EuclideanVector& lhs, const EuclideanVector& rhs);
  friend EuclideanVector operator/(const EuclideanVector& lhs, const EuclideanVector& rhs);
  friend EuclideanVector operator*(const EuclideanVector& lhs, double) noexcept;
  friend EuclideanVector operator*(double, const EuclideanVector& rhs) noexcept;
  friend EuclideanVector operator/(const EuclideanVector& lhs, double);
  friend std::ostream& operator<<(std::ostream& os, const EuclideanVector& v) noexcept;

 private:
  std::unique_ptr<double[]> magnitudes_;
  int size_;
};

#endif  // ASSIGNMENTS_EV_EUCLIDEAN_VECTOR_H_
