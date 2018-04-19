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

  /// The number of MPI tasks with which to run Serpent
  const long _serpent_mpi_tasks;

  /// The number of OpenMP threads with which to run Serpent
  const int _serpent_omp_threads;
};

#endif // SERPENTTIMESTEPPER_H
