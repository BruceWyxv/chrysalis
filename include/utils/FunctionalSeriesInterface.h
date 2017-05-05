#ifndef FUNCTIONALSERIESINTERFACE_H
#define FUNCTIONALSERIESINTERFACE_H

// MOOSE includes
#include "MooseError.h"

// libMesh includes
#include"libmesh/libmesh_common.h"

#include <array>
#include <vector>

/// This class provides an interface for the basis set of a functional expansion
template<std::size_t dimensions>
class FunctionalSeriesInterface
{
public:
  /**
   * Constructor, sets up the class for evaluating a Legrendre polynomial series
   * in one dimension
   */
  FunctionalSeriesInterface(std::array<unsigned int, dimensions> orders, unsigned int number_of_coefficients);
  virtual ~FunctionalSeriesInterface() = default;

  // Disable the copy constructors
  FunctionalSeriesInterface(FunctionalSeriesInterface const&) = delete;
  FunctionalSeriesInterface& operator=(FunctionalSeriesInterface const&) = delete;

  /**
   * Evaluates the functional series at #location, then returns the requested
   * values which are linear mappings of a multi #dimensions function system
   */
  const std::vector<libMesh::Real> & operator()(std::array<libMesh::Real, dimensions> location) {return evaluate(location);};
  const std::vector<libMesh::Real> & evaluate(std::array<libMesh::Real, dimensions> location);

  /// Returns the value of #_series at the given index
  libMesh::Real operator[](std::size_t index) {return _series[index];};

  /**
   * Returns the location of the cached values in the listed dimension
   * (default = 0)
   */
  libMesh::Real & getLocation(unsigned int dimension = 0) {return _location[dimension];};

  /// Returns the order of the listed dimension (default = 0)
  unsigned int getOrder(unsigned int dimension = 0) {return _orders[dimension];};

protected:
  /**
   * Evaluates the functional series at #_location, then caches the results in
   * #_series and #_expansion
   */
  virtual void evaluateConcreteAndStoreResults() = 0;

  /// Loads a value from #_series
  libMesh::Real load(std::size_t index) {return _series[index];};

  /// Stores the value in #_series
  void save(std::size_t index, libMesh::Real value) {_series[index] = value;};

  /// Stores the location of the evaluation
  std::array<libMesh::Real, dimensions> _location;

  /// The polynomial coefficient orders
  const std::array<unsigned int, dimensions> _orders;

  /// Stores the values of the series evaluation
  std::vector<libMesh::Real> _series;

private:
  /// Indicates if the cached values are still valid
  bool _is_cache_valid;
};

template<std::size_t dimensions>
FunctionalSeriesInterface<dimensions>::FunctionalSeriesInterface(std::array<unsigned int, dimensions> orders, unsigned int number_of_coefficients)
  : _location({{}}),
    _orders(orders),
    _series(number_of_coefficients, 0),
    _is_cache_valid(false)
{
  _series.shrink_to_fit();
}

template<std::size_t dimensions>
const std::vector<libMesh::Real> &
FunctionalSeriesInterface<dimensions>::evaluate(std::array<libMesh::Real, dimensions> location)
{
  if (!_is_cache_valid || _location != location)
  {
    _location = location;
    evaluateConcreteAndStoreResults();
    _is_cache_valid = true;
  }

  return _series;
}

#endif // FUNCTIONALSERIESINTERFACE_H
