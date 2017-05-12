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

  virtual const std::vector<libMesh::Real> & expand(libMesh::Point location) override;
  virtual std::vector< std::vector<libMesh::Real> > expand(std::vector<const libMesh::Point *> & locations) override;

  virtual bool isInBounds(const libMesh::Point * point) override;

  virtual libMesh::Real sample(libMesh::Point location) override;
  virtual std::vector<libMesh::Real> sample(std::vector<const libMesh::Point *> & locations) override;

  virtual void setBounds(const std::vector<libMesh::Real> & bounds) override;

protected:
  void normalizeLocation(libMesh::Point & location);

private:
  /// Evaluators for the Legendre polynomials
  FunctionalSeriesLegendre _x;
  FunctionalSeriesLegendre _y;
  FunctionalSeriesLegendre _z;
};

#endif // FUNCTIONALEXPANSIONSOLIDCARTESIANLEGENDRE_H
