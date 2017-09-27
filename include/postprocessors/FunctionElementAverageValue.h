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

#ifndef FUNCTIONELEMENTAVERAGEVALUE_H
#define FUNCTIONELEMENTAVERAGEVALUE_H

#include "FunctionElementIntegral.h"

// Forward Declarations
class FunctionElementAverageValue;

template <>
InputParameters validParams<FunctionElementAverageValue>();

/**
 * This postprocessor computes the integral of a function over a specified boundary
 */
class FunctionElementAverageValue : public FunctionElementIntegral
{
public:
  FunctionElementAverageValue(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void finalize() override;
  virtual Real getValue() override;
  virtual void threadJoin(const UserObject & y) override;

protected:
  /// The volume
  Real _volume;

private:
  Real _value;
};

#endif // FUNCTIONELEMENTAVERAGEVALUE_H
