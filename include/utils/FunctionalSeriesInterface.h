#ifndef FUNCTIONALSERIESINTERFACE_H
#define FUNCTIONALSERIESINTERFACE_H

// MOOSE includes
#include "MooseError.h"

// libMesh includes
#include"libmesh/libmesh_common.h"

#include <array>
#include <vector>

/// Provides an interface for the basis set of a functional expansion
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

  /// Returns the evaluation at the given index
  Real operator[](std::size_t index) {return _series[index];}

  /**
   * Evaluates the orthonormalized functional series at location, then returns
   * the requested values which are linear mappings of a multi dimensions
   * function system
   */
  const std::vector<Real> & evaluateOrthonormal(std::array<Real, dimensions> location);
  /**
   * Evaluates the functional series at location, then returns the requested
   * values which are linear mappings of a multi dimensions function system
   */
  const std::vector<Real> & evaluatePure(std::array<Real, dimensions> location);

  /// Returns whether the current evaluation is orthonormalized
  bool isOrthonormalized() const {return _is_orthonormalized;}

  /**
   * Returns the location of the cached values in the listed dimension
   * (default = 0)
   */
  const Real & getLocation(unsigned int dimension = 0) const {return _location[dimension];};

  /// Returns the order of the listed dimension (default = 0)
  unsigned int getOrder(unsigned int dimension = 0) const {return _orders[dimension];};

protected:
  /// Evaluates the orthonormal functional series at #_location
  virtual void orthonormal() = 0;
  /// Evaluates the pure functional series at #_location
  virtual void pure() = 0;

  /// Helper function to load a value from #_series
  Real load(std::size_t index) const {return _series[index];};

  /// Helper function to store a value in #_series
  void save(std::size_t index, Real value) {_series[index] = value;};

  /// Stores the location of the evaluation
  std::array<Real, dimensions> _location;

  /// The polynomial coefficient orders
  const std::array<unsigned int, dimensions> _orders;

private:
  /// Indicates if the cached values are still valid
  bool _is_cache_valid;

  /// Indicates whether the cache evaluate is pure or orthonormalized
  bool _is_orthonormalized;

  /// Stores the values of the series evaluation
  std::vector<Real> _series;
};

template<std::size_t dimensions>
FunctionalSeriesInterface<dimensions>::FunctionalSeriesInterface(std::array<unsigned int, dimensions> orders, unsigned int number_of_coefficients)
  : _location({{}}),
    _orders(orders),
    _is_cache_valid(false),
    _series(number_of_coefficients, 0)
{
  _series.shrink_to_fit();
}

template<std::size_t dimensions>
const std::vector<Real> &
FunctionalSeriesInterface<dimensions>::evaluateOrthonormal(std::array<Real, dimensions> location)
{
  if (!_is_cache_valid || !_is_orthonormalized || _location != location)
  {
    _location = location;
    orthonormal();
    _is_cache_valid = true;
    _is_orthonormalized = true;
  }

  return _series;
}

template<std::size_t dimensions>
const std::vector<Real> &
FunctionalSeriesInterface<dimensions>::evaluatePure(std::array<Real, dimensions> location)
{
  if (!_is_cache_valid || _is_orthonormalized || _location != location)
  {
    _location = location;
    pure();
    _is_cache_valid = true;
    _is_orthonormalized = false;
  }

  return _series;
}

#endif // FUNCTIONALSERIESINTERFACE_H
