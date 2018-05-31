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
  params.set<std::string>("_object_name", "SerpentTimeStepper");

  /*
   * Files for interfacing
   */
  params.addParam<std::string>(
      "serpent_input",
      "",
      "Name of the main Serpent input file to be used as a template for creating the multiphysics "
      "interface. Namely, the communication settings and the \"ifc ...\" lines will be appended to "
      "a duplicate file using the name \"'serpent_input'.moose\".");
  params.addParamNamesToGroup("serpent_input", "Interface Files");

  /*
   * Parallel processing options
   */
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
    _serpent_input_template_file_name(getParam<std::string>("serpent_input")),
    _serpent_input_file_name(makeInputFileNameFromTemplate(_serpent_input_template_file_name)),
#ifdef SERPENT_OPENMP_AVAILABLE
    _serpent_omp_threads(getParam<int>("serpent_omp_threads") > 0
                             ? getParam<int>("serpent_omp_threads")
                             : libMesh::n_threads()),
#else
    _serpent_omp_threads(1),
#endif // SERPENT_OPENMP_AVAILABLE
    _first_serpent_step(true)
{
}

void
SerpentTimeStepper::appendCommand(int & argv,
                                  std::vector<std::string> & arg_strings,
                                  const std::string & new_arg)
{
  arg_strings.push_back(new_arg);
  ++argv;
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
  int argv = 0;
  std::vector<char *> argc;
  std::vector<std::string> arg_strings;
  std::ostringstream formatter;

  appendCommand(argv, arg_strings, "chrysalis");
  appendCommand(argv, arg_strings, _serpent_input_file_name);

#ifdef SERPENT_OPENMP_AVAILABLE
  if (_serpent_omp_threads > 1)
  {
    appendCommand(argv, arg_strings, "-omp");

    formatter.str("");
    formatter.clear();
    formatter << _serpent_omp_threads;
    appendCommand(argv, arg_strings, formatter.str());
  }
#endif // SERPENT_OPENMP_AVAILABLE

  for (auto & string : arg_strings)
    argc.push_back(&string[0]);
  serpentInit(argv, &argc[0]);
}

const std::string &
SerpentTimeStepper::getSerpentInputFileName() const
{
  return _serpent_input_file_name;
}

const std::string &
SerpentTimeStepper::getSerpentInputTemplateFileName() const
{
  return _serpent_input_template_file_name;
}

std::string
SerpentTimeStepper::makeInputFileNameFromTemplate(const std::string & template_name)
{
  return template_name + ".moose";
}

std::string
SerpentTimeStepper::makeMpiUnique()
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::ostringstream formatter("");
  formatter << ".m" << rank;

  return formatter.str();
}

int
SerpentTimeStepper::serpentInit(int argc, char ** argv)
{
  _console << COLOR_YELLOW << "Starting Serpent initialization..." << COLOR_DEFAULT << std::endl;

  /////////////////////////////////////////////////////////////////////////////////
  // 504 lines redacted to maintain compliance with RSICC export control license //
  /////////////////////////////////////////////////////////////////////////////////

  _console << COLOR_YELLOW << "Serpent initialization complete!" << COLOR_DEFAULT << std::endl;

  return 0;
}

void
SerpentTimeStepper::serpentPostIterate()
{
  /////////////////////////////////////////////////////////////////////////////////
  //  47 lines redacted to maintain compliance with RSICC export control license //
  /////////////////////////////////////////////////////////////////////////////////
}

void
SerpentTimeStepper::serpentPreIterate()
{
  /////////////////////////////////////////////////////////////////////////////////
  //  79 lines redacted to maintain compliance with RSICC export control license //
  /////////////////////////////////////////////////////////////////////////////////
}

void
SerpentTimeStepper::setInputFileName(const std::string & template_name)
{
  if (_serpent_input_template_file_name.empty())
  {
    _serpent_input_template_file_name = template_name;
    _serpent_input_file_name = makeInputFileNameFromTemplate(template_name);
  }
  else
    mooseError("SerpentTimeStepper: the template input file name has already been set.");
}

void
SerpentTimeStepper::step()
{
  _console << COLOR_YELLOW << "Starting a Serpent run..." << COLOR_DEFAULT << std::endl;

  if (_first_serpent_step)
    _first_serpent_step = false;
  else
    serpentPreIterate();

  /////////////////////////////////////////////////////////////////////////////////
  //   8 lines redacted to maintain compliance with RSICC export control license //
  /////////////////////////////////////////////////////////////////////////////////

  serpentPostIterate();

  _console << COLOR_YELLOW << "Serpent run complete." << COLOR_DEFAULT << std::endl;
}

SerpentTimeStepper::~SerpentTimeStepper()
{
  /////////////////////////////////////////////////////////////////////////////////
  //  15 lines redacted to maintain compliance with RSICC export control license //
  /////////////////////////////////////////////////////////////////////////////////
}
