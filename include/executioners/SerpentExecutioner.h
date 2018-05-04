#ifndef SERPENTEXECUTIONER_H
#define SERPENTEXECUTIONER_H

#include "FunctionInterface.h"

#include "FXExecutioner.h"

class Function;
class SerpentExecutioner;
class SerpentTimeStepper;

template <>
InputParameters validParams<SerpentExecutioner>();

/**
 * This class is responsible for orchestrating the interaction with Serpent by:
 *   1) Ensuring that the Serpent input file exists
 *   2) Generating the power/density multiphysics input files
 *   3) Parsing the fission power density output file from each iteration
 */
class SerpentExecutioner : public FXExecutioner, FunctionInterface
{
public:
  SerpentExecutioner(const InputParameters & parameters);

protected:
  /**
   * Extracts the coefficients generated from Serpent and loads them into the MCI instance
   */
  virtual void exportCoefficients(const std::vector<Real> & out_coefficients) override;

  /**
   * Takes the coefficients from MCI and writes them to the Serpent multiphysics input files
   */
  virtual void importCoefficients(std::vector<Real> & array_to_fill) override;

  /**
   * Sets the default TimeStepper to SerpentTimeStepper if needed, then calls Transient::init()
   */
  virtual void init() override;

protected:
  /// Name of the multiphysics interface file for density
  const std::string _serpent_interface_density_file_name;

  /// Name of the multiphysics interface file for temperatures
  const std::string _serpent_interface_temperature_file_name;

  /// The name of the material in Serpent that will have its cross-sections adjusted on-the-fly
  /// using the exported FE
  const std::string _serpent_otf_material;

  /// The name of the material in Serpent that is the outermost layer of a pin definition, used to
  /// identify which pins should be scored for the fission power density FET
  const std::string _serpent_fission_power_outermost_material;

  /// Name of the multiphysics interface file for fission power density
  const std::string _serpent_interface_fission_power_density_file_name;

  /// Keeps all the files by creating files with the following format: file_name_#S_#P, where #S is
  /// the current step and #P is the number of Picard iterations at time step #P
  const bool _keep_files;

  /// A boolean that specifies where to request the power density FETs
  const bool _request_fission_power_in_density_file;

  /// A boolean that defines whether the power level is an average or scaling
  const bool _is_power_level_an_average;

  /// A boolean that defines whether the power level is constant or time-varying
  const bool _is_power_level_time_varying;

  /// A fixed value that defines the power level
  const Real _const_power_level;

  /// A Function that defines a power level
  Function * const _function_power_level;

  /**
   * Get the name of the output file that contains the fission power density FET from Serpent
   */
  std::string getFissionPowerDensityFileName() const;

  /**
   * Returns the time stepper as a SerpentTimeStepper reference
   */
  const SerpentTimeStepper & getMySerpentTimeStepper() const;

  /**
   * Get the string that corresponds to the keep_files file name tracking component
   */
  std::string getTrackingFileNameComponent() const;
};

#endif // SERPENTEXECUTIONER_H
