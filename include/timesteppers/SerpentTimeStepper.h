#ifndef SERPENTTIMESTEPPER_H
#define SERPENTTIMESTEPPER_H

#include "TimeStepper.h"

class SerpentTimeStepper;

template <>
InputParameters validParams<SerpentTimeStepper>();

/**
 * This class is responsible for communicating with Serpent and starting each iteration's solve
 */
class SerpentTimeStepper : public TimeStepper
{
public:
  SerpentTimeStepper(const InputParameters & parameters);

protected:
  /* Overrides from TimeStepper */
  virtual Real computeDT() override;
  virtual Real computeInitialDT() override;
  virtual bool converged() override;
  virtual void init() override;
  virtual void postExecute() override;
  virtual void step() override;

private:
  /// The first Serpent run must be handled differently
  bool _first_run;
};

#endif // SERPENTTIMESTEPPER_H
