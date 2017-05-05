#ifndef FUNCTIONALEXPANSIONINTERFACE_H
#define FUNCTIONALEXPANSIONINTERFACE_H

// libMesh includes
#include "libmesh/point.h"

#include <vector>

/**
 * This class provides an interface for operating on functional expansions. In
 * particular, it contains abstractions for evaluating polynomials, collapsing a
 * variable space to a set of expansion coefficients, and expanding a set of
 * coefficients back to the variable space.
 */
class FunctionalExpansionInterface
{
public:
  /**
   * Class constructors
   * Sets the number of coefficients for the underlying functional expansion(s)
   * and sizes the underlying data structures accordingly
   * @param number_of_coefficients The number of coefficients as reported by the
   * implementing class
   */
  FunctionalExpansionInterface(unsigned int number_of_coefficients);

  // Disable the copy constructors
  FunctionalExpansionInterface(FunctionalExpansionInterface const&) = delete;
  FunctionalExpansionInterface& operator=(FunctionalExpansionInterface const&) = delete;

  /**
   * Empty class destructor
   */
  virtual ~FunctionalExpansionInterface() = default;

  /**
   * Evaluates and returns the individual expansion terms in the functional
   * series
   * @param location The Point at which to evaluate the functional expansion
   * @return A vector of the evaluated terms in the functional expansion
   */
  virtual const std::vector<libMesh::Real> & operator()(libMesh::Point location) {return expand(location);};
  virtual const std::vector<libMesh::Real> & expand(libMesh::Point location) = 0;
  virtual std::vector< std::vector<libMesh::Real> > operator()(std::vector<libMesh::Point> & locations) {return expand(locations);};
  virtual std::vector< std::vector<libMesh::Real> > expand(std::vector<libMesh::Point> & locations) = 0;

  /// Returns the current set of expansion coefficients
  const std::vector<libMesh::Real> & getCoefficients() {return _coefficients;};

  /// Returns the number of coefficients
  unsigned int getNumberOfCoefficients() {return _number_of_coefficients;};

  /**
   * Resets all the coefficients to 1, essentially resulting in a pure
   * orthonormal evaluation
   */
  void resetCoefficients() {for (auto& coefficient : _coefficients) coefficient = 1;};

  /// Returns the superimposed expansions terms
  virtual libMesh::Real sample(libMesh::Point location) = 0;
  virtual std::vector<libMesh::Real> sample(std::vector<libMesh::Point> & locations) = 0;

  /// Sets the expansion coefficients
  virtual void setCoefficients(std::vector<libMesh::Real> && coefficients);
  virtual void setCoefficients(std::vector<libMesh::Real> & coefficients);

protected:
  /// The expansion coefficients in linear sequence
  std::vector<libMesh::Real> _coefficients;

  /// The number of expansion coefficients
  const unsigned int _number_of_coefficients;

  /// The previous expansion
  std::vector<libMesh::Real> _expansion;

  /// The previous location
  libMesh::Point _location;

  /// Indicates if the cached values are valid
  bool _is_cache_valid;
};

#endif // FUNCTIONALEXPANSIONINTERFACE_H
