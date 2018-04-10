#include "TREATHeatAux.h"

#include "math.h"

registerMooseObject("ChrysalisApp", TREATHeatAux);

const Real _ninety_log = log(0.9);

template <>
InputParameters
validParams<TREATHeatAux>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredCoupledVar("in", "The temperature of the system");

  params.addRequiredParam<Point>("center", "The center of the fuel element");
  params.addRangeCheckedParam<Real>(
      "total_energy",
      "total_energy > 1",
      "The total energy that can be generated during a transient event");
  params.addRangeCheckedParam<Real>(
      "transient_duration", "transient_duration > 0.25", "The duration of the transient event");
  params.addRangeCheckedParam<Real>("max_temperature",
                                    "max_temperature <= 925",
                                    "The maximum design temperature of the transient");
  params.addRangeCheckedParam<Real>(
      "initial_temperature",
      "initial_temperature >= 200",
      "The initial temperature (at which heat generation is maximum)");

  return params;
}

TREATHeatAux::TREATHeatAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _element_length(120),                        // cm
    _element_length_half(_element_length / 2.0), // cm
    _element_widths(10),                         // cm
    _element_widths_half(_element_widths / 2.0), // cm
    _number_of_elements(19 * 19),
    _total_volume(_number_of_elements * _element_widths * _element_widths *
                  _element_length), // cm^3
    _zero_power_radius(5 * sqrt(_element_widths * _element_widths)),
    _in(coupledValue("in")),
    _center(getParam<Point>("center")),
    _total_energy(getParam<Real>("total_energy")),
    _transient_duration(getParam<Real>("transient_duration")),
    _front_edge(_transient_duration / 3.0),
    _max_heating_rate(_total_energy / (_total_volume * _transient_duration)),
    _max_temperature(getParam<Real>("max_temperature")),
    _initial_temperature(getParam<Real>("initial_temperature"))
{
}

Real
TREATHeatAux::computeValue()
{
  const Point & point = isNodal() ? *_current_node : _q_point[_qp];
  const Real x = point(0) - _center(0);
  const Real y = point(1) - _center(1) + _element_widths_half;
  const Real z = point(2) - _center(2) + _element_widths_half;

  const Real axial_factor = cos(M_PI * pow(x / _element_length_half, 4) / 3);
  const Real radial_factor = 1 - sqrt(y * y + z * z) / _zero_power_radius;
  const Real neutronic_heating =
      exp(-2.30258509299 * (_in[_qp] - _initial_temperature) /
          (_max_temperature - _initial_temperature)); // 10% heat generation at _max_temperature
  const Real ramping =
      (_t <= _front_edge) ? (_t / _front_edge) : ((_t < _transient_duration) ? 1.0 : 1e-4);

  return _max_heating_rate * ramping * neutronic_heating * axial_factor * radial_factor;
}
