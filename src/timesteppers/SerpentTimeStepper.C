#include "SerpentTimeStepper.h"

registerMooseObject("ChrysalisApp", SerpentTimeStepper);

template <>
InputParameters
validParams<SerpentTimeStepper>()
{
  InputParameters params = validParams<TimeStepper>();

  params.addClassDescription("TimeStepper for running the Serpent Reactor Physics MC code "
                             "developed at VTT, Finland");

  return params;
}

SerpentTimeStepper::SerpentTimeStepper(const InputParameters & parameters)
  : TimeStepper(parameters), _first_run(true)
{
}

Real
SerpentTimeStepper::computeDT()
{
  return 1.0;
}

Real
SerpentTimeStepper::computeInitialDT()
{
  return 1.0;
}

bool
SerpentTimeStepper::converged()
{
  return true;
}

void
SerpentTimeStepper::init()
{
}

void
SerpentTimeStepper::postExecute()
{
}

void
SerpentTimeStepper::step()
{
  if (_first_run)
  {
    // Do the special stuff required for the first Serpent run
    _first_run = false;
  }

  // Do the rest of the stuff
}
