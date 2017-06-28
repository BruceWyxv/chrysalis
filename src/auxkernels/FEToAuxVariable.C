#include "FEToAuxVariable.h"


// libmesh includes
#include "libmesh/quadrature.h"

template <>
InputParameters
validParams<FEToAuxVariable>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addRequiredParam<UserObjectName>(
      "fe_coefficients_user_object",
      "The FECoefficientsUserObject that will be expanded to the AuxVariable");

  params.addParam<bool>("print_evaluations", false, "Print the results of each evaluation");

  return params;
}

FEToAuxVariable::FEToAuxVariable(const InputParameters & parameters)
  : AuxKernel(parameters),
    _fe_evaluator(getUserObject<FECoefficientsUserObject>("fe_coefficients_user_object")),
    _print_evaluations(getParam<bool>("print_evaluations"))
{
  // Nothing here
}

Real
FEToAuxVariable::computeValue()
{
  const Point & point = isNodal() ? *_current_node : _q_point[_qp];
  const Real value = _fe_evaluator.spatialValue(point);

  if (_print_evaluations)
  {
    if (isNodal())
      _console << "Node " << _current_node->id() << " @ " << point << ":    " << value << std::endl;
    else
      _console << "(Element " << _current_elem->id() << ") qp " << _qp + 1 << " of " << _q_point.size() << " @ " << _q_point[_qp] << ":    " << value << std::endl;
  }
  
  return value;
}
