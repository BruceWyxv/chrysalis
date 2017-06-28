#ifndef ELEMENTVARIABLESDIFFERENCEVARIANCE_H
#define ELEMENTVARIABLESDIFFERENCEVARIANCE_H

#include "ElementVectorPostprocessor.h"

// Forward Declarations
class ElementVariablesDifferenceVariance;

template <>
InputParameters validParams<ElementVariablesDifferenceVariance>();

class ElementVariablesDifferenceVariance : public ElementVectorPostprocessor
{
public:
  ElementVariablesDifferenceVariance(const InputParameters & parameters);

  // Required overrides
  virtual void execute() override;
  virtual void finalize() override;
  virtual void initialize() override;
  virtual void threadJoin(const UserObject & s) override;

protected:
  /// The algorithm to combine results from two parallel entities
  void combineResults(Real & mean,
                      Real & sum_squares,
                      Real & weight_sum,
                      Real & weight_sum_squares,
                      const Real mean_other,
                      const Real sum_squares_other,
                      const Real weight_sum_other,
                      const Real weight_sum_squares_other);

  /// The first variable, operated to produce a difference as: #_a - #_b
  const VariableValue & _a;

  /// The second variable, operated to produce a difference as: #_a - #_b
  const VariableValue & _b;

  /// The mean of the differences
  VectorPostprocessorValue & _final_mean;

  /// The variance of the differences
  VectorPostprocessorValue & _final_variance;

  /// The running mean for the online algorithm
  Real _running_mean;

  /// The running sum of squares for the online algorithm
  Real _running_sum_squares;

  /// The running population/weight for the online algorithm
  Real _running_weight_sum;

  /// The running population/weight sum of squares for the online algorithm
  Real _running_weight_sum_squares;
};

#endif // ELEMENTVARIABLESDIFFERENCEVARIANCE_H
