//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//============================================================================

#include <vtkm/cont/ErrorFilterExecution.h>
#include <vtkm/filter/flow/BoundsMap.h>
#include <vtkm/filter/flow/Pathline.h>

#include <vtkm/filter/flow/DataSetIntegratorUnsteadyState.h>
#include <vtkm/filter/flow/ParticleAdvector.h>

namespace vtkm
{
namespace filter
{
namespace flow
{

VTKM_CONT vtkm::cont::PartitionedDataSet Pathline::DoExecutePartitions(
  const vtkm::cont::PartitionedDataSet& input)
{
  using DSIType = vtkm::filter::flow::DataSetIntegratorUnsteadyState;
  this->ValidateOptions();

  vtkm::filter::flow::BoundsMap boundsMap(input);
  auto dsi = this->CreateDataSetIntegrators(input, boundsMap, this->ResultType);

  vtkm::filter::flow::ParticleAdvector<DSIType> pav(
    boundsMap, dsi, this->UseThreadedAlgorithm, this->ResultType);

  return pav.Execute(this->NumberOfSteps, this->StepSize, this->Seeds);
}

}
}
} // namespace vtkm::filter::flow
