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
  /**
   * Evaluates the Legendre polynomial series, then caches everything in
   * #_previous_location, #_previous_series, and #_previous_expansion
   */
  void evaluateConcreteAndStoreResults() final;
};

#endif // FUNCTIONALEXPANSIONLEGENDRE_H
