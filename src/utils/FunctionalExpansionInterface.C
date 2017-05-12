#include "FunctionalExpansionInterface.h"

// MOOSE includes
#include "MooseError.h"

FunctionalExpansionInterface::FunctionalExpansionInterface(unsigned int number_of_coefficients)
  : _coefficients(number_of_coefficients, 1),
    _expansion(number_of_coefficients, 0),
    _is_cache_valid(false),
    _location(0),
    _number_of_coefficients(number_of_coefficients)
{
  _coefficients.shrink_to_fit();
}

void
FunctionalExpansionInterface::setCoefficients(std::vector<libMesh::Real> && coefficients)
{
  mooseAssert(_coefficients.size() == coefficients.size(), "Mismatched size in coefficient assignment");

  if (_coefficients != coefficients)
  {
    _coefficients = coefficients;
    _is_cache_valid = false;
  }
}

void
FunctionalExpansionInterface::setCoefficients(std::vector<libMesh::Real> & coefficients)
{
  mooseAssert(_coefficients.size() == coefficients.size(), "Mismatched size in coefficient assignment");

  if (_coefficients != coefficients)
  {
    _coefficients = coefficients;
    _is_cache_valid = false;
  }
}
