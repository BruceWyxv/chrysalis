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

#include "FunctionElementAverageValue.h"

template <>
InputParameters
validParams<FunctionElementAverageValue>()
{
  InputParameters params = validParams<FunctionElementIntegral>();
  return params;
}

FunctionElementAverageValue::FunctionElementAverageValue(const InputParameters & parameters)
  : FunctionElementIntegral(parameters), _volume(0)
{
}

void
FunctionElementAverageValue::initialize()
{
  FunctionElementIntegral::initialize();
  _volume = 0;
  _console << COLOR_MAGENTA << "Evaluating Function Average!!! " << COLOR_DEFAULT << std::endl;
}

void
FunctionElementAverageValue::execute()
{
  FunctionElementIntegral::execute();

  _volume += _current_elem_volume;
}

void
FunctionElementAverageValue::finalize()
{
  Real integral = FunctionElementIntegral::getValue();

  gatherSum(_volume);

  _value = integral / _volume;
}

Real
FunctionElementAverageValue::getValue()
{
  return _value;
}

void
FunctionElementAverageValue::threadJoin(const UserObject & y)
{
  FunctionElementIntegral::threadJoin(y);
  const FunctionElementAverageValue & pps = static_cast<const FunctionElementAverageValue &>(y);
  _volume += pps._volume;
}
