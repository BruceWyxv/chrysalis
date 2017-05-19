#include "FunctionalExpansionSolidCartesianLegendre.h"

#include <iomanip>
#include <unordered_map>

FunctionalExpansionSolidCartesianLegendre::FunctionalExpansionSolidCartesianLegendre(unsigned int order_x,
                                                                                     unsigned int order_y,
                                                                                     unsigned int order_z)
  : FunctionalExpansionInterface((order_x + 1) * (order_y + 1) * (order_z + 1),
                                 {-1, 1, -1, 1, -1, 1}),
    _x(order_x),
    _y(order_y),
    _z(order_z)
{
  _bounds = {-1, 1, -1, 1, -1, 1};
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

const std::vector<Real> &
FunctionalExpansionSolidCartesianLegendre::expand(const Point & location)
{
  if (_location != location)
  {
    // Evaluate the dimensional polynomials at the defined locaton
    NormalizablePoint normalized(_bounds, _range, location);

    switch (_dimensionality)
    {
      case 3:
        _z.evaluateOrthonormal({{normalized(2)}});

      case 2:
        _y.evaluateOrthonormal({{normalized(1)}});

      default:
        _x.evaluateOrthonormal({{normalized(0)}});
    }

    _location = location;
    _is_cache_valid = false;
  }

  if (!_is_cache_valid)
  {
    switch (_dimensionality)
    {
      case 3:
      {
        // Loop over the dimensions and evaluate
        std::size_t index = 0;
        for (std::size_t x = 0; x <= _x.getOrder(); ++x)
          for (std::size_t y = 0; y <= _y.getOrder(); ++y)
            for (std::size_t z = 0; z <= _z.getOrder(); ++z)
            {
              _expansion[index] = _x[x] * _y[y] * _z[z];
              ++index;
            }
      }
      break;

      case 2:
      {
        // Loop over the dimensions and evaluate
        std::size_t index = 0;
        for (std::size_t x = 0; x <= _x.getOrder(); ++x)
          for (std::size_t y = 0; y <= _y.getOrder(); ++y)
          {
            _expansion[index] = _x[x] * _y[y];
            ++index;
          }
      }
      break;

      default:
      {
        // Loop over the dimensions and evaluate
        for (std::size_t x = 0; x <= _x.getOrder(); ++x)
          _expansion[x] = _x[x];
      }
      break;
    }

    _is_cache_valid = true;
  }

  return _expansion;
}

std::vector< std::vector<Real> >
FunctionalExpansionSolidCartesianLegendre::expand(const std::vector<const Point *> & locations)
{
  std::vector< std::vector<Real> > expansions;
  expansions.reserve(locations.size());

  std::unordered_map< Real, std::vector<Real> > x_evaluations;
  std::unordered_map< Real, std::vector<Real> > y_evaluations;
  std::unordered_map< Real, std::vector<Real> > z_evaluations;

  /*
   * Perform an first loop to precompute all the needed expansions. This is
   * advantageous if multiple any coordinates in any dimension are repeated, and
   * adds very little overhead in any other situation.
   */
  for (auto location : locations)
  {
    NormalizablePoint normalized(_bounds, _range, *location);
    if (x_evaluations.count((*location)(0)) == 0)
      x_evaluations.emplace((*location)(0), _x.evaluateOrthonormal({{normalized(0)}}));

    if (_dimensionality > 1 && y_evaluations.count((*location)(1)) == 0)
      y_evaluations.emplace((*location)(1), _y.evaluateOrthonormal({{normalized(1)}}));

    if (_dimensionality > 2 && z_evaluations.count((*location)(2)) == 0)
      z_evaluations.emplace((*location)(2), _z.evaluateOrthonormal({{normalized(2)}}));
  }

  // Perform the actual expansions in the second loop
  std::vector<Real> expansion(_number_of_coefficients, 0);
  for (unsigned int l = 0; l < locations.size(); ++l)
  {
    const Point & location = *locations[l];

    switch (_dimensionality)
    {
      case 3:
      {
        const std::vector<Real> & x_series = x_evaluations.at(location(0));
        const std::vector<Real> & y_series = y_evaluations.at(location(1));
        const std::vector<Real> & z_series = z_evaluations.at(location(2));

        // Loop over the dimensions and evaluate
        std::size_t index = 0;
        for (std::size_t x = 0; x <= _x.getOrder(); ++x)
          for (std::size_t y = 0; y <= _y.getOrder(); ++y)
            for (std::size_t z = 0; z <= _z.getOrder(); ++z)
            {
              expansion[index] = x_series[x] * y_series[y] * z_series[z];
              ++index;
            }
      }
      break;

      case 2:
      {
        const std::vector<Real> & x_series = x_evaluations.at(location(0));
        const std::vector<Real> & y_series = y_evaluations.at(location(1));

        // Loop over the dimensions and evaluate
        std::size_t index = 0;
        for (std::size_t x = 0; x <= _x.getOrder(); ++x)
          for (std::size_t y = 0; y <= _y.getOrder(); ++y)
          {
            expansion[index] = x_series[x] * y_series[y];
            ++index;
          }
      }
      break;

      default:
      {
        const std::vector<Real> & x_series = x_evaluations.at(location(0));

        // Loop over the dimensions and evaluate
        for (std::size_t x = 0; x <= _x.getOrder(); ++x)
          expansion[x] = x_series[x];
      }
      break;
    }

    expansions.push_back(expansion);
  }

  return expansions;
}

std::string
FunctionalExpansionSolidCartesianLegendre::getDescription() const
{
  return "A 3D function expansion for solid geometries in a Cartesian\n"
         "    coordinate system using three Legendre polynomial series,\n"
         "    one for each dimension.\n";
}

std::string
FunctionalExpansionSolidCartesianLegendre::getFormattedCoefficients() const
{
  std::ostringstream formatted;

  formatted                << "-------------- Coefficients -----------------\n";
  formatted                << "             == Sub-indices ==\n"
                           << " == Index ==     x   y   z     === Value ===\n";

  std::size_t index = 0;
  for (std::size_t x = 0; x <= _x.getOrder(); ++x)
    for (std::size_t y = 0; y <= _y.getOrder(); ++y)
      for (std::size_t z = 0; z <= _z.getOrder(); ++z)
      {
        formatted          << "     " << std::setw(2) << index
                           <<        "        " << std::setw(3) << x
                           <<                   " " << std::setw(3) << y
                           <<                       " " << std::setw(3) << z
                           <<                           "     " << std::setw(12) << _coefficients[index]
                           << "\n";
        ++index;
      }

  formatted                << "---------------------------------------------\n";
  return formatted.str();
}

std::string
FunctionalExpansionSolidCartesianLegendre::getName() const
{
  return "FunctionalExpansionSolidCartesianLegendre";
}

Real
FunctionalExpansionSolidCartesianLegendre::sample(const Point & location)
{
  Real sample = 0.0;
  NormalizablePoint normalized(_bounds, _range, location);

  switch (_dimensionality)
  {
    case 3:
    {
      _x.evaluatePure({{normalized(0)}});
      _y.evaluatePure({{normalized(1)}});
      _z.evaluatePure({{normalized(2)}});

      // Loop over the dimensions and evaluate
      std::size_t index = 0;
      for (std::size_t x = 0; x <= _x.getOrder(); ++x)
        for (std::size_t y = 0; y <= _y.getOrder(); ++y)
          for (std::size_t z = 0; z <= _z.getOrder(); ++z)
          {
            sample += _x[x] * _y[y] * _z[z] * _coefficients[index];
            ++index;
          }
    }
    break;

    case 2:
    {
      _x.evaluatePure({{normalized(0)}});
      _y.evaluatePure({{normalized(1)}});

      // Loop over the dimensions and evaluate
      std::size_t index = 0;
      for (std::size_t x = 0; x <= _x.getOrder(); ++x)
        for (std::size_t y = 0; y <= _y.getOrder(); ++y)
        {
          sample += _x[x] * _y[y] * _coefficients[index];
          ++index;
        }
    }
    break;

    default:
    {
      _x.evaluatePure({{normalized(0)}});

      // Loop over the dimensions and evaluate
      for (std::size_t x = 0; x <= _x.getOrder(); ++x)
        sample += _x[x] * _coefficients[x];
    }
    break;
  }

  return sample;
}

std::vector<Real>
FunctionalExpansionSolidCartesianLegendre::sample(std::vector<const Point *> & locations)
{
  std::vector<Real> samples(locations.size(), 0);

  std::unordered_map< Real, std::vector<Real> > x_evaluations;
  std::unordered_map< Real, std::vector<Real> > y_evaluations;
  std::unordered_map< Real, std::vector<Real> > z_evaluations;

  /*
   * Perform an first loop to precompute all the needed expansions. This is
   * advantageous if multiple any coordinates in any dimension are repeated, and
   * adds very little overhead in any other situation.
   */
  for (auto location : locations)
  {
    NormalizablePoint normalized(_bounds, _range, *location);
    if (x_evaluations.count((*location)(0)) == 0)
      x_evaluations.emplace((*location)(0), _x.evaluatePure({{normalized(0)}}));

    if (_dimensionality > 1 && y_evaluations.count((*location)(1)) == 0)
      y_evaluations.emplace((*location)(1), _y.evaluatePure({{normalized(1)}}));

    if (_dimensionality > 2 && z_evaluations.count((*location)(2)) == 0)
      z_evaluations.emplace((*location)(2), _z.evaluatePure({{normalized(2)}}));
  }

  // Perform the actual sampling in the second loop
  for (unsigned int l = 0; l < locations.size(); ++l)
  {
    const Point & location = *locations[l];

    switch (_dimensionality)
    {
      case 3:
      {
        const std::vector<Real> & x_series = x_evaluations.at(location(0));
        const std::vector<Real> & y_series = y_evaluations.at(location(1));
        const std::vector<Real> & z_series = z_evaluations.at(location(2));

        // Loop over the dimensions and evaluate
        std::size_t index = 0;
        for (std::size_t x = 0; x <= _x.getOrder(); ++x)
          for (std::size_t y = 0; y <= _y.getOrder(); ++y)
            for (std::size_t z = 0; z <= _z.getOrder(); ++z)
            {
              samples[l] += x_series[x] * y_series[y] * z_series[z] * _coefficients[index];
              ++index;
            }
      }
      break;

      case 2:
      {
        const std::vector<Real> & x_series = x_evaluations.at(location(0));
        const std::vector<Real> & y_series = y_evaluations.at(location(1));

        // Loop over the dimensions and evaluate
        std::size_t index = 0;
        for (std::size_t x = 0; x <= _x.getOrder(); ++x)
          for (std::size_t y = 0; y <= _y.getOrder(); ++y)
          {
            samples[l] += x_series[x] * y_series[y] * _coefficients[index];
            ++index;
          }
      }
      break;

      default:
      {
        const std::vector<Real> & x_series = x_evaluations.at(location(0));

        // Loop over the dimensions and evaluate
        for (std::size_t x = 0; x <= _x.getOrder(); ++x)
          samples[l] += x_series[x] * _coefficients[x];
      }
      break;
    }
  }

  return samples;
}
