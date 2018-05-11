#ifndef SERPENTTIMESTEPPER_H
#define SERPENTTIMESTEPPER_H

#include "TimeStepper.h"

#include "header.h"
#include "locations.h"

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
  virtual ~SerpentTimeStepper();

  /**
   * Gets the name of the main input file for Serpent
   */
  const std::string & getSerpentInputFileName() const;

  /**
   * Gets the name of the main input file for Serpent
   */
  const std::string & getSerpentInputTemplateFileName() const;

  // /**
  //  * Gets the name of the file in which any POSIX signals from Serpent will be found
  //  */
  // const std::string & getSignalFromSerpentFileName() const;
  //
  // /**
  //  * Gets the name of the file to which any POSIX signals to Serpent will be written
  //  */
  // const std::string & getSignalToSerpentFileName() const;

  /**
   * Generate a string that is unique for each instance based on any threading and MPI ranks
   */
  static std::string makeMpiUnique();

  /**
   * Generate the name of the file that is to be used as the main input file
   */
  static std::string makeInputFileNameFromTemplate(const std::string & template_name);

  /**
   * Set the input file name if required
   */
  void setInputFileName(const std::string & template_name);

protected:
  /* Overrides from TimeStepper */
  virtual Real computeDT() override;
  virtual Real computeInitialDT() override;
  virtual bool converged() override;
  virtual void init() override;
  virtual void step() override;

protected:
  // /// Name of the file used to receive POSIX signals from Serpent
  // const std::string _posix_signals_from_serpent_file_name;
  //
  // /// Name of the file used to send POSIX signals to Serpent
  // const std::string _posix_signals_to_serpent_file_name;

  /// Name of the main Serpent input file name
  std::string _serpent_input_template_file_name;

  /// Name of the main Serpent input file name
  std::string _serpent_input_file_name;

  /// The number of OpenMP threads with which to run Serpent
  const int _serpent_omp_threads;

  // /**
  //  * Make a guaranteed unique name for each instance of SerpentTimeStepper
  //  */
  // static std::string
  // makePosixFileName(const std::string & file_base, const std::string & unique, bool
  // from_serpent);

private:
  /**
   * Helper to generate the command input arguments for Serpent
   */
  void
  appendCommand(int & argv, std::vector<std::string> & arg_strings, const std::string & new_arg);

  /**
   * The parts of IterateCC() and IterateExternal() that come after SignalExternal()
   */
  void serpentPostIterate();

  /**
   * The parts of IterateCC() and IterateExternal() that come before SignalExternal()
   */
  void serpentPreIterate();

  /**
   * Initializes Serpent
   */
  int serpentInit(int argc, char ** argv);

  // Flag for first Serpent step
  bool _first_serpent_step;

  /// The name of the main Serpent input file
  std::string _main_input_file_name;

  /// The name of the file in which any POSIX signals from Serpent will be found
  std::string _signal_input_file_name;

  /// The name of the file to which any POSIX signals to Serpent will be written
  std::string _signal_output_file_name;

  // These are used internally by Serpent, do not change
  long ptr, idx[10000], ncoef;
  char str[MAX_STR];
  double t;
};

#endif // SERPENTTIMESTEPPER_H
