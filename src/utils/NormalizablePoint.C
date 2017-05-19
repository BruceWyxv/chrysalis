#include "NormalizablePoint.h"

NormalizablePoint::NormalizablePoint(Real x,
                                     Real y,
                                     Real z)
: Point(x, y, z),
  _bounds({-1, 1, -1, 1, -1, 1}),
  _is_normalized(false),
  _normalized_bounds({-1, 1, -1, 1, -1, 1})
{
  // Empty constructor
}

NormalizablePoint::NormalizablePoint(const Point & point)
: Point(point),
  _bounds({-1, 1, -1, 1, -1, 1}),
  _is_normalized(false),
  _normalized_bounds({-1, 1, -1, 1, -1, 1})
{
  // Empty constructor
}

NormalizablePoint::NormalizablePoint(const std::vector<Real> && bounds,
                                     Real x,
                                     Real y,
                                     Real z)
  : Point(x, y, z),
    _bounds(bounds),
    _is_normalized(false),
    _normalized_bounds({-1, 1, -1, 1, -1, 1})
{
  libmesh_assert_equal_to(_bounds.size(), 6);
}

NormalizablePoint::NormalizablePoint(const std::vector<Real> && bounds,
                                     const std::vector<Real> && normalized_bounds,
                                     Real x,
                                     Real y,
                                     Real z)
  : Point(x, y, z),
    _bounds(bounds),
    _normalized_bounds(normalized_bounds)
{
  map_point(*this, _bounds, _normalized_bounds);
  _is_normalized = true;
}

NormalizablePoint::NormalizablePoint(const std::vector<Real> && bounds,
                                     const std::vector<Real> && normalized_bounds,
                                     const Point & point)
  : Point(point),
    _bounds(bounds),
    _normalized_bounds(normalized_bounds)
{
  map_point(*this, _bounds, _normalized_bounds);
  _is_normalized = true;
}

NormalizablePoint::NormalizablePoint(const std::vector<Real> & bounds,
                                     Real x,
                                     Real y,
                                     Real z)
  : Point(x, y, z),
    _bounds(bounds),
    _is_normalized(false),
    _normalized_bounds({-1, 1, -1, 1, -1, 1})
{
  _bounds.resize(6, 0);
}

NormalizablePoint::NormalizablePoint(const std::vector<Real> & bounds,
                                     const std::vector<Real> & normalized_bounds,
                                     Real x,
                                     Real y,
                                     Real z)
  : Point(x, y, z),
    _bounds(bounds),
    _normalized_bounds(normalized_bounds)
{
  map_point(*this, _bounds, _normalized_bounds);
  _is_normalized = true;
}

NormalizablePoint::NormalizablePoint(const std::vector<Real> & bounds,
                                     const std::vector<Real> & normalized_bounds,
                                     const Point & point)
  : Point(point),
    _bounds(bounds),
    _normalized_bounds(normalized_bounds)
{
  map_point(*this, _bounds, _normalized_bounds);
  _is_normalized = true;
}

NormalizablePoint &
NormalizablePoint::getExpanded()
{
  if (_is_normalized)
  {
    map_point(*this, _normalized_bounds, _bounds);

    _is_normalized = false;
  }

  return *this;
}

const NormalizablePoint
NormalizablePoint::getExpanded() const
{
  if (!_is_normalized)
    return *this;

  Point expanded_point(*this);

  map_point(expanded_point, _normalized_bounds, _bounds);

  return expanded_point;
}

NormalizablePoint &
NormalizablePoint::getNormalized()
{
  if (!_is_normalized)
  {
    map_point(*this, _bounds, _normalized_bounds);

    _is_normalized = true;
  }

  return *this;
}

const NormalizablePoint
NormalizablePoint::getNormalized() const
{
  if (_is_normalized)
    return *this;

  Point normalized_point(*this);

  map_point(normalized_point, _bounds, _normalized_bounds);

  return normalized_point;
}

void
NormalizablePoint::map_point(Point & point_to_expand,
                             const std::vector<Real> & from_bounds,
                             const std::vector<Real> & to_bounds)
{
  Real to_diff;
  Real from_diff;
  Real ratio;

  libmesh_assert_equal_to(from_bounds.size(), 6);
  libmesh_assert_equal_to(to_bounds.size(), 6);

  // Convert x
  to_diff = to_bounds[1] - to_bounds[0];
  from_diff = from_bounds[1] - from_bounds[0];
  ratio = to_diff / from_diff;
  point_to_expand(0) = to_bounds[0] + (point_to_expand(0) - from_bounds[0]) * ratio;

  // Convert y
  to_diff = to_bounds[3] - to_bounds[2];
  from_diff = from_bounds[3] - from_bounds[2];
  ratio = to_diff / from_diff;
  point_to_expand(1) = to_bounds[2] + (point_to_expand(1) - from_bounds[2]) * ratio;

  // Convert z
  to_diff = to_bounds[5] - to_bounds[4];
  from_diff = from_bounds[5] - from_bounds[4];
  ratio = to_diff / from_diff;
  point_to_expand(2) = to_bounds[4] + (point_to_expand(2) - from_bounds[4]) * ratio;
}

NormalizablePoint &
NormalizablePoint::setBounds(const std::vector<Real> & bounds)
{
  libmesh_assert_equal_to(bounds.size(), 6);

  _bounds = bounds;

  return *this;
}

NormalizablePoint &
NormalizablePoint::setBounds(const std::vector<Real> && bounds)
{
  libmesh_assert_equal_to(bounds.size(), 6);

  _bounds = bounds;

  return *this;
}

NormalizablePoint &
NormalizablePoint::setNormalizedRange(const std::vector<Real> & range)
{
  libmesh_assert_equal_to(range.size(), 6);

  _normalized_bounds = range;

  return *this;
}

NormalizablePoint &
NormalizablePoint::setNormalizedRange(const std::vector<Real> && range)
{
  libmesh_assert_equal_to(range.size(), 6);

  _normalized_bounds = range;

  return *this;
}
