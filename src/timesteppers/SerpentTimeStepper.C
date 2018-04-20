#include <atomic>

#include "SerpentTimeStepper.h"

registerMooseObject("ChrysalisApp", SerpentTimeStepper);

template <>
InputParameters
validParams<SerpentTimeStepper>()
{
  InputParameters params = validParams<TimeStepper>();

  params.addClassDescription("TimeStepper for running the Serpent Reactor Physics MC code "
                             "developed at VTT, Finland");

  /*
   * Files for interfacing
   */
  params.addRequiredParam<std::string>(
      "serpent_input",
      "Name of the main Serpent input file to be used as a template for creating the multiphysics "
      "interface. Namely, the communication settings and the \"ifc ...\" lines will be appended to "
      "a duplicate file using the name \"'serpent_input'.moose\".");
  params.addParam<std::string>("signal_file_base",
                               ".serpent_posix",
                               "The base name of the files to be used to pass signals between "
                               "MOOSE and Serpent. The actual files will be generated from this "
                               "base name using a guaranteed unique identifier for each "
                               "SerpentTimeStepper instance.");
  params.addParamNamesToGroup("serpent_input signal_file_base", "Interface Files");

  /*
   * Parallel processing options
   */
#ifdef SERPENT_MPI_AVAILABLE
  params.addParam<long>("serpent_mpi_tasks",
                        -1,
                        "The number of MPI tasks with which to run Serpent. A value of '-1' will "
                        "default to the number of ranks in the communicator for this Executioner.");
  params.addParamNamesToGroup("serpent_mpi_tasks", "Serpent Execution Options");
#endif // SERPENT_MPI_AVAILABLE
#ifdef SERPENT_OPENMP_AVAILABLE
  params.addParam<int>("serpent_omp_threads",
                       -1,
                       "The number of OpenMP threads with which to run Serpent. A value of '-1' "
                       "will default to the number of threads used for this MultiApp.");
  params.addParamNamesToGroup("serpent_omp_threads", "Serpent Execution Options");
#endif // SERPENT_OPENMP_AVAILABLE

  return params;
}

SerpentTimeStepper::SerpentTimeStepper(const InputParameters & parameters)
  : TimeStepper(parameters),
    _unique(declareRecoverableData<std::string>("unique", makeOmpMpiUnique())),
    _first_run(declareRecoverableData<bool>("first_run", true)),
    _posix_signals_from_serpent_file_name(
        makePosixFileName(getParam<std::string>("signal_file_base"), _unique, true)),
    _posix_signals_to_serpent_file_name(
        makePosixFileName(getParam<std::string>("signal_file_base"), _unique, false)),
    _serpent_input_template_file_name(getParam<std::string>("serpent_input")),
    _serpent_input_file_name(_serpent_input_template_file_name + ".moose"),
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

const std::string &
SerpentTimeStepper::GetSerpentInputFileName() const
{
  return _serpent_input_file_name;
}

const std::string &
SerpentTimeStepper::GetSerpentInputTemplateFileName() const
{
  return _serpent_input_template_file_name;
}

const std::string &
SerpentTimeStepper::GetSignalFromSerpentFileName() const
{
  return _posix_signals_from_serpent_file_name;
}

const std::string &
SerpentTimeStepper::GetSignalToSerpentFileName() const
{
  return _posix_signals_to_serpent_file_name;
}

const std::string &
SerpentTimeStepper::GetUniqueIdentifier() const
{
  return _unique;
}

std::string
SerpentTimeStepper::makeOmpMpiUnique()
{
  static std::atomic<uint64_t> id;
  const uint64_t next_id = id++;

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::ostringstream formatter("");
  formatter << ".o" << next_id << ".m" << rank;

  return formatter.str();
}

std::string
SerpentTimeStepper::makePosixFileName(const std::string & file_base,
                                      const std::string & unique,
                                      bool from_serpent)
{
  std::string file_name(file_base);

  file_name.append(unique + (from_serpent ? ".from_serpent" : ".to_serpent"));

  return file_name;
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
