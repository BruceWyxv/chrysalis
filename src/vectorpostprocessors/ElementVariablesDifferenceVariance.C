#include "ElementVariablesDifferenceVariance.h"

// MOOSE includes
#include "MooseVariable.h"

// libMesh includes
#include "libmesh/quadrature.h"

template <>
InputParameters
validParams<ElementVariablesDifferenceVariance>()
{
  InputParameters params = validParams<ElementVectorPostprocessor>();

  params.addRequiredCoupledVar(
      "compare_a",
      "The first variable to evaluate the difference with, performed as \"compare_a - compare_b\"");
  params.addRequiredCoupledVar("compare_b",
                               "The second variable to evaluate the difference with, "
                               "performed as \"compare_a - compare_b\"");

  return params;
}

ElementVariablesDifferenceVariance::ElementVariablesDifferenceVariance(const InputParameters & parameters)
  : ElementVectorPostprocessor(parameters),
    _a(coupledValue("compare_a")),
    _b(coupledValue("compare_b")),
    _final_mean(declareVector("Mean")),
    _final_variance(declareVector("Variance"))
{
  _final_mean.resize(1);
  _final_variance.resize(1);
}

void
ElementVariablesDifferenceVariance::execute()
{
  // Algorithm adapted from https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Weighted_incremental_algorithm
  for (unsigned int qp = 0; qp < _qrule->n_points(); ++qp)
  {
    const Real difference = _a[qp] - _b[qp];
    const Real weight = _JxW[qp] * _coord[qp];
    const Real old_mean = _running_mean;

    _running_weight_sum += weight;
    _running_weight_sum_squares += weight * weight;

    _running_mean += (weight / _running_weight_sum) * (difference - old_mean);
    _running_sum_squares += weight * (difference - old_mean) * (difference - _running_mean);
  }
}

void
ElementVariablesDifferenceVariance::finalize()
{
  const unsigned int number_of_processors = _communicator.size();

  std::vector<Real> all_mean(number_of_processors, 0.0);
  std::vector<Real> all_sum_squares(number_of_processors, 0.0);
  std::vector<Real> all_weight_sum(number_of_processors, 0.0);
  std::vector<Real> all_weight_sum_squares(number_of_processors, 0.0);

  if (number_of_processors > 1)
  {
    // Get everything from everyone
    MPI_Allgather(&_running_mean, 1, MPI_REAL, &all_mean[0], 1, MPI_REAL, _communicator.get());
    MPI_Allgather(&_running_sum_squares, 1, MPI_REAL, &all_sum_squares[0], 1, MPI_REAL, _communicator.get());
    MPI_Allgather(&_running_weight_sum, 1, MPI_REAL, &all_weight_sum[0], 1, MPI_REAL, _communicator.get());
    MPI_Allgather(&_running_weight_sum_squares, 1, MPI_REAL, &all_weight_sum_squares[0], 1, MPI_REAL, _communicator.get());

    // Combine everything
    for (std::size_t i = 1; i < number_of_processors; ++i)
      combineResults(all_mean[0], all_sum_squares[0], all_weight_sum[0], all_weight_sum_squares[0], all_mean[i], all_sum_squares[i], all_weight_sum[i], all_weight_sum_squares[i]);
  }

  // Use the reliability weight algorithm for calculating the variance
  _final_mean[0] = all_mean[0];
  _final_variance[0] = all_sum_squares[0] / (all_weight_sum[0] - all_weight_sum_squares[0] / all_weight_sum[0]);
}

void
ElementVariablesDifferenceVariance::initialize()
{
  _running_mean = 0;
  _running_sum_squares = 0;
  _running_weight_sum = 0;
  _running_weight_sum_squares = 0;
}

void
ElementVariablesDifferenceVariance::threadJoin(const UserObject & s)
{
  const ElementVariablesDifferenceVariance & sibling =
      static_cast<const ElementVariablesDifferenceVariance &>(s);

  combineResults(_running_mean,
                 _running_sum_squares,
                 _running_weight_sum,
                 _running_weight_sum_squares,
                 sibling._running_mean,
                 sibling._running_sum_squares,
                 sibling._running_weight_sum,
                 sibling._running_weight_sum_squares);
}

void
ElementVariablesDifferenceVariance::combineResults(Real & mean,
                                                   Real & sum_squares,
                                                   Real & weight_sum,
                                                   Real & weight_sum_squares,
                                                   const Real mean_other,
                                                   const Real sum_squares_other,
                                                   const Real weight_sum_other,
                                                   const Real weight_sum_squares_other)
{
  // Algorithm adapted from https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Parallel_algorithm
  const Real delta_mean = mean - mean_other;
  const Real total_weight = weight_sum + weight_sum_other;

  mean = ((weight_sum * mean) + (weight_sum_other * mean_other)) / total_weight;
  sum_squares += sum_squares_other + delta_mean * delta_mean * (weight_sum * weight_sum_other) / total_weight;
  weight_sum = total_weight;
  weight_sum_squares += weight_sum_squares_other;
}
