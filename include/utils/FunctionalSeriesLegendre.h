#ifndef FUNCTIONALEXPANSIONLEGENDRE_H
#define FUNCTIONALEXPANSIONLEGENDRE_H

#include "FunctionalSeriesInterface.h"

#include <vector>

/**
 * This class provides a framework for working with the Legrendre polynomials.
 */
class FunctionalSeriesLegendre final : public FunctionalSeriesInterface<1>
{
public:
  /**
   * Constructor, sets up the class for evaluating a Legrendre polynomial series
   * in one dimension
   */
  FunctionalSeriesLegendre(unsigned int order);
  virtual ~FunctionalSeriesLegendre() = default;

protected:
  /// Evaluates an orthonormalized Legendre polynomial series at _location
  void orthonormal() final;
  /// Evaluates a pure Legendre polynomial series at _location
  void pure() final;
};

#endif // FUNCTIONALEXPANSIONLEGENDRE_H
