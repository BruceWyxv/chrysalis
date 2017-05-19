#include "gtest/gtest.h"

#include "FunctionalSeriesLegendre.h"
#include "FunctionalExpansionSolidCartesianLegendre.h"

const double tol = 1e-13;

using namespace libMesh;

TEST(FunctionalExpansionsTest, legendreConstructor)
{
  const unsigned int order = 5;
  FunctionalSeriesLegendre legendre(order);
  EXPECT_EQ(legendre.getOrder(), order);
}

TEST(FunctionalExpansionsTest, legendreSeriesEvaluation)
{
  const unsigned int order = 15;
  Real location = -0.90922108754014;
  std::array<Real, order> truth = {{ 0.50000000000000,
                                             -1.36383163131021,
                                              1.85006119760378,
                                             -1.80341832197563,
                                              1.19175581122701,
                                             -0.11669847057321,
                                             -1.20462734483853,
                                              2.48341349094950,
                                             -3.41981864606651,
                                              3.76808851494207,
                                             -3.39261995754146,
                                              2.30300489952095,
                                             -0.66011244776270,
                                             -1.24901920248131,
                                              3.06342136027001}};
  FunctionalSeriesLegendre legendre(order);

  auto& answer = legendre.evaluateOrthonormal({{location}});
  EXPECT_NEAR(legendre.getLocation(), location, tol);
  for (std::size_t i = 0; i < order; ++i)
    EXPECT_NEAR(answer[i], truth[i], tol);
}

TEST(FunctionalExpansionsTest, solidCartesianLegendreConstructor)
{
  const unsigned int order_one = 19;
  const std::array<unsigned int, 2> orders_two = {{13, 15}};
  const std::array<unsigned int, 3> orders_three = {{14, 21, 22}};

  FunctionalExpansionSolidCartesianLegendre legendreOne(order_one);
  FunctionalExpansionSolidCartesianLegendre legendreTwo(orders_two);
  FunctionalExpansionSolidCartesianLegendre legendreThree(orders_three);

  EXPECT_EQ(legendreOne.getNumberOfCoefficients(), order_one + 1);
  EXPECT_EQ(legendreTwo.getNumberOfCoefficients(), (orders_two[0] + 1) * (orders_two[1] + 1));
  EXPECT_EQ(legendreThree.getNumberOfCoefficients(), (orders_three[0] + 1) * (orders_three[1] + 1) * (orders_three[2] + 1));
}

TEST(FunctionalExpansionsTest, solidCartesianLegendreEvaluation1D)
{
  const unsigned int number_of_locations = 3;
  const unsigned int order = 19;
  std::vector<const Point *> locations;
  std::array<Real, number_of_locations> truths;
  locations.push_back(new Point(-0.91053106631643));
  truths[0] =  0.49379010080979;
  locations.push_back(new Point( 0.54546250195497));
  truths[1] =  1.09239546629484;
  locations.push_back(new Point( 0.87815200624901));
  truths[2] =  2.17537116324191;

  FunctionalExpansionSolidCartesianLegendre legendre(order);
  legendre.setDimensionality(1);

  auto answers = legendre.sample(locations);
  for (std::size_t i = 0; i < number_of_locations; ++i)
  {
    EXPECT_NEAR(answers[i], truths[i], tol);
    delete locations[i];
  }
}

TEST(FunctionalExpansionsTest, solidCartesianLegendreEvaluation2D)
{
  const unsigned int number_of_locations = 3;
  const std::array<unsigned int, 2> orders = {{13, 15}};
  std::vector<const Point *> locations;
  std::array<Real, number_of_locations> truths;
  locations.push_back(new Point(-0.62565861438665,
                                 0.35778939787196));
  truths[0] =  0.37765836859915;
  locations.push_back(new Point(-0.76095070771917,
                                -0.44189666651278));
  truths[1] =  0.41262404264148;
  locations.push_back(new Point( 0.37288682040427,
                                 0.54937715621170));
  truths[2] =  1.04443001278784;

  FunctionalExpansionSolidCartesianLegendre legendre(orders);
  legendre.setDimensionality(2);

  auto answers = legendre.sample(locations);
  for (std::size_t i = 0; i < number_of_locations; ++i)
  {
    EXPECT_NEAR(answers[i], truths[i], tol);
    delete locations[i];
  }
}

TEST(FunctionalExpansionsTest, solidCartesianLegendreEvaluation3D)
{
  const unsigned int number_of_locations = 3;
  const std::array<unsigned int, 3> orders = {{14, 21, 22}};
  std::vector<const Point *> locations;
  std::array<Real, number_of_locations> truths;
  locations.push_back(new Point(-0.14854612627465,
                                 0.60364074055275,
                                 0.76978431165674));
  truths[0] =  1.32257143058688;
  locations.push_back(new Point( 0.93801805187856,
                                 0.74175118177279,
                                 0.74211345600994));
  truths[1] =  3.68047786931852;
  locations.push_back(new Point( 0.35423736896098,
                                -0.83921049062126,
                                -0.02231845586669));
  truths[2] =  0.17515811557416;

  FunctionalExpansionSolidCartesianLegendre legendre(orders);
  legendre.setDimensionality(3);

  auto answers = legendre.sample(locations);
  for (std::size_t i = 0; i < number_of_locations; ++i)
  {
    EXPECT_NEAR(answers[i], truths[i], tol);
    delete locations[i];
  }
}

TEST(FunctionalExpansionsTest, functionalExpansionInterface)
{
  const std::array<unsigned int, 3> orders = {{4, 5, 3}};
  const Point location(-0.38541903411291,
                        0.61369802505416,
                       -0.04539307255549);
  const Real truth = 0.26458908225718;

  FunctionalExpansionSolidCartesianLegendre legendre(orders);
  FunctionalExpansionInterface & interface = (FunctionalExpansionInterface &)legendre;
  interface.setDimensionality(3);

  // Check with default coefficients = 1
  EXPECT_NEAR(interface.sample(location), truth, tol);

  // Set all coefficients to 2 using the reference version of setCoefficients
  std::vector<Real> newCoefficients(interface.getNumberOfCoefficients(), 2);
  interface.setCoefficients(newCoefficients);
  EXPECT_NEAR(interface.sample(location), truth * 2, tol);

  // Set all coefficients to 0 using the std::move version of setCoefficients
  interface.setCoefficients(std::vector<Real>(interface.getNumberOfCoefficients(), 0));
  EXPECT_EQ(interface.sample(location), 0);
}
