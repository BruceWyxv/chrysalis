#ifndef FUELPINHEAT_H
#define FUELPINHEAT_H

#include "Kernel.h"

// Forward Declarations
class FuelPinHeat;

template <>
InputParameters validParams<FuelPinHeat>();

/**
 * This Kernel implements a heating term that generalizes the behavior of the
 * TREAT reactor during a transient event in a single core element
 *
 * The axial direction of the fuel element is assumed to be in 'x'
 */
class FuelPinHeat : public Kernel
{
public:
  FuelPinHeat(const InputParameters & parameters);

  const Real _element_length;
  const Real _element_length_half;
  const Real _element_widths;
  const Real _element_widths_half;
  const Real _total_volume;
  const Real _zero_power_radius;

protected:
  virtual Real computeQpResidual() override;

  const Point _center;
  const Real _total_energy;
  const Real _max_heating_rate;
  const Real _max_temperature;
  const Real _initial_temperature;
};

#endif // FUELPINHEAT_H
