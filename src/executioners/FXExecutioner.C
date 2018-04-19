#include "FXExecutioner.h"

template <>
InputParameters
validParams<FXExecutioner>()
{
  InputParameters params = validParams<Transient>();

  params += validParams<MutableCoefficientsInterface>();

  return params;
}

FXExecutioner::FXExecutioner(const InputParameters & parameters)
  : Transient(parameters), MutableCoefficientsInterface(this, parameters)
{
}

void
FXExecutioner::postExecute()
{
  // Perform the default postExecute() actions, which includes the TimeStepper's postExecute()
  Transient::postExecute();

  _coefficients.clear();
  importCoefficients(_coefficients);
}

void
FXExecutioner::preExecute()
{
  exportCoefficients(_coefficients);

  // Perform the default preExecute() actions
  Transient::preExecute();
}
