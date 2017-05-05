#ifndef FUNCTIONALEXPANSIONSOLIDCARTESIANLEGENDRE_H
#define FUNCTIONALEXPANSIONSOLIDCARTESIANLEGENDRE_H

#include "FunctionalExpansionInterface.h"
#include "FunctionalSeriesLegendre.h"

class FunctionalExpansionSolidCartesianLegendre final : public FunctionalExpansionInterface
{
public:
  FunctionalExpansionSolidCartesianLegendre(unsigned int order_x,
                                            unsigned int order_y = 0,
                                            unsigned int order_z = 0);
  FunctionalExpansionSolidCartesianLegendre(std::array<unsigned int, 2> orders);
  FunctionalExpansionSolidCartesianLegendre(std::array<unsigned int, 3> orders);

  virtual ~FunctionalExpansionSolidCartesianLegendre() = default;

  const std::vector<libMesh::Real> & expand(libMesh::Point location) override;
  std::vector< std::vector<libMesh::Real> > expand(std::vector<libMesh::Point> & locations) override;

  libMesh::Real sample(libMesh::Point location) override;
  std::vector<libMesh::Real> sample(std::vector<libMesh::Point> & locations) override;

private:
  /// Evaluators for the Legendre polynomials
  FunctionalSeriesLegendre _x;
  FunctionalSeriesLegendre _y;
  FunctionalSeriesLegendre _z;
};

#endif // FUNCTIONALEXPANSIONSOLIDCARTESIANLEGENDRE_H
