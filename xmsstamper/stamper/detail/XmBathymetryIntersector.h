#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup stamping
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers

// 4. External library headers
#include <xmscore/misc/boost_defines.h>
#include <xmscore/misc/base_macros.h> // for XM_DISALLOW_COPY_AND_ASSIGN
#include <xmscore/stl/vector.h>       // VecPt3d

// 5. Shared code headers

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class XmStamper3dPts;
class XmStamperIo;
class TrTin;

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class XmBathymetryIntersector
/// \brief Intersects a feature stamp with a TIN
class XmBathymetryIntersector
{
public:
  static BSHP<XmBathymetryIntersector> New(BSHP<TrTin> a_tin, BSHP<TrTin> a_stamp);

  XmBathymetryIntersector();
  virtual ~XmBathymetryIntersector();

  /// \cond
  virtual void IntersectCenterLine(XmStamperIo& a_io) = 0;
  virtual void DecomposeCenterLine(XmStamperIo& a_io, std::vector<XmStamperIo>& a_vIo) = 0;
  virtual void IntersectXsects(XmStamper3dPts& a_pts) = 0;
  virtual void IntersectEndCaps(XmStamperIo& a_io, XmStamper3dPts& a_pts) = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(XmBathymetryIntersector);
  /// \endcond
}; // XmBathymetryIntersector

} // namespace xms
