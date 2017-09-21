#ifndef TREATHEATAUX_H
#define TREATHEATAUX_H

#include "AuxKernel.h"

// Forward Declarations
class TREATHeatAux;

template <>
InputParameters validParams<TREATHeatAux>();

/**
 * This AuxKernel implements a heating term that generalizes the behavior of the
 * TREAT reactor during a transient event in a single core element
 *
 * The axial direction of the fuel element is assumed to be in 'x'
 */
class TREATHeatAux : public AuxKernel
{
public:
  TREATHeatAux(const InputParameters & parameters);

  const Real _element_length;
  const Real _element_length_half;
  const Real _element_widths;
  const Real _element_widths_half;
  const unsigned int _number_of_elements;
  const Real _total_volume;
  const Real _zero_power_radius;

protected:
  virtual Real computeValue() override;

  const VariableValue & _in;
  
  const Point _center;
  const Real _total_energy;
  const Real _transient_duration;
  const Real _front_edge;
  const Real _max_heating_rate;
  const Real _max_temperature;
  const Real _initial_temperature;
};

#endif // TREATHEATAUX_H
