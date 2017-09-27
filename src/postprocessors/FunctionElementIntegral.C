/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "FunctionElementIntegral.h"
#include "Function.h"

template <>
InputParameters
validParams<FunctionElementIntegral>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  params.addParam<FunctionName>(
      "function",
      1.0,
      "This postprocessor will return the integral of this function");
  return params;
}

FunctionElementIntegral::FunctionElementIntegral(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters), _func(getFunction("function"))
{
}

void
FunctionElementIntegral::threadJoin(const UserObject & y)
{
  const FunctionElementIntegral & pps = static_cast<const FunctionElementIntegral &>(y);
  _integral_value += pps._integral_value;
}

Real
FunctionElementIntegral::computeQpIntegral()
{
  return _func.value(_t, _q_point[_qp]);
}
