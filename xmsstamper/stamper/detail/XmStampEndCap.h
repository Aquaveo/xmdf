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
class XmStamperIo;
class XmStamper3dPts;

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class XmStampEndCap
/// \brief End caps for feature stamping
class XmStampEndCap
{
public:
  static BSHP<XmStampEndCap> New();

  XmStampEndCap();
  virtual ~XmStampEndCap();
  /// \cond
  virtual void RotateEndCapCrossSections(XmStamperIo& a_io,
                                         VecPt3d2d& a_left,
                                         VecPt3d2d& a_right) = 0;

  virtual void ConvertTo3dPts(XmStamperIo& a_io, XmStamper3dPts& a_3dpts) = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(XmStampEndCap);
  /// \endcond
}; // XmStampEndCap

} // namespace xms
