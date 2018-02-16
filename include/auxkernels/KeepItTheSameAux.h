#ifndef KEEPITTHESAMEAUX_H
#define KEEPITTHESAMEAUX_H

#include "AuxKernel.h"

// Forward Declarations
class KeepItTheSameAux;

template <>
InputParameters validParams<KeepItTheSameAux>();

class KeepItTheSameAux : public AuxKernel
{
public:
  KeepItTheSameAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;
};

#endif // KEEPITTHESAMEAUX_H
