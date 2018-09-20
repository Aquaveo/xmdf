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
#include <xmscore/stl/vector.h>

// 5. Shared code headers

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Forward declarations ---------------------------------------------------
class XmStampCrossSection;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class XmUtil
/// \brief Utility functions
class XmUtil
{
public:
  XmUtil();
  virtual ~XmUtil();

  static void ConvertXsPointsTo3d(const Pt3d& a_cl,
                                  const VecPt2d& a_pts,
                                  double a_maxX,
                                  double a_angle,
                                  VecPt3d2d& a_3dpts);
  static void EnsureVectorAtMaxX(VecPt2d& a_pts, double a_maxX);
  static void GetAnglesFromCenterLine(size_t a_idx,
                                      const VecPt3d& a_cl,
                                      double& a_leftAngle,
                                      double& a_rightAngle);
  static void ScaleCrossSectionXvals(XmStampCrossSection& a_xs, double a_factor);

  /// \cond

protected:
private:
  /// \endcond
}; // StSlopedAbutmentUtil

} // namespace xms
