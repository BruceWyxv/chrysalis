#include "KeepItTheSameAux.h"

registerMooseObject("ChrysalisApp", KeepItTheSameAux);

template <>
InputParameters
validParams<KeepItTheSameAux>()
{
  InputParameters params = validParams<AuxKernel>();

  return params;
}

KeepItTheSameAux::KeepItTheSameAux(const InputParameters & parameters) : AuxKernel(parameters)
{
  // Nothing here
}

Real
KeepItTheSameAux::computeValue()
{
  return _u[_qp];
}
