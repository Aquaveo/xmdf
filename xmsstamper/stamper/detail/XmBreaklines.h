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
#include <xmscore/stl/vector.h>
#include <xmscore/misc/base_macros.h> // for XM_DISALLOW_COPY_AND_ASSIGN

// 5. Shared code headers

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Forward declarations ---------------------------------------------------
class XmStamperIo;
struct cs3dPtIdx;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class XmBreaklines
/// \brief Creates breaklines for the stamp operation
class XmBreaklines
{
public:
  static BSHP<XmBreaklines> New();

  /// enumeration to identify types of breaklines
  enum { BL_CENTERLINE = 0, BL_XSECT, BL_SHOULDER, BL_END };

  /// \cond
  virtual bool CreateBreaklines(XmStamperIo& a_io, cs3dPtIdx& a_ptIdx, VecInt& a_blTypes) = 0;

  virtual const VecInt& GetOuterPolygon() = 0;

  virtual bool BreaklinesIntersect(const VecInt2d& a_bl, const VecPt3d& a_pts) = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(XmBreaklines);
  /// \endcond

protected:
  XmBreaklines();
  virtual ~XmBreaklines();
}; // XmBathemetryIntersector

} // namespace xms
