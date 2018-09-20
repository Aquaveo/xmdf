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
#include <xmscore/stl/vector.h>

// 5. Shared code headers

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Forward declarations ---------------------------------------------------
class XmStamperIo;
class XmStamper3dPts;
struct cs3dPtIdx;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class XmSlopedAbutmentUtil
/// \brief Converts sloped abutment to 3d points
class XmSlopedAbutmentUtil
{
public:
  static BSHP<XmSlopedAbutmentUtil> New();

  /// \cond
  virtual void DoConvertTo3d(bool a_first, XmStamperIo& a_io, XmStamper3dPts& a_3dpts) = 0;
  virtual void GetEndCapEndPoints(cs3dPtIdx& a_ptIdx,
                                  VecInt& a_firstEndCapEndPts,
                                  VecInt& a_lastEndCapEndPts,
                                  XmStamperIo& a_io) = 0;
  virtual void GetEndCapBreakLines(XmStamperIo& a_io, cs3dPtIdx& a_ptIdx, VecInt& a_blTypes) = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(XmSlopedAbutmentUtil);

  /// \endcond

protected:
  XmSlopedAbutmentUtil();
  virtual ~XmSlopedAbutmentUtil();

}; // XmSlopedAbutmentUtil

} // namespace xms
