//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2015 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//  Copyright 2015 UT-Battelle, LLC.
//  Copyright 2015 Los Alamos National Security.
//
//  Under the terms of Contract DE-NA0003525 with NTESS,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================

#include <vtkm/cont/DeviceAdapter.h>
#include <vtkm/cont/testing/MakeTestDataSet.h>
#include <vtkm/cont/testing/Testing.h>
#include <vtkm/rendering/Actor.h>
#include <vtkm/rendering/CanvasRayTracer.h>
#include <vtkm/rendering/MapperCylinder.h>
#include <vtkm/rendering/Scene.h>
#include <vtkm/rendering/View3D.h>
#include <vtkm/rendering/testing/RenderTest.h>

namespace
{

void RenderTests()
{
  typedef vtkm::rendering::MapperCylinder M;
  typedef vtkm::rendering::CanvasRayTracer C;
  typedef vtkm::rendering::View3D V3;
  typedef vtkm::rendering::View2D V2;

  vtkm::cont::testing::MakeTestDataSet maker;
  vtkm::cont::ColorTable colorTable("inferno");

  vtkm::rendering::testing::Render<M, C, V3>(
    maker.Make3DRegularDataSet0(), "pointvar", colorTable, "rt_reg3D.pnm");
  vtkm::rendering::testing::Render<M, C, V3>(
    maker.Make3DRectilinearDataSet0(), "pointvar", colorTable, "rt_rect3D.pnm");
  vtkm::rendering::testing::Render<M, C, V3>(
    maker.Make3DExplicitDataSet4(), "pointvar", colorTable, "rt_expl3D.pnm");

  vtkm::rendering::testing::Render<M, C, V2>(
    maker.Make2DUniformDataSet1(), "pointvar", colorTable, "uni2D.pnm");

  vtkm::rendering::testing::Render<M, C, V3>(
    maker.Make3DExplicitDataSet8(), "cellvar", colorTable, "cylinder.pnm");

  //hexahedron, wedge, pyramid, tetrahedron
  vtkm::rendering::testing::Render<M, C, V3>(
    maker.Make3DExplicitDataSet5(), "cellvar", colorTable, "rt_hex3d.pnm");

  M mapper;

  mapper.SetRadius(0.1f);
  vtkm::rendering::testing::Render<M, C, V3>(
    mapper, maker.Make3DExplicitDataSet8(), "cellvar", colorTable, "cyl_static_radius.pnm");

  mapper.UseVariableRadius(true);
  mapper.SetRadiusDelta(2.0f);
  vtkm::rendering::testing::Render<M, C, V3>(
    mapper, maker.Make3DExplicitDataSet8(), "cellvar", colorTable, "cyl_var_radius.pnm");

  //test to make sure can reset
  mapper.UseVariableRadius(false);
  vtkm::rendering::testing::Render<M, C, V3>(
    maker.Make3DExplicitDataSet8(), "cellvar", colorTable, "cylinder2.pnm");
}

} //namespace

int UnitTestMapperCylinders(int, char* [])
{
  vtkm::cont::GetGlobalRuntimeDeviceTracker().ForceDevice(VTKM_DEFAULT_DEVICE_ADAPTER_TAG());
  return vtkm::cont::testing::Testing::Run(RenderTests);
}
