#include "InterfaceDiffusion.h"

registerMooseObject("ChrysalisApp", InterfaceDiffusion);

#include <cmath>

template <>
InputParameters
validParams<InterfaceDiffusion>()
{
  InputParameters params = validParams<InterfaceKernel>();

  params.addParam<bool>("use_variable_side_material",
                        true,
                        "Use a material to get the diffusion coefficient on the variable side.");
  params.addRequiredParam<std::string>(
      "variable_side_diffusivity",
      "The diffusivity property on the variable side that will be used in the "
      "flux computation. This must be the name of a material property if "
      "'use_variable_side_material' is true (default), otherwise this must be "
      "a numeric value.");

  params.addParam<bool>("use_neighbor_side_material",
                        true,
                        "Use a material to get the diffusion coefficient on the neighbor side.");
  params.addParam<std::string>(
      "neighbor_side_diffusivity",
      "The diffusivity property on the neighbor side that will be used in the "
      "flux computation. This should be the name of a material property if "
      "'use_neighbor_side_material' is true (default), otherwise this should "
      "be a numeric value. If left blank then it will default to the parameter "
      "provided for 'variable_side_diffusivity'.");

  return params;
}

InterfaceDiffusion::InterfaceDiffusion(const InputParameters & parameters)
  : InterfaceKernel(parameters),
    _use_variable_side_material(getParam<bool>("use_variable_side_material")),
    _use_neighbor_side_material(getParam<bool>("use_neighbor_side_material")),
    _variable_diffusivity_input(getParam<std::string>("variable_side_diffusivity")),
    _neighbor_diffusivity_input(isParamValid("neighbor_side_diffusivity")
                                    ? getParam<std::string>("neighbor_side_diffusivity")
                                    : (_use_variable_side_material == _use_neighbor_side_material
                                           ? getParam<std::string>("variable_side_diffusivity")
                                           : "")),
    _variable_diffusion_coefficient_property(
        _use_variable_side_material ? &getMaterialProperty<Real>(_variable_diffusivity_input)
                                    : NULL),
    _neighbor_diffusion_coefficient_property(
        _use_neighbor_side_material
            ? &getNeighborMaterialProperty<Real>(_neighbor_diffusivity_input)
            : NULL),
    _variable_diffusion_coefficient(
        _use_variable_side_material ? 0.0 : atof(&_variable_diffusivity_input.front())),
    _neighbor_diffusion_coefficient(
        _use_neighbor_side_material ? 0.0 : atof(&_neighbor_diffusivity_input.front()))
{
  if (!parameters.isParamValid("boundary"))
    mooseError("In order to use the InterfaceDiffusion '",
               name(),
               "', you must specify a boundary where it will live.");

  if (!isParamValid("neighbor_side_diffusivity") &&
      _use_variable_side_material != _use_neighbor_side_material)
    mooseError("InterfaceKernel '",
               name(),
               "': cannot reuse the value from 'variable_side_diffusivity' in "
               "'neighbor_side_diffusivity'.\nThis is because the parameters "
               "'use_variable_side_material' and 'use_neighbor_side_material' "
               "are not identical."
               "\nPossible fixes:"
               "\n\t1) create an entry for 'neighbor_side_diffusivity' and supply "
               "a diffusivity constant manually"
               "\n\t2) change 'use_neighbor_side_material' to match "
               "'use_variable_side_material'");
}

Real
InterfaceDiffusion::computeQpResidual(Moose::DGResidualType type)
{
  if (_use_variable_side_material)
    _variable_diffusion_coefficient = (*_variable_diffusion_coefficient_property)[_qp];
  if (_use_neighbor_side_material)
    _neighbor_diffusion_coefficient = (*_neighbor_diffusion_coefficient_property)[_qp];

  Real r = 0.5 * (-_variable_diffusion_coefficient * _grad_u[_qp] * _normals[_qp] +
                  -_neighbor_diffusion_coefficient * _grad_neighbor_value[_qp] * _normals[_qp]);

  switch (type)
  {
    case Moose::Element:
      r *= _test[_i][_qp];
      break;

    case Moose::Neighbor:
      r *= -_test_neighbor[_i][_qp];
      break;
  }

  return r;
}

Real
InterfaceDiffusion::computeQpJacobian(Moose::DGJacobianType type)
{
  Real jac = 0;

  if (_use_variable_side_material)
    _variable_diffusion_coefficient = (*_variable_diffusion_coefficient_property)[_qp];
  if (_use_neighbor_side_material)
    _neighbor_diffusion_coefficient = (*_neighbor_diffusion_coefficient_property)[_qp];

  switch (type)
  {

    case Moose::ElementElement:
      jac -= 0.5 * _variable_diffusion_coefficient * _grad_phi[_j][_qp] * _normals[_qp] *
             _test[_i][_qp];
      break;

    case Moose::NeighborNeighbor:
      jac += 0.5 * _neighbor_diffusion_coefficient * _grad_phi_neighbor[_j][_qp] * _normals[_qp] *
             _test_neighbor[_i][_qp];
      break;

    case Moose::NeighborElement:
      jac += 0.5 * _variable_diffusion_coefficient * _grad_phi[_j][_qp] * _normals[_qp] *
             _test_neighbor[_i][_qp];
      break;

    case Moose::ElementNeighbor:
      jac -= 0.5 * _neighbor_diffusion_coefficient * _grad_phi_neighbor[_j][_qp] * _normals[_qp] *
             _test[_i][_qp];
      break;
  }

  return jac;
}
