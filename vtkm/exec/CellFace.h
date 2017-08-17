//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2016 Sandia Corporation.
//  Copyright 2016 UT-Battelle, LLC.
//  Copyright 2016 Los Alamos National Security.
//
//  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================
#ifndef vtk_m_exec_CellFace_h
#define vtk_m_exec_CellFace_h

#include <vtkm/Assert.h>
#include <vtkm/CellShape.h>
#include <vtkm/Types.h>
#include <vtkm/exec/FunctorBase.h>
#include <vtkm/internal/Assume.h>

namespace vtkm
{
namespace exec
{

namespace detail
{

static const vtkm::IdComponent MAX_FACE_SIZE = 4;
static const vtkm::IdComponent MAX_NUM_FACES = 6;

VTKM_EXEC_CONSTANT
static const vtkm::IdComponent NumFaces[vtkm::NUMBER_OF_CELL_SHAPES] = {
  0, //  0: CELL_SHAPE_EMPTY
  0, //  1: CELL_SHAPE_VERTEX
  0, //  2: Unused
  0, //  3: CELL_SHAPE_LINE
  0, //  4: Unused
  0, //  5: CELL_SHAPE_TRIANGLE
  0, //  6: Unused
  0, //  7: CELL_SHAPE_POLYGON
  0, //  8: Unused
  0, //  9: CELL_SHAPE_QUAD
  4, // 10: CELL_SHAPE_TETRA
  0, // 11: Unused
  6, // 12: CELL_SHAPE_HEXAHEDRON
  5, // 13: CELL_SHAPE_WEDGE
  5  // 14: CELL_SHAPE_PYRAMID
};

VTKM_EXEC_CONSTANT
static const vtkm::IdComponent NumPointsInFace[vtkm::NUMBER_OF_CELL_SHAPES][MAX_NUM_FACES] = {
  { -1, -1, -1, -1, -1, -1 }, //  0: CELL_SHAPE_EMPTY
  { -1, -1, -1, -1, -1, -1 }, //  1: CELL_SHAPE_VERTEX
  { -1, -1, -1, -1, -1, -1 }, //  2: Unused
  { -1, -1, -1, -1, -1, -1 }, //  3: CELL_SHAPE_LINE
  { -1, -1, -1, -1, -1, -1 }, //  4: Unused
  { -1, -1, -1, -1, -1, -1 }, //  5: CELL_SHAPE_TRIANGLE
  { -1, -1, -1, -1, -1, -1 }, //  6: Unused
  { -1, -1, -1, -1, -1, -1 }, //  7: CELL_SHAPE_POLYGON
  { -1, -1, -1, -1, -1, -1 }, //  8: Unused
  { -1, -1, -1, -1, -1, -1 }, //  9: CELL_SHAPE_QUAD
  { 3, 3, 3, 3, -1, -1 },     // 10: CELL_SHAPE_TETRA
  { -1, -1, -1, -1, -1, -1 }, // 11: Unused
  { 4, 4, 4, 4, 4, 4 },       // 12: CELL_SHAPE_HEXAHEDRON
  { 3, 3, 4, 4, 4, -1 },      // 13: CELL_SHAPE_WEDGE
  { 4, 3, 3, 3, 3, -1 }       // 14: CELL_SHAPE_PYRAMID
};

VTKM_EXEC_CONSTANT
static const vtkm::IdComponent PointsInFace[vtkm::NUMBER_OF_CELL_SHAPES][MAX_NUM_FACES]
                                           [MAX_FACE_SIZE] = {
                                             //  0: CELL_SHAPE_EMPTY
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             //  1: CELL_SHAPE_VERTEX
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             //  2: Unused
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             //  3: CELL_SHAPE_LINE
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             //  4: Unused
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             //  5: CELL_SHAPE_TRIANGLE
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             //  6: Unused
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             //  7: CELL_SHAPE_POLYGON
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             //  8: Unused
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             //  9: CELL_SHAPE_QUAD
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             // 10: CELL_SHAPE_TETRA
                                             { { 0, 1, 3, -1 },
                                               { 1, 2, 3, -1 },
                                               { 2, 0, 3, -1 },
                                               { 0, 2, 1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             // 11: Unused
                                             { { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 },
                                               { -1, -1, -1, -1 } },
                                             // 12: CELL_SHAPE_HEXAHEDRON
                                             { { 0, 4, 7, 3 },
                                               { 1, 2, 6, 5 },
                                               { 0, 1, 5, 4 },
                                               { 3, 7, 6, 2 },
                                               { 0, 3, 2, 1 },
                                               { 4, 5, 6, 7 } },
                                             // 13: CELL_SHAPE_WEDGE
                                             { { 0, 1, 2, -1 },
                                               { 3, 5, 4, -1 },
                                               { 0, 3, 4, 1 },
                                               { 1, 4, 5, 2 },
                                               { 2, 5, 3, 0 },
                                               { -1, -1, -1, -1 } },
                                             // 14: CELL_SHAPE_PYRAMID
                                             { { 0, 3, 2, 1 },
                                               { 0, 1, 4, -1 },
                                               { 1, 2, 4, -1 },
                                               { 2, 3, 4, -1 },
                                               { 3, 0, 4, -1 },
                                               { -1, -1, -1, -1 } }
                                           };

} // namespace detail

template <typename CellShapeTag>
static inline VTKM_EXEC vtkm::IdComponent CellFaceNumberOfFaces(CellShapeTag shape,
                                                                const vtkm::exec::FunctorBase&)
{
  return detail::NumFaces[shape.Id];
}

template <typename CellShapeTag>
static inline VTKM_EXEC vtkm::IdComponent CellFaceNumberOfPoints(
  vtkm::IdComponent faceIndex,
  CellShapeTag shape,
  const vtkm::exec::FunctorBase& worklet)
{
  VTKM_ASSUME(faceIndex >= 0);
  VTKM_ASSUME(faceIndex < detail::MAX_NUM_FACES);
  if (faceIndex >= vtkm::exec::CellFaceNumberOfFaces(shape, worklet))
  {
    worklet.RaiseError("Invalid face number.");
    return 0;
  }

  return detail::NumPointsInFace[shape.Id][faceIndex];
}

template <typename CellShapeTag>
static inline VTKM_EXEC vtkm::UInt8 CellFaceShape(vtkm::IdComponent faceIndex,
                                                  CellShapeTag shape,
                                                  const vtkm::exec::FunctorBase& worklet)
{
  VTKM_ASSUME(faceIndex >= 0);
  VTKM_ASSUME(faceIndex < detail::MAX_NUM_FACES);
  switch (CellFaceNumberOfPoints(faceIndex, shape, worklet))
  {
    case 3:
      return vtkm::CELL_SHAPE_TRIANGLE;
    case 4:
      return vtkm::CELL_SHAPE_QUAD;
    default:
      return vtkm::CELL_SHAPE_POLYGON;
  }
}

template <typename CellShapeTag>
static inline VTKM_EXEC vtkm::VecCConst<vtkm::IdComponent> CellFaceLocalIndices(
  vtkm::IdComponent faceIndex,
  CellShapeTag shape,
  const vtkm::exec::FunctorBase& worklet)
{
  vtkm::IdComponent numPointsInFace = vtkm::exec::CellFaceNumberOfPoints(faceIndex, shape, worklet);
  if (numPointsInFace < 1)
  {
    // An invalid face. We should already have gotten an error from
    // CellFaceNumberOfPoints.
    return vtkm::VecCConst<vtkm::IdComponent>();
  }

  return vtkm::make_VecC(detail::PointsInFace[shape.Id][faceIndex], numPointsInFace);
}

/// \brief Returns a cononical identifer for a cell face
///
/// Given information about a cell face and the global point indices for that cell, returns a
/// vtkm::Id3 that contains values that are unique to that face. The values for two faces will be
/// the same if and only if the faces contain the same points.
///
/// Note that this property is only true if the mesh is conforming. That is, any two neighboring
/// cells that share a face have the same points on that face. This preculdes 2 faces sharing more
/// than a single point or single edge.
///
template <typename CellShapeTag, typename GlobalPointIndicesVecType>
static inline VTKM_EXEC vtkm::Id3 CellFaceCononicalId(
  vtkm::IdComponent faceIndex,
  CellShapeTag shape,
  const GlobalPointIndicesVecType& globalPointIndicesVec,
  const vtkm::exec::FunctorBase& worklet)
{
  vtkm::VecCConst<vtkm::IdComponent> localPointIndices =
    vtkm::exec::CellFaceLocalIndices(faceIndex, shape, worklet);

  VTKM_ASSERT(localPointIndices.GetNumberOfComponents() >= 3);

  //Sort the first 3 face points/nodes in ascending order
  vtkm::Id3 sorted(globalPointIndicesVec[localPointIndices[0]],
                   globalPointIndicesVec[localPointIndices[1]],
                   globalPointIndicesVec[localPointIndices[2]]);
  vtkm::Id temp;
  if (sorted[0] > sorted[2])
  {
    temp = sorted[0];
    sorted[0] = sorted[2];
    sorted[2] = temp;
  }
  if (sorted[0] > sorted[1])
  {
    temp = sorted[0];
    sorted[0] = sorted[1];
    sorted[1] = temp;
  }
  if (sorted[1] > sorted[2])
  {
    temp = sorted[1];
    sorted[1] = sorted[2];
    sorted[2] = temp;
  }

  // Check the rest of the points to see if they are in the lowest 3
  vtkm::IdComponent numPointsInFace = localPointIndices.GetNumberOfComponents();
  for (vtkm::IdComponent pointIndex = 3; pointIndex < numPointsInFace; pointIndex++)
  {
    vtkm::Id nextPoint = globalPointIndicesVec[localPointIndices[pointIndex]];
    if (nextPoint < sorted[2])
    {
      if (nextPoint < sorted[1])
      {
        sorted[2] = sorted[1];
        if (nextPoint < sorted[0])
        {
          sorted[1] = sorted[0];
          sorted[0] = nextPoint;
        }
        else // nextPoint > P0, nextPoint < P1
        {
          sorted[1] = nextPoint;
        }
      }
      else // nextPoint > P1, nextPoint < P2
      {
        sorted[2] = nextPoint;
      }
    }
    else // nextPoint > P2
    {
      // Do nothing. nextPoint not in top 3.
    }
  }

  return sorted;
}
}
} // namespace vtkm::exec

#endif //vtk_m_exec_CellFace_h
