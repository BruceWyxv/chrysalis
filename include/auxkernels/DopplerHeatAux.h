#ifndef DOPPLERHEATAUX_H
#define DOPPLERHEATAUX_H

#include "AuxKernel.h"

// Forward Declarations
class DopplerHeatAux;

template <>
InputParameters validParams<DopplerHeatAux>();

/**
 * This AuxKernel implements a generic heating term that attempts to emulate the
 * effect of Doppler broadening in nuclear fuel
 *
 * The results are placed in _in
 */
class DopplerHeatAux : public AuxKernel
{
public:
  DopplerHeatAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  const VariableValue & _in;

  const Real _full_point;
  const Real _ninety_point;
  const Real _peak_value;
  const Real _ramp_time;
};

#endif // DOPPLERHEATAUX_H
