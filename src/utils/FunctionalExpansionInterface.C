#include "FunctionalExpansionInterface.h"

// MOOSE includes
#include "MooseError.h"

FunctionalExpansionInterface::FunctionalExpansionInterface(unsigned int number_of_coefficients,
                                                           const std::vector<Real> && range)
  : _range(range),
    _coefficients(number_of_coefficients, 1),
    _dimensionality(3),
    _expansion(number_of_coefficients, 0),
    _is_cache_valid(false),
    _location(0),
    _number_of_coefficients(number_of_coefficients),
    _orthonormalized_coefficients(false)
{
  _coefficients.shrink_to_fit();

  for (unsigned int i = 0; i < _range.size(); i += 2)
    if (_range[i] >= range[i + 1])
      mooseError("FunctionalExpansionInterface:\n"
                 "Cannot have a minimum value that is less than a maximum value for a normalized range");
}

std::string
FunctionalExpansionInterface::getInformation() const
{
  return "=== FunctionalExpansionInterface::" + getName() + " ===\n\n"
         + "Description:\n" + getDescription() + "\n\n"
         + getFormattedCoefficients()
         + "=== FunctionalExpansionInterface::" + getName() + " ===\n";
}

bool
FunctionalExpansionInterface::isInBounds(const Point & point)
{
  for (unsigned int b = 0, p = 0; b < _bounds.size(); b += 2, ++p)
    if (point(p) < _bounds[b] || _bounds[b + 1] < point(p))
      return false;

  return true;
}

void
FunctionalExpansionInterface::setBounds(const std::vector<Real> & bounds)
{
  if (bounds.size() != _dimensionality * 2)
    mooseError("FunctionalExpansionInterface:\n"
               "Mismatch size in bounds assignment");

  _bounds = bounds;
}

void
FunctionalExpansionInterface::setBounds(const std::vector<Real> && bounds)
{
  if (bounds.size() != _dimensionality * 2)
    mooseError("FunctionalExpansionInterface:\n"
               "Mismatch size in bounds assignment");

  _bounds = bounds;
}

void
FunctionalExpansionInterface::setCoefficients(const std::vector<Real> && coefficients, bool orthonormalized)
{
  if (_coefficients.size() != coefficients.size())
    mooseError("FunctionalExpansionInterface:\n"
               "Mismatched size in coefficient assignment");

  if (_coefficients != coefficients)
  {
    _coefficients = coefficients;
    _is_cache_valid = false;
  }

  _orthonormalized_coefficients = orthonormalized;
}

void
FunctionalExpansionInterface::setCoefficients(const std::vector<Real> & coefficients, bool orthonormalized)
{
  if (_coefficients.size() != coefficients.size())
    mooseError("FunctionalExpansionInterface:\n"
               "Mismatched size in coefficient assignment");

  if (_coefficients != coefficients)
  {
    _coefficients = coefficients;
    _is_cache_valid = false;
  }

  _orthonormalized_coefficients = orthonormalized;
}

std::ostream &
operator<<(std::ostream & stream, FunctionalExpansionInterface const & me)
{
  return stream << me.getInformation();
}
