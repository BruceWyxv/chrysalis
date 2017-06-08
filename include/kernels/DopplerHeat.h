#ifndef DOPPLERHEAT_H
#define DOPPLERHEAT_H

#include "Kernel.h"

// Forward Declarations
class DopplerHeat;

template <>
InputParameters validParams<DopplerHeat>();

/**
 * This kernel implements a generic heating term that attempts to emulate the
 * effect of Doppler broadening in nuclear fuel
 */
class DopplerHeat : public Kernel
{
public:
  DopplerHeat(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  const Real _full_point;
  const Real _ninety_point;
  const Real _peak_value;
  const Real _ramp_time;
};

#endif // DOPPLERHEAT_H
