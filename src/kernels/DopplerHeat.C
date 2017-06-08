#include "DopplerHeat.h"

#include "math.h"

const Real _ninety_log = log(0.9);

template <>
InputParameters
validParams<DopplerHeat>()
{
  InputParameters params = validParams<Kernel>();

  params.addParam<Real>("full_point", 300.0, "Temperature at which the result is peak_value");
  params.addParam<Real>("ninety_point", 600.0, "Temperature at which the heat generation is 90%% of peak_value");
  params.addParam<Real>("peak_value", 430.0, "Peak value of the heat generation at full_point");
  params.addParam<Real>("ramp_time", -1, "Time over which the peak_value is increase from 0 to its full value");

  return params;
}

DopplerHeat::DopplerHeat(const InputParameters & parameters)
  : Kernel(parameters),
    _full_point(getParam<Real>("full_point")),
    _ninety_point(getParam<Real>("ninety_point")),
    _peak_value(getParam<Real>("peak_value")),
    _ramp_time(getParam<Real>("ramp_time"))
{
}

Real
DopplerHeat::computeQpResidual()
{
  Real factor = _peak_value * exp(_ninety_log / (_ninety_point - _full_point) * (_u[_qp] - _full_point));
  if (_ramp_time > 0 && _t < _ramp_time)
    factor *= _t / _ramp_time;
  return -_test[_i][_qp] * factor;
}
