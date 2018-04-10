#ifndef SERPENTEXECUTIONER_H
#define SERPENTEXECUTIONER_H

#include "Transient.h"

#include "MutableCoefficientsInterface.h"

class SerpentExecutioner;
class Function;

template <>
InputParameters validParams<SerpentExecutioner>();

/**
 * This class is responsible for orchestrating the interaction with Serpent by:
 *   1) Ensuring that the Serpent input file exists
 *   2) Generating the power/density multiphysics input files
 *   3) Parsing the fission power density output file from each iteration
 */
class SerpentExecutioner : public Transient, public MutableCoefficientsInterface
{
public:
  SerpentExecutioner(const InputParameters & parameters);

protected:
  /**
   * Extracts the coefficients generated from Serpent and loads them into the MCI instance
   */
  virtual void postExecute() override;

  /**
   * Takes the coefficients from MCI and writes them to the Serpent multiphysics input files
   */
  virtual void preExecute() override;

  /**
   * Sets the default TimeStepper to SerpentTimeSteper, then calls Transient::init()
   */
  virtual void init() override;

private:
  /// Name of the main Serpent input file
  const std::string _serpent_input_file_name;

  /// Name of the multiphysics interface file for density
  const std::string _serpent_interface_density_file_name;

  /// Name of the multiphysics interface file for temperatures
  const std::string _serpent_interface_temperature_file_name;

  /// Name of the multiphysics interface file for fission power density
  const std::string _serpent_interface_fission_power_density_file_name;

  /// A function that defines the power level, may be a constant value or user-defined
  std::shared_ptr<Function> _power_level;

  /// A boolean that specifies where to request
};

#endif // SERPENTEXECUTIONER_H
