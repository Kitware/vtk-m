//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//============================================================================

#include <vtkm/CellShape.h>

#include <vtkm/cont/CellSetStructured.h>
#include <vtkm/cont/DataSet.h>
#include <vtkm/cont/serial/DeviceAdapterSerial.h>

#include <vtkm/exec/ConnectivityStructured.h>

#include <vtkm/cont/CoordinateSystem.h>
#include <vtkm/cont/testing/MakeTestDataSet.h>
#include <vtkm/cont/testing/Testing.h>

static void TwoDimRectilinearTest();
static void ThreeDimRectilinearTest();

void TestDataSet_Rectilinear()
{
  std::cout << std::endl;
  std::cout << "--TestDataSet_Rectilinear--" << std::endl << std::endl;

  TwoDimRectilinearTest();
  ThreeDimRectilinearTest();
}

static void TwoDimRectilinearTest()
{
  std::cout << "2D Rectilinear data set" << std::endl;
  vtkm::cont::testing::MakeTestDataSet testDataSet;

  vtkm::cont::DataSet dataSet = testDataSet.Make2DRectilinearDataSet0();

  vtkm::cont::CellSetStructured<2> cellSet;
  dataSet.GetCellSet(0).CopyTo(cellSet);

  VTKM_TEST_ASSERT(dataSet.GetNumberOfCellSets() == 1, "Incorrect number of cell sets");
  VTKM_TEST_ASSERT(dataSet.GetNumberOfFields() == 2, "Incorrect number of fields");
  VTKM_TEST_ASSERT(dataSet.GetNumberOfCoordinateSystems() == 1,
                   "Incorrect number of coordinate systems");
  VTKM_TEST_ASSERT(cellSet.GetNumberOfPoints() == 6, "Incorrect number of points");
  VTKM_TEST_ASSERT(cellSet.GetNumberOfCells() == 2, "Incorrect number of cells");

  // test various field-getting methods and associations
  try
  {
    dataSet.GetField("cellvar", vtkm::cont::Field::Association::CELL_SET);
  }
  catch (...)
  {
    VTKM_TEST_FAIL("Failed to get field 'cellvar' with Association::CELL_SET.");
  }

  try
  {
    dataSet.GetField("pointvar", vtkm::cont::Field::Association::POINTS);
  }
  catch (...)
  {
    VTKM_TEST_FAIL("Failed to get field 'pointvar' with ASSOC_POINT_SET.");
  }

  vtkm::Id numCells = cellSet.GetNumberOfCells();
  for (vtkm::Id cellIndex = 0; cellIndex < numCells; cellIndex++)
  {
    VTKM_TEST_ASSERT(cellSet.GetNumberOfPointsInCell(cellIndex) == 4,
                     "Incorrect number of cell indices");
    vtkm::IdComponent shape = cellSet.GetCellShape();
    VTKM_TEST_ASSERT(shape == vtkm::CELL_SHAPE_QUAD, "Incorrect element type.");
  }

  vtkm::exec::ConnectivityStructured<vtkm::TopologyElementTagCell, vtkm::TopologyElementTagPoint, 2>
    pointToCell = cellSet.PrepareForInput(vtkm::cont::DeviceAdapterTagSerial(),
                                          vtkm::TopologyElementTagCell(),
                                          vtkm::TopologyElementTagPoint());
  vtkm::exec::ConnectivityStructured<vtkm::TopologyElementTagPoint, vtkm::TopologyElementTagCell, 2>
    cellToPoint = cellSet.PrepareForInput(vtkm::cont::DeviceAdapterTagSerial(),
                                          vtkm::TopologyElementTagPoint(),
                                          vtkm::TopologyElementTagCell());

  vtkm::Id cells[2][4] = { { 0, 1, 4, 3 }, { 1, 2, 5, 4 } };
  for (vtkm::Id cellIndex = 0; cellIndex < 2; cellIndex++)
  {
    vtkm::Id4 pointIds = pointToCell.GetIndices(pointToCell.FlatToLogicalToIndex(cellIndex));
    for (vtkm::IdComponent localPointIndex = 0; localPointIndex < 4; localPointIndex++)
    {
      VTKM_TEST_ASSERT(pointIds[localPointIndex] == cells[cellIndex][localPointIndex],
                       "Incorrect point ID for cell");
    }
  }

  vtkm::Id expectedCellIds[6][4] = { { 0, -1, -1, -1 }, { 0, 1, -1, -1 }, { 1, -1, -1, -1 },
                                     { 0, -1, -1, -1 }, { 0, 1, -1, -1 }, { 1, -1, -1, -1 } };

  for (vtkm::Id pointIndex = 0; pointIndex < 6; pointIndex++)
  {
    vtkm::VecVariable<vtkm::Id, 4> retrievedCellIds =
      cellToPoint.GetIndices(cellToPoint.FlatToLogicalToIndex(pointIndex));
    VTKM_TEST_ASSERT(retrievedCellIds.GetNumberOfComponents() <= 4,
                     "Got wrong number of cell ids.");
    for (vtkm::IdComponent cellIndex = 0; cellIndex < retrievedCellIds.GetNumberOfComponents();
         cellIndex++)
    {
      VTKM_TEST_ASSERT(retrievedCellIds[cellIndex] == expectedCellIds[pointIndex][cellIndex],
                       "Incorrect cell ID for point");
    }
  }
}

static void ThreeDimRectilinearTest()
{
  std::cout << "3D Rectilinear data set" << std::endl;
  vtkm::cont::testing::MakeTestDataSet testDataSet;

  vtkm::cont::DataSet dataSet = testDataSet.Make3DRectilinearDataSet0();

  vtkm::cont::CellSetStructured<3> cellSet;
  dataSet.GetCellSet(0).CopyTo(cellSet);

  VTKM_TEST_ASSERT(dataSet.GetNumberOfCellSets() == 1, "Incorrect number of cell sets");

  VTKM_TEST_ASSERT(dataSet.GetNumberOfFields() == 2, "Incorrect number of fields");

  VTKM_TEST_ASSERT(dataSet.GetNumberOfCoordinateSystems() == 1,
                   "Incorrect number of coordinate systems");

  VTKM_TEST_ASSERT(cellSet.GetNumberOfPoints() == 18, "Incorrect number of points");

  VTKM_TEST_ASSERT(cellSet.GetNumberOfCells() == 4, "Incorrect number of cells");

  try
  {
    dataSet.GetField("cellvar", vtkm::cont::Field::Association::CELL_SET);
  }
  catch (...)
  {
    VTKM_TEST_FAIL("Failed to get field 'cellvar' with Association::CELL_SET.");
  }

  try
  {
    dataSet.GetField("pointvar", vtkm::cont::Field::Association::POINTS);
  }
  catch (...)
  {
    VTKM_TEST_FAIL("Failed to get field 'pointvar' with ASSOC_POINT_SET.");
  }

  vtkm::Id numCells = cellSet.GetNumberOfCells();
  for (vtkm::Id cellIndex = 0; cellIndex < numCells; cellIndex++)
  {
    VTKM_TEST_ASSERT(cellSet.GetNumberOfPointsInCell(cellIndex) == 8,
                     "Incorrect number of cell indices");
    vtkm::IdComponent shape = cellSet.GetCellShape();
    VTKM_TEST_ASSERT(shape == vtkm::CELL_SHAPE_HEXAHEDRON, "Incorrect element type.");
  }

  //Test regular connectivity.
  vtkm::exec::ConnectivityStructured<vtkm::TopologyElementTagCell, vtkm::TopologyElementTagPoint, 3>
    pointToCell = cellSet.PrepareForInput(vtkm::cont::DeviceAdapterTagSerial(),
                                          vtkm::TopologyElementTagCell(),
                                          vtkm::TopologyElementTagPoint());
  vtkm::Id expectedPointIds[8] = { 0, 1, 4, 3, 6, 7, 10, 9 };
  vtkm::Vec<vtkm::Id, 8> retrievedPointIds = pointToCell.GetIndices(vtkm::Id3(0));
  for (vtkm::IdComponent localPointIndex = 0; localPointIndex < 8; localPointIndex++)
  {
    VTKM_TEST_ASSERT(retrievedPointIds[localPointIndex] == expectedPointIds[localPointIndex],
                     "Incorrect point ID for cell");
  }

  vtkm::exec::ConnectivityStructured<vtkm::TopologyElementTagPoint, vtkm::TopologyElementTagCell, 3>
    cellToPoint = cellSet.PrepareForInput(vtkm::cont::DeviceAdapterTagSerial(),
                                          vtkm::TopologyElementTagPoint(),
                                          vtkm::TopologyElementTagCell());
  vtkm::Id retrievedCellIds[6] = { 0, -1, -1, -1, -1, -1 };
  vtkm::VecVariable<vtkm::Id, 6> expectedCellIds = cellToPoint.GetIndices(vtkm::Id3(0));
  VTKM_TEST_ASSERT(expectedCellIds.GetNumberOfComponents() <= 6,
                   "Got unexpected number of cell ids");
  for (vtkm::IdComponent localPointIndex = 0;
       localPointIndex < expectedCellIds.GetNumberOfComponents();
       localPointIndex++)
  {
    VTKM_TEST_ASSERT(expectedCellIds[localPointIndex] == retrievedCellIds[localPointIndex],
                     "Incorrect cell ID for point");
  }
}

int UnitTestDataSetRectilinear(int argc, char* argv[])
{
  return vtkm::cont::testing::Testing::Run(TestDataSet_Rectilinear, argc, argv);
}
