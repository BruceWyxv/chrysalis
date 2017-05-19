#ifndef NORMALIZABLEPOINT_H
#define NORMALIZABLEPOINT_H

#include <vector>

// libMesh includes
#include "libmesh/point.h"


class NormalizablePoint : public libMesh::Point
{
protected:
  // Internal typedefs for convenience
  typedef libMesh::Real Real;
  typedef libMesh::Point Point;

public:
  /// Default constructor
  NormalizablePoint(Real x = 0.0,
                    Real y = 0.0,
                    Real z = 0.0);
  /// Constructor to set the coordinates from a passed-in Point reference
  NormalizablePoint(const Point & point);
  /// Constructor to set bounds passed by rvalue
  NormalizablePoint(const std::vector<Real> && bounds,
                    Real x = 0.0,
                    Real y = 0.0,
                    Real z = 0.0);
  /// Constructor to set bounds and normalized range passed by rvalue
  NormalizablePoint(const std::vector<Real> && bounds,
                    const std::vector<Real> && normalized_range,
                    Real x = 0.0,
                    Real y = 0.0,
                    Real z = 0.0);
  /**
   * \brief Constructor to set bounds and normalized range passed by rvalue,
   *        and the coordinates from a Point reference
   */
  NormalizablePoint(const std::vector<Real> && bounds,
                    const std::vector<Real> && normalized_range,
                    const Point & point);
  /// Constructor to set bounds
  NormalizablePoint(const std::vector<Real> & bounds,
                    Real x = 0.0,
                    Real y = 0.0,
                    Real z = 0.0);
  /// Constructor to set bounds and normalized range
  NormalizablePoint(const std::vector<Real> & bounds,
                    const std::vector<Real> & normalized_range,
                    Real x = 0.0,
                    Real y = 0.0,
                    Real z = 0.0);
  /**
  * \brief Constructor to set bounds and normalized range, and the coordinates
  *        from a a Point reference
  */
  NormalizablePoint(const std::vector<Real> & bounds,
                    const std::vector<Real> & normalized_range,
                    const Point & point);
  /// Default destructor
  virtual ~NormalizablePoint() = default;

  /// Get this Point as expanded to the full bounds
  NormalizablePoint & getExpanded();
  /// Get a representative Point as expanded to the full bounds
  const NormalizablePoint getExpanded() const;
  /// Get this Point in normalized form
  NormalizablePoint & getNormalized();
  /// Get a representative Point in normalizd form
  const NormalizablePoint getNormalized() const;

  /// Returns true if normalized
  bool isNormalized() const {return _is_normalized;};

  /// Map a Point between any two spaces
  static void map_point(Point & point_to_remap,
                        const std::vector<Real> & from_bounds,
                        const std::vector<Real> & to_bounds);

  /// Set the expanded bounds
  NormalizablePoint & setBounds(const std::vector<Real> & bounds);
  /// Set the expanded bounds from an rvalue
  NormalizablePoint & setBounds(const std::vector<Real> && bounds);

  /// Set the normalized range
  NormalizablePoint & setNormalizedRange(const std::vector<Real> & range);
  /// Set the normalized range from an rvlaue
  NormalizablePoint & setNormalizedRange(const std::vector<Real> && range);

protected:
  /// The fully expanded bounds
  std::vector<Real> _bounds;

  /// Normalization flag
  bool _is_normalized;

  /// The normalization bounds
  std::vector<Real> _normalized_bounds;
};

#endif // NORMALIZABLEPOINT_H
