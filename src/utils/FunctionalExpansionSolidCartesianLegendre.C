#include "FunctionalExpansionSolidCartesianLegendre.h"

#include <unordered_map>

FunctionalExpansionSolidCartesianLegendre::FunctionalExpansionSolidCartesianLegendre(unsigned int order_x,
                                                                                     unsigned int order_y,
                                                                                     unsigned int order_z)
  : FunctionalExpansionInterface((order_x + 1) * (order_y + 1) * (order_z + 1)),
    _x(order_x),
    _y(order_y),
    _z(order_z)
{
  // Nothing here, everything is prepared in the initialization list
}

FunctionalExpansionSolidCartesianLegendre::FunctionalExpansionSolidCartesianLegendre(std::array<unsigned int, 2> orders)
  : FunctionalExpansionSolidCartesianLegendre(orders[0], orders[1])
{
  // Nothing here, everything is performed by the delegate constructor
}

FunctionalExpansionSolidCartesianLegendre::FunctionalExpansionSolidCartesianLegendre(std::array<unsigned int, 3> orders)
  : FunctionalExpansionSolidCartesianLegendre(orders[0], orders[1], orders[2])
{
  // Nothing here, everything is performed by the delegate constructor
}

const std::vector<libMesh::Real> &
FunctionalExpansionSolidCartesianLegendre::expand(libMesh::Point location)
{
  if (_location != location)
  {
    // Evaluate the dimensional polynomials at the defined locaton
    _x({{location(0)}});
    _y({{location(1)}});
    _z({{location(2)}});

    _location = location;
    _is_cache_valid = false;
  }

  if (!_is_cache_valid)
  {
    // Loop over the dimensions and evaluate
    std::size_t index = 0;
    for (std::size_t x = 0; x <= _x.getOrder(); ++x)
      for (std::size_t y = 0; y <= _y.getOrder(); ++y)
        for (std::size_t z = 0; z <= _z.getOrder(); ++z)
        {
          _expansion[index] = _x[x] * _y[y] * _z[z] * _coefficients[index];
          ++index;
        }

    _is_cache_valid = true;
  }

  return _expansion;
}

std::vector< std::vector<libMesh::Real> >
FunctionalExpansionSolidCartesianLegendre::expand(std::vector<libMesh::Point> & locations)
{
  std::vector< std::vector<libMesh::Real> > expansions;
  expansions.reserve(locations.size());

  std::unordered_map< libMesh::Real, std::vector<libMesh::Real> > x_evaluations;
  std::unordered_map< libMesh::Real, std::vector<libMesh::Real> > y_evaluations;
  std::unordered_map< libMesh::Real, std::vector<libMesh::Real> > z_evaluations;

  /*
   * Perform an first loop to precompute all the needed expansions. This is
   * advantageous if multiple any coordinates in any dimension are repeated, and
   * adds very little overhead in any other situation.
   */
  for (auto& location : locations)
  {
    if (x_evaluations.count(location(0)) == 0)
      x_evaluations.emplace(location(0), _x({{location(0)}}));

    if (y_evaluations.count(location(1)) == 0)
      y_evaluations.emplace(location(1), _y({{location(1)}}));

    if (z_evaluations.count(location(2)) == 0)
      z_evaluations.emplace(location(2), _z({{location(2)}}));
  }

  // Perform the actual expansions in the second loop
  std::vector<libMesh::Real> expansion(_number_of_coefficients, 0);
  for (auto& location : locations)
  {
    auto& x_series = x_evaluations.at(location(0));
    auto& y_series = y_evaluations.at(location(1));
    auto& z_series = z_evaluations.at(location(2));

    std::size_t index = 0;
    for (std::size_t x = 0; x < x_series.size(); ++x)
      for (std::size_t y = 0; y < y_series.size(); ++y)
        for (std::size_t z = 0; z < z_series.size(); ++z)
        {
          expansion[index] = x_series[x] * y_series[y] * z_series[z] * _coefficients[index];
          ++index;
        }

    expansions.push_back(expansion);
  }

  return expansions;
}

libMesh::Real
FunctionalExpansionSolidCartesianLegendre::sample(libMesh::Point location)
{
  libMesh::Real sample = 0.0;
  expand(location);

  for (auto& term : _expansion)
    sample += term;

  return sample;
}

std::vector<libMesh::Real>
FunctionalExpansionSolidCartesianLegendre::sample(std::vector<libMesh::Point> & locations)
{
  std::vector<libMesh::Real> samples(locations.size(), 0);
  auto expansions = expand(locations);

  for (std::size_t index = 0; index < locations.size(); ++index)
    for (auto& term : expansions[index])
      samples[index] += term;

  return samples;
}
