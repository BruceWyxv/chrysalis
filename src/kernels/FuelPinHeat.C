#include "FuelPinHeat.h"

#include "math.h"

const Real _ninety_log = log(0.9);

template <>
InputParameters
validParams<FuelPinHeat>()
{
  InputParameters params = validParams<Kernel>();

  params.addRequiredParam<Point>("center", "The center of the fuel pin");
  params.addRangeCheckedParam<Real>("total_energy", "total_energy > 1", "The total energy that can be generated during a transient event");
  params.addRangeCheckedParam<Real>("max_temperature", "max_temperature <= 925", "The maximum design temperature of the transient");
  params.addRangeCheckedParam<Real>("initial_temperature", "initial_temperature >= 200", "The initial temperature (at which heat generation is maximum)");

  return params;
}

FuelPinHeat::FuelPinHeat(const InputParameters & parameters)
  : Kernel(parameters),
    _element_length(5), // cm
    _element_length_half(_element_length / 2.0), // cm
    _element_widths(2), // cm
    _element_widths_half(_element_widths / 2.0), // cm
    _total_volume(M_PI * _element_widths_half * _element_widths_half * _element_length), // cm^3
    _zero_power_radius(1.5 * _element_widths),
    _center(getParam<Point>("center")),
    _total_energy(getParam<Real>("total_energy")),
    _max_heating_rate(_total_energy / _total_volume),
    _max_temperature(getParam<Real>("max_temperature")),
    _initial_temperature(getParam<Real>("initial_temperature"))
{
}

Real
FuelPinHeat::computeQpResidual()
{
  const Point & point = _q_point[_qp];
  const Real x = point(0) - _center(0);
  const Real y = point(1) - _center(1);
  const Real z = abs(point(2) - _center(2));

  const Real axial_factor = cos(M_PI * pow(z / _element_length_half, 4) / 3);
  const Real radial_factor = 1 - sqrt(x * x + y * y) / _zero_power_radius;
  const Real neutronic_heating = exp(-2.30258509299 * (_u[_qp] - _initial_temperature) / (_max_temperature - _initial_temperature)); // 10% heat generation at _max_temperature

  return -_test[_i][_qp] * _max_heating_rate * neutronic_heating * axial_factor * radial_factor;
}
