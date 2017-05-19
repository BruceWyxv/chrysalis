#ifndef FUNCTIONALEXPANSIONSOLIDCARTESIANLEGENDRE_H
#define FUNCTIONALEXPANSIONSOLIDCARTESIANLEGENDRE_H

#include "FunctionalExpansionInterface.h"
#include "FunctionalSeriesLegendre.h"

enum CartesianBoundaries
{
  X_MIN, X_MAX,
  Y_MIN, Y_MAX,
  Z_MIN, Z_MAX,
  SIZE
};

class FunctionalExpansionSolidCartesianLegendre final : public FunctionalExpansionInterface
{
public:
  FunctionalExpansionSolidCartesianLegendre(unsigned int order_x,
                                            unsigned int order_y = 0,
                                            unsigned int order_z = 0);
  FunctionalExpansionSolidCartesianLegendre(std::array<unsigned int, 2> orders);
  FunctionalExpansionSolidCartesianLegendre(std::array<unsigned int, 3> orders);

  virtual ~FunctionalExpansionSolidCartesianLegendre() = default;

  virtual const std::vector<Real> & expand(const Point & location) override;
  virtual std::vector< std::vector<Real> > expand(const std::vector<const Point *> & locations) override;

  virtual Real sample(const Point & location) override;
  virtual std::vector<Real> sample(std::vector<const Point *> & locations) override;

protected:
  std::string getDescription() const override;
  std::string getFormattedCoefficients() const override;
  std::string getName() const override;

private:
  /// Evaluators for the Legendre polynomials
  FunctionalSeriesLegendre _x;
  FunctionalSeriesLegendre _y;
  FunctionalSeriesLegendre _z;
};

#endif // FUNCTIONALEXPANSIONSOLIDCARTESIANLEGENDRE_H
