#include "SerpentTimeStepper.h"

registerMooseObject("ChrysalisApp", SerpentTimeStepper);

template <>
InputParameters
validParams<SerpentTimeStepper>()
{
  InputParameters params = validParams<TimeStepper>();

  params.addClassDescription("TimeStepper for running the Serpent Reactor Physics MC code "
                             "developed at VTT, Finland");

#ifdef SERPENT_MPI_AVAILABLE
  params.addParam<long>("serpent_mpi_tasks",
                        -1,
                        "The number of MPI tasks with which to run Serpent. A value of '-1' will "
                        "default to the number of ranks in the communicator for this Executioner.");
#endif // SERPENT_MPI_AVAILABLE
#ifdef SERPENT_OPENMP_AVAILABLE
  params.addParam<int>("serpent_omp_threads",
                       -1,
                       "The number of OpenMP threads with which to run Serpent. A value of '-1' "
                       "will default to the number of threads used for this MultiApp.");
#endif // SERPENT_OPENMP_AVAILABLE

  return params;
}

SerpentTimeStepper::SerpentTimeStepper(const InputParameters & parameters)
  : TimeStepper(parameters),
    _first_run(true),
#ifdef SERPENT_MPI_AVAILABLE
    _serpent_mpi_tasks(getParam<long>("serpent_mpi_tasks")),
#else
    _serpent_mpi_tasks(0),
#endif // SERPENT_MPI_AVAILABLE
#ifdef SERPENT_OPENMP_AVAILABLE
    _serpent_omp_threads(getParam<int>("serpent_omp_threads"))
#else
    _serpent_omp_threads(0)
#endif // SERPENT_OPENMP_AVAILABLE
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
  /* Assume that Serpent has always converged so that the simulation will continue */
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
