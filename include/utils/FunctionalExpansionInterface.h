#ifndef FUNCTIONALEXPANSIONINTERFACE_H
#define FUNCTIONALEXPANSIONINTERFACE_H

#include "NormalizablePoint.h"

#include <vector>

/**
 * \brief Provides an interface for operating on functional expansions
 * \details Functionality/abstractions are provided for:
 *            -# checking the bounds of a point for coverage by the underlying
 *               functional expansion
 *            -# evaluating a functional expansion at (a) given location(s)
 *            -# setting/getting the coefficient values for the underlying
 *               functional expansion
 *            -# proper handling of expansion coefficients produced with either
 *               ordinary or orthonormal functions
 *            -# sampling the underlying functional expansion at (a) given
 *               location(s)
 */
class FunctionalExpansionInterface
{
protected:
  // Internal typedef for convenience
  typedef libMesh::Real Real;
  typedef libMesh::Point Point;

public:
  /**
   * \brief Class constructor
   * \details Sets the number of coefficients for the underlying functional
   *          expansion(s) and sizes the underlying data structures accordingly
   * \param number_of_coefficients The number of coefficients as reported by the
   *                               implementing class
   * \param range The valid normalized range of an input point for the
   *              implementing class
   */
  FunctionalExpansionInterface(unsigned int number_of_coefficients,
                               const std::vector<Real> && range);

  /**
   * \name Deleted Functions
   */
  ///\{

  /// Deleted copy constructor
  FunctionalExpansionInterface(FunctionalExpansionInterface const&) = delete;

  /// Deleted assignment operator
  FunctionalExpansionInterface& operator=(FunctionalExpansionInterface const&) = delete;

  ///\}

  /// Empty class destructor
  virtual ~FunctionalExpansionInterface() = default;

  /**
   * \brief Evaluates and returns the individual expansion terms in the functional
   *        series.
   * \param location The point at which to evaluate the functional expansion
   * \return A vector reference of the evaluated terms in the functional
   *         expansion
   */
  virtual const std::vector<Real> & expand(const Point & location) = 0;
  /**
   * \brief Returns the previously evaluated individual expansion terms in the
   *        functional series.
   * \details The input parameter is unused.
   * \return A vector of the evaluated terms in the previously evaluated
   *         functional expansion
   */
  virtual const std::vector<Real> & expand(const Point & /*location*/) const {return _expansion;};
  /**
   * \brief \copybrief expand(const Point &) const
   * \return A vector of the evaluated terms in the previously computed
   *         functional expansion
   */
  virtual const std::vector<Real> & expand() const {return _expansion;};
  /**
   * \brief Evaluates and returns the individual expansion terms in the
   *        functional series for a set of points.
   * \details Any overriding function should take advantage of vectrorization
   *          methodologies to improve performance when any of the points share
   *          one or more coordinates.
   * \param locations A vector of point at which to evaluate the functional
   *                  expansion
   * \return A vector containing the corresponding vectors of term evaluations
   *         for each input Point
   */
  virtual std::vector< std::vector<Real> > expand(const std::vector<const Point *> & locations) = 0;

  /// Returns the dimensionality of this functional expansion
  unsigned int getDimensionality() const {return _dimensionality;}

  /// Returns a string with class information
  virtual std::string getInformation() const;

  /// Returns the current set of expansion coefficients
  const std::vector<Real> & getCoefficients() const {return _coefficients;};
  /// Returns the number of coefficients
  unsigned int getNumberOfCoefficients() const {return _number_of_coefficients;};

  /**
   * \brief Determines if the provided point is within the boundaries of the
   *        underlying functional expansion.
   * \param point The point to check for inclusion
   */
  virtual bool isInBounds(const Point & point);

  /// Resets all the coefficients to 1
  void resetCoefficients() {for (auto& coefficient : _coefficients) coefficient = 1;};

  /**
   * Returns the full evaluation of the underlying functional expansion
   * \param location the point at which to sample the functional expansion
   */
  virtual Real sample(const Point & location) = 0;
  /**
   * \brief Returns the full evaluation of the underlying functional expansion
   *        at multiple locations.
   * \details Any overriding function should take advantage of vectrorization
   *          methodologies to improve performance when any of the points share
   *          one or more coordinates.
   * \param locations A vector of points at which to sample the functional
   *                  expansion
   * \return A vector of the corresponding evaluations at each location
   */
  virtual std::vector<Real> sample(std::vector<const Point *> & locations) = 0;

  /// Sets the bounds of the underlying functional expansion
  virtual void setBounds(const std::vector<Real> & bounds);
  /// \copydoc setBounds(const std::vector<Real> &) by rvalue
  virtual void setBounds(const std::vector<Real> && bounds);

  /// Sets the number of dimensions to use for an evaluation or expansion
  void setDimensionality(unsigned int dimensionality) {_dimensionality = dimensionality;}

  /// Sets the expansion coefficients
  virtual void setCoefficients(const std::vector<Real> & coefficients, bool orthonormalized = true);
  /// \copydoc setCoefficients(const std::vector<Real> &, bool) by rvalue
  virtual void setCoefficients(const std::vector<Real> && coefficients, bool orthonormalized = true);

  /// The normalized range of a mapped point
  const std::vector<Real> _range;

protected:
  /// Functional expansion information
  virtual std::string getDescription() const = 0;

  /// Formatted coefficients
  virtual std::string getFormattedCoefficients() const = 0;

  /// Class name
  virtual std::string getName() const = 0;

  /**
   * \brief Array of bounds.
   * \details Used for bounds checking in isInBounds() and coordinate
   *          normalization in normalizeLocation()
   */
  std::vector<Real> _bounds;

  /// The expansion coefficients in linear sequence
  std::vector<Real> _coefficients;

  /**
   * The functional expansion's dimensionality, to be used by any implementing
   * class as appropirate
   */
  unsigned int _dimensionality;

  /// The previous expansion
  std::vector<Real> _expansion;

  /// Indicates if the cached values are valid
  bool _is_cache_valid;

  /// The previous location
  Point _location;

  /// The number of expansion coefficients
  const unsigned int _number_of_coefficients;

  /**
   * Flag for whether the coefficients are already orthonormalized or not. Any
   * implementing class should check this value during sample() and perform the
   * appropriate action
   */
  bool _orthonormalized_coefficients;
};

/// Operator << for ostream use
std::ostream & operator<<(std::ostream & stream, FunctionalExpansionInterface const & me);

#endif // FUNCTIONALEXPANSIONINTERFACE_H
