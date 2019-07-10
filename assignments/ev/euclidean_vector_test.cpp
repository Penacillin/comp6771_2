/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include <vector>

#include "assignments/ev/euclidean_vector.h"

#include "catch.h"

// TODO(students): Fill this in.
#define CATCH_CONFIG_MAIN

SCENARIO("Euclidean vectors can be created by various initialiization methods and they'll have"
          " the correct magnitudes and dimensions.") {
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
          REQUIRE(euclideanVector.at(i) == 0);
        }
      }
    }
  }

  GIVEN("4 magnitudes (2,5.4,7,9.8) that the vector should have") {
    std::vector<double> vals{2, 5.4, 7, 9.8};

    WHEN("An euclidean vector constructed using these values will have these magnitudes"
        " and a dimensions size of 4") {
      EuclideanVector euclideanVector = EuclideanVector(vals.begin(), vals.end());

      THEN("The magnitudes at each dimension will match up"
            "with the expected values (2,5.4,7,9.8)") {
        for (size_t i = 0; i < vals.size(); ++i) {
          REQUIRE(euclideanVector.at(i) == vals.at(i));
        }
      }
    }
  }
}

