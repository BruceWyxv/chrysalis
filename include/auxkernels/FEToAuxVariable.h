#ifndef FETOAUXVARIABLE_H
#define FETOAUXVARIABLE_H

#include "AuxKernel.h"
#include "FECoefficientsUserObject.h"

// Forward Declarations
class FEToAuxVariable;

template <>
InputParameters validParams<FEToAuxVariable>();

/**
 * This AuxKernel uses a FECoefficientsUserObject to evaluate a functional
 * expansion at all quadrature points in a mesh and store them in an AuxVariable
 */
class FEToAuxVariable : public AuxKernel
{
public:
  FEToAuxVariable(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  const FECoefficientsUserObject & _fe_evaluator;

  const bool _print_evaluations;
};

#endif //FETOAUXVARIABLE_H
