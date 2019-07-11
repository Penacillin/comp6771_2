/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include <vector>
#include <iostream>

#include "assignments/ev/euclidean_vector.h"

#include "catch.h"

// TODO(students): Fill this in.
constexpr double EPS = 1e-8;

bool dblEqual(const double& x, const double& y) {
  if (std::abs(x - y) < EPS) return true;
  return false;
}

#define CATCH_CONFIG_MAIN

SCENARIO("Euclidean vectors can be created by various initialiization methods and they'll have"
          " the correct magnitudes and dimensions.", "[EuclideanVector][Construction]") {
  GIVEN("A number of dimensions (3) that the vector should have") {
    int dimension = 3;

    WHEN("an eucliden vector is constructed using this dimension as an arguments to its "
        "constructor") {
      EuclideanVector euclideanVector{dimension};

      THEN("The num of dimension of the vector is this (3)") {
        REQUIRE(euclideanVector.GetNumDimensions() == 3);
      }
      THEN("The magnitude of each each dimension is zero") {
        for (int i = 0; i < dimension; ++i) {
          REQUIRE(dblEqual(euclideanVector.at(i), 0));
        }
      }
    }
  }

  GIVEN("The 4 magnitudes (2,5.4,7,9.8) that the vector should have") {
    std::vector<double> vals{2, 5.4, 7, 9.8};

    WHEN("An euclidean vector is constructed using these values") {
      EuclideanVector euclideanVector = EuclideanVector(vals.begin(), vals.end());

      THEN("The magnitudes at each dimension will match up"
            "with the expected values (2,5.4,7,9.8) and the number of dimensions is 4") {
        for (size_t i = 0; i < vals.size(); ++i) {
          REQUIRE(dblEqual(euclideanVector.at(i), vals.at(i)));
        }

        REQUIRE(euclideanVector.GetNumDimensions() == vals.size());
      }
    }
  }

  GIVEN("the number of dimension (5) and a default value (4.2) for every magnitude") {
    int dimensions = 5;
    double defaultMagnitude = 4.2;

    WHEN("An EuclideanVector is created with 5 dimensions and 4.2 as the magnitude for each"
        " dimension") {
      EuclideanVector euclideanVector = EuclideanVector(dimensions, defaultMagnitude);

      THEN("The magnitude of each dimension will be 4.2 and the vector will have 5 dimensions") {
        REQUIRE(euclideanVector.GetNumDimensions() == dimensions);

        for (int i = 0; i < dimensions; ++i) {
          REQUIRE(dblEqual(euclideanVector.at(i), defaultMagnitude));
        }

        AND_WHEN("another EuclideanVector is created using the copy constructor") {
          EuclideanVector copiedVector = EuclideanVector(euclideanVector);

          THEN("Each dimension will also have a magnitude 4.2 and it'll also have 5 dimensions") {
            REQUIRE(copiedVector.GetNumDimensions() == dimensions);

            for (int i = 0; i < dimensions; ++i) {
              REQUIRE(dblEqual(copiedVector.at(i), defaultMagnitude));
            }
          }
        }

        AND_WHEN("another EuclideanVector is declared using move semantics") {
          EuclideanVector movedVector = std::move(euclideanVector);

          THEN("The new movedVector will have a magnitude of 4.2 of each of its 5 dimensions"
                " and the old vector will have a dimension size of 0") {
            REQUIRE(movedVector.GetNumDimensions() == dimensions);

            for (int i = 0; i < dimensions; ++i) {
              REQUIRE(dblEqual(movedVector.at(i), defaultMagnitude));
            }

            REQUIRE(euclideanVector.GetNumDimensions() == 0);
          }
        }
      }
    }
  }
}

SCENARIO("EuclideanVectors can be modified by changing specific dimensions or byapplying operations"
          " to") {
  GIVEN("A vector with values (1,2,2.5,5)") {
    std::vector<double> vals{1, 2, 2.5, 5};
    WHEN("An euclideanVector is constructed using these values") {
      EuclideanVector euclideanVector = EuclideanVector(vals.begin(), vals.end());

      THEN("Each dimension should have the expected magnitude that we initialized with") {
        for (size_t i = 0; i < vals.size(); ++i) {
          REQUIRE(dblEqual(euclideanVector[i], vals[i]));
        }

        AND_WHEN("We modify the first dimension to have a magnitude of 3 (using subscript)") {
          euclideanVector[0] = 3;

          THEN("The first dimension will have a magnitude of 3 and the other dimensions should"
                " be unmodified") {
            REQUIRE(euclideanVector[0] == 3);
            for (size_t i = 1; i < vals.size(); ++i) {
              REQUIRE(dblEqual(euclideanVector[i], vals[i]));
            }
          }
        }

        AND_WHEN("We modify the first dimension to have a magnitude of 3 (using at method)") {
          euclideanVector.at(0) = 3;

          THEN("The first dimension will have a magnitude of 3 and the other dimensions should"
                " be unmodified") {
            REQUIRE(euclideanVector[0] == 3);
            for (size_t i = 1; i < vals.size(); ++i) {
              REQUIRE(dblEqual(euclideanVector[i], vals[i]));
            }
          }
        }

        AND_WHEN("We modify the third dimension to have a magnitude of 3 (using at method)") {
          euclideanVector.at(2) = 3;

          THEN("The first dimension will have a magnitude of 3 and the other dimensions should"
                " be unmodified") {
            REQUIRE(dblEqual(euclideanVector[0], vals[0]));
            REQUIRE(dblEqual(euclideanVector[1], vals[1]));
            REQUIRE(dblEqual(euclideanVector[2], 3));
            REQUIRE(dblEqual(euclideanVector[3], vals[3]));
          }
        }

        AND_WHEN("We multiply the vector by a value of 3.5 using the *= operator") {
          euclideanVector *= 3.5;

          THEN("Each dimension of the vector will have been multiplied by 3.5") {
            REQUIRE(dblEqual(euclideanVector[0], vals[0] * 3.5));
            REQUIRE(dblEqual(euclideanVector[1], vals[1] * 3.5));
            REQUIRE(dblEqual(euclideanVector[2], vals[2] * 3.5));
            REQUIRE(dblEqual(euclideanVector[3], vals[3] * 3.5));
          }
        }

        AND_WHEN("We multiply the vector by a value of 3.5 using the * operator") {
          euclideanVector = euclideanVector * 3.5;

          THEN("Each dimension of the vector will have been multiplied by 3.5") {
            REQUIRE(dblEqual(euclideanVector[0], vals[0] * 3.5));
            REQUIRE(dblEqual(euclideanVector[1], vals[1] * 3.5));
            REQUIRE(dblEqual(euclideanVector[2], vals[2] * 3.5));
            REQUIRE(dblEqual(euclideanVector[3], vals[3] * 3.5));
          }
        }

        AND_WHEN("We divide the vector by a value of 2.5 using the /= operator") {
          euclideanVector /= 2.5;

          THEN("Each dimension of the vector will have been multiplied by 3.5") {
            REQUIRE(dblEqual(euclideanVector[0], vals[0] / 2.5));
            REQUIRE(dblEqual(euclideanVector[1], vals[1] / 2.5));
            REQUIRE(dblEqual(euclideanVector[2], vals[2] / 2.5));
            REQUIRE(dblEqual(euclideanVector[3], vals[3] / 2.5));
          }
        }

        AND_GIVEN("Another EuclideanVector with the same number of dimensions (4) and values: "
                  "(2.2, 3.3, 5, 7)") {
          std::vector<double> vals{2.2, 3.3, 5, 7};
          EuclideanVector newVector = EuclideanVector(vals.begin(), vals.end());
          WHEN("We add this EuclideanVector to the original") {
            euclideanVector += newVector;

            THEN("The original euclideanVector should have values of (1,2,2.5,5)+(2.2, 3.3, 5, 7)"
                " = (3.2, 5.3, 7.5, 12)") {
              REQUIRE(dblEqual(euclideanVector[0], 3.2));
              REQUIRE(dblEqual(euclideanVector[1], 5.3));
              REQUIRE(dblEqual(euclideanVector[2], 7.5));
              REQUIRE(dblEqual(euclideanVector[3], 12));
            }
          }

          WHEN("We substract this EuclideanVector from the original") {
            euclideanVector -= newVector;

            THEN("The original euclideanVector should have values of (1,2,2.5,5)-(2.2, 3.3, 5, 7)"
                " = (-1.2, -1.3, -2.5, -2)") {
              REQUIRE(dblEqual(euclideanVector[0], -1.2));
              REQUIRE(dblEqual(euclideanVector[1], -1.3));
              REQUIRE(dblEqual(euclideanVector[2], -2.5));
              REQUIRE(dblEqual(euclideanVector[3], -2));
            }
          }
        }
      }
    }
  }
}
