/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

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

SCENARIO("EuclideanVectors can be modified by changing specific dimensions or"
          " by applying operations to them") {
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

SCENARIO("We can apply operations to existing EuclideanVectors to create new results") {
  GIVEN("An EuclideanVector with values (2, 3, 2.5)") {
    std::vector<double> vals = {2, 3, 2.5};
    EuclideanVector euclideanVector = EuclideanVector(vals.begin(), vals.end());

    WHEN("We multiply the euclideanVector by the value (2.5)") {
      EuclideanVector res = euclideanVector * 2.5;
      THEN("the new EuclideanVector will have magnitudes (5, 7.5, 6.25)") {
        REQUIRE(res.GetNumDimensions() == 3);

        REQUIRE(dblEqual(res[0], 5));
        REQUIRE(dblEqual(res[1], 7.5));
        REQUIRE(dblEqual(res[2], 6.25));
      }
    }

    WHEN("We multiply the value (2.5) by the EuclideanVector") {
      EuclideanVector res = 2.5*euclideanVector;

      THEN("the new EuclideanVector will have magnitudes (5, 7.5, 6.25)") {
        REQUIRE(res.GetNumDimensions() == 3);

        REQUIRE(dblEqual(res[0], 5));
        REQUIRE(dblEqual(res[1], 7.5));
        REQUIRE(dblEqual(res[2], 6.25));
      }
    }

    WHEN("We divide the euclideanVector by 2") {
      EuclideanVector res = euclideanVector / 2;

      THEN("the new EuclideanVector will have magnitudes (1, 1.5, 1.25)") {
        REQUIRE(res.GetNumDimensions() == 3);

        REQUIRE(dblEqual(res[0], 1));
        REQUIRE(dblEqual(res[1], 1.5));
        REQUIRE(dblEqual(res[2], 1.25));
      }
    }

    WHEN("We get the Euclidean Normal of the EuclideanVector") {
      double res = euclideanVector.GetEuclideanNorm();
        // std::cout << res << std::endl;

      THEN("We will have Euclidean Normal sqrt(2^2 + 3^2 + 2.5^) = 4.38748219") {
        REQUIRE(dblEqual(res, 4.38748219));
      }
    }

    AND_GIVEN("Another vector with the same number of dimensions and values"
                "1, 2, 1.5)") {
      std::vector<double> vals = {1, 2, 1.5};

      EuclideanVector euclideanVector2 = EuclideanVector(vals.begin(), vals.end());

      WHEN("We add the two vectors together") {
        EuclideanVector res = euclideanVector + euclideanVector2;

        THEN("We will have a new vector which is a element wise sum (3, 5, 4)") {
          REQUIRE(dblEqual(res[0], 3));
          REQUIRE(dblEqual(res[1], 5));
          REQUIRE(dblEqual(res[2], 4));
        }
      }

      WHEN("We substract the second vector from the first one") {
        EuclideanVector res = euclideanVector - euclideanVector2;

        THEN("We will have a new vector which is a element wise substraction (1,1,1)") {
          REQUIRE(dblEqual(res[0], 1));
          REQUIRE(dblEqual(res[1], 1));
          REQUIRE(dblEqual(res[2], 1));
        }
      }

      WHEN("We call the CreateUnitVector function the first one") {
        EuclideanVector res = euclideanVector.CreateUnitVector();
        std::cout << std::setprecision(16) << res << std::endl;
        THEN("We will have a new vector which is a unit vector of the first one(1,1,1)") {
          REQUIRE(dblEqual(res.GetEuclideanNorm(), 1));
          REQUIRE(dblEqual(res[0], 0.4558423058385518));
          REQUIRE(dblEqual(res[1], 0.6837634587578276));
          REQUIRE(dblEqual(res[2], 0.5698028822981898));
        }
      }

      WHEN("We multiply (dot product) the two vectors") {
        double res = euclideanVector * euclideanVector2;
          // std::cout << res << std::endl;

        THEN("We will have the dot product of the two vectors") {
          REQUIRE(dblEqual(res, 11.75));
        }
      }

      
    }
  }
}

SCENARIO("We can compare two vectors for equality and inequality") {
  GIVEN("three vectors, two which are the same and two which are different") {
    std::vector<double> vals = {1, 2, 1.5};
    std::vector<double> vals2 = {1, 3, 1.5};

    EuclideanVector euclideanVector1 = EuclideanVector(vals.begin(), vals.end());
    EuclideanVector euclideanVector2 = EuclideanVector(vals.begin(), vals.end());
    EuclideanVector euclideanVector3 = EuclideanVector(vals2.begin(), vals2.end());

    WHEN("We test for equality between the two equal vectors") {
      bool res = euclideanVector1 == euclideanVector2;

      THEN("The result will be true") {
        REQUIRE(res == true);
      }
    }

    WHEN("We test for equality between the two unequal vectors") {
      bool res = euclideanVector1 == euclideanVector3;

      THEN("The result will be false") {
        REQUIRE(res == false);
      }
    }

    WHEN("We test for inequality between the two unequal vectors") {
      bool res = euclideanVector1 != euclideanVector3;

      THEN("The result will be true") {
        REQUIRE(res == true);
      }
    }

    WHEN("We test for inequality between the two equal vectors") {
      bool res = euclideanVector1 != euclideanVector2;

      THEN("The result will be false") {
        REQUIRE(res == false);
      }
    }
  }
}

SCENARIO("EuclideanVectors can be printed") {
  GIVEN("A vector with 3 magnitudes") {
    std::vector<double> vals = {1, 2, 1.5};

    EuclideanVector euclideanVector = EuclideanVector(vals.begin(), vals.end());

    WHEN("The EuclideanVector is printed to a output stream") {
      std::ostringstream os;
      os << euclideanVector;

      THEN("The output string should be '[1 2 1.5]'") {
        REQUIRE(os.str() == "[1 2 1.5]");
      }
    }
  }
  GIVEN("A vector with 2 magnitudes") {
    std::vector<double> vals = {1, 2};

    EuclideanVector euclideanVector = EuclideanVector(vals.begin(), vals.end());

    WHEN("The EuclideanVector is printed to a output stream") {
      std::ostringstream os;
      os << euclideanVector;

      THEN("The output string should be '[1 2]'") {
        REQUIRE(os.str() == "[1 2]");
      }
    }
  }
  GIVEN("A vector with 1 magnitude") {
    std::vector<double> vals = {1};

    EuclideanVector euclideanVector = EuclideanVector(vals.begin(), vals.end());

    WHEN("The EuclideanVector is printed to a output stream") {
      std::ostringstream os;
      os << euclideanVector;

      THEN("The output string should be '[1]'") {
        REQUIRE(os.str() == "[1]");
      }
    }
  }
  GIVEN("A vector with 0 magnitudes") {
    std::vector<double> vals = {};

    EuclideanVector euclideanVector = EuclideanVector(vals.begin(), vals.end());

    WHEN("The EuclideanVector is printed to a output stream") {
      std::ostringstream os;
      os << euclideanVector;

      THEN("The output string should be '[]'") {
        REQUIRE(os.str() == "[]");
      }
    }
  }
}

SCENARIO("Given invalid arguments, an EuclideanVector will throw the "
          " correct EuclideanVectorError") {
  GIVEN("An empty EuclideanVector") {
    EuclideanVector euclideanVector = EuclideanVector(0);

    WHEN("We try to create a Unit Vector off this dimensionless Vector") {
      REQUIRE_THROWS_WITH(euclideanVector.CreateUnitVector(), "EuclideanVector with no dimensions"
          " does not have a unit vector");
    }

    WHEN("We try to Get Euclidean Norm off this dimensionless Vector") {
      REQUIRE_THROWS_WITH(euclideanVector.GetEuclideanNorm(), "EuclideanVector with no dimensions"
          " does not have a unit vector");
    }
  }

  GIVEN("An EuclideanVector with values (2, 3, 2.5)") {
    std::vector<double> vals = {2, 3, 2.5};
    EuclideanVector euclideanVector = EuclideanVector(vals.begin(), vals.end());

    WHEN("We try to get the index at -1") {
      REQUIRE_THROWS_WITH(euclideanVector.at(-1), "Index -1 is not valid for this"
        " EuclideanVector object");
    }

    WHEN("We try to get the index at 3") {
      REQUIRE_THROWS_WITH(euclideanVector.at(3), "Index 3 is not valid for this"
        " EuclideanVector object");
    }

    WHEN("We try to divide the Vector by 0") {
      REQUIRE_THROWS_WITH(euclideanVector /= 0, "Invalid vector division by 0");
    }

    WHEN("We try to divide the Vector by 0") {
      REQUIRE_THROWS_WITH(euclideanVector / 0, "Invalid vector division by 0");
    }

    AND_GIVEN("Another EuclideanVector with a different number of dimensions") {
      std::vector<double> vals = {2, 3};
      EuclideanVector euclideanVector2 = EuclideanVector(vals.begin(), vals.end());

      REQUIRE(euclideanVector.GetNumDimensions() != euclideanVector2.GetNumDimensions());

      WHEN("We try to add the second Vector with 2 dimensions to the Vector with 3 dimensions") {
        REQUIRE_THROWS_WITH(euclideanVector += euclideanVector2, "Dimensions of LHS(3) and RHS(2)"
          " do not match");
      }

      WHEN("We try to add together the two Vectors of different dimensions to create a third"
            " Vector") {
        REQUIRE_THROWS_WITH(euclideanVector + euclideanVector2,
          "Dimensions of LHS(3) and RHS(2) do not match");
      }

      WHEN("We try to subtract the second Vector with 2 dimensions"
          " from the Vector with 3 dimensions") {
        REQUIRE_THROWS_WITH(euclideanVector -= euclideanVector2, "Dimensions of LHS(3) and RHS(2)"
          " do not match");
      }

      WHEN("We try to subtract the second Vector with 2 dimensions"
          " from the Vector with 3 dimensions to create a new vector") {
        REQUIRE_THROWS_WITH(euclideanVector - euclideanVector2, "Dimensions of LHS(3) and RHS(2)"
          " do not match");
      }

      WHEN("We try to multiply the second Vector with 2 dimensions"
          " from the Vector with 3 dimensions to get the dot product") {
        REQUIRE_THROWS_WITH(euclideanVector * euclideanVector2, "Dimensions of LHS(3) and RHS(2)"
          " do not match");
      }
    }
  }
}
