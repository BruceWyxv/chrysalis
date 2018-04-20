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

  /**
   * Gets the name of the main input file for Serpent
   */
  const std::string & GetSerpentInputFileName() const;

  /**
   * Gets the name of the main input file for Serpent
   */
  const std::string & GetSerpentInputTemplateFileName() const;

  /**
   * Gets the name of the file in which any POSIX signals from Serpent will be found
   */
  const std::string & GetSignalFromSerpentFileName() const;

  /**
   * Gets the name of the file to which any POSIX signals to Serpent will be written
   */
  const std::string & GetSignalToSerpentFileName() const;

  /**
   * Gets a unique identifier based on the OMP process and MPI task
   */
  const std::string & GetUniqueIdentifier() const;

protected:
  /* Overrides from TimeStepper */
  virtual Real computeDT() override;
  virtual Real computeInitialDT() override;
  virtual bool converged() override;
  virtual void init() override;
  virtual void postExecute() override;
  virtual void step() override;

protected:
  /// A unique identifier for each instance that should work for split processes and MPI ranks
  const std::string _unique;

  /// The first Serpent run must be handled differently
  bool _first_run;

  /// Name of the file used to receive POSIX signals from Serpent
  const std::string _posix_signals_from_serpent_file_name;

  /// Name of the file used to send POSIX signals to Serpent
  const std::string _posix_signals_to_serpent_file_name;

  /// Name of the main Serpent input file name
  const std::string _serpent_input_template_file_name;

  /// Name of the main Serpent input file name
  const std::string _serpent_input_file_name;

  /// The number of MPI tasks with which to run Serpent
  const long _serpent_mpi_tasks;

  /// The number of OpenMP threads with which to run Serpent
  const int _serpent_omp_threads;

  /**
   * Generate a string that is unique for each instance based on any threading and MPI ranks
   */
  static std::string makeOmpMpiUnique();

  /**
   * Make a guaranteed unique name for each instance of SerpentTimeStepper
   */
  static std::string
  makePosixFileName(const std::string & file_base, const std::string & unique, bool from_serpent);

private:
  /// The name of the main Serpent input file
  std::string _main_input_file_name;

  /// The name of the file in which any POSIX signals from Serpent will be found
  std::string _signal_input_file_name;

  /// The name of the file to which any POSIX signals to Serpent will be written
  std::string _signal_output_file_name;
};

#endif // SERPENTTIMESTEPPER_H
