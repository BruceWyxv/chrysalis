#include "FEToAuxVariable.h"

template <>
InputParameters
validParams<FEToAuxVariable>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredParam<UserObjectName>(
      "fe_coefficients_user_object",
      "The FECoefficientsUserObject that will be expanded to the AuxVariable");

  return params;
}

FEToAuxVariable::FEToAuxVariable(const InputParameters & parameters)
  : AuxKernel(parameters),
    _fe_evaluator(getUserObject<FECoefficientsUserObject>("fe_coefficients_user_object"))
{
}

Real
FEToAuxVariable::computeValue()
{
  return _fe_evaluator.spatialValue(_q_point[_qp]);
}
