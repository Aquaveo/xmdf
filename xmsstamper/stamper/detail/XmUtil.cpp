//------------------------------------------------------------------------------
/// \file
/// \ingroup stamping
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsstamper/stamper/detail/XmUtil.h>

// 3. Standard library headers
#include <cmath>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/misc/XmError.h>
#include <xmsinterp/geometry/geoms.h>
#include <xmsstamper/stamper/XmStamperIo.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//------------------------------------------------------------------------------
/// \brief Converts the left or right portion of a cross section data to
/// 3d point locations using the angle from the center line and the max x value
/// \param[in] a_cl The location on the centerline for these cross section points
/// \param[in] a_pts The cross section points
/// \param[in] a_maxX The max X value for this portion of the cross section
/// \param[in] a_angle The angle of the cross section relative to the center
/// line of the cross section
/// \param[out] a_3dpts (x,y,z) points that are filled by the this method
//------------------------------------------------------------------------------
void XmUtil::ConvertXsPointsTo3d(const Pt3d& a_cl,
                                 const VecPt2d& a_pts,
                                 double a_maxX,
                                 double a_angle,
                                 VecPt3d2d& a_3dpts)
{
  if (a_pts.size() < 2 || a_maxX <= 0.0)
    return;

  VecPt2d pts(a_pts);
  XmUtil::EnsureVectorAtMaxX(pts, a_maxX);

  a_3dpts.push_back(VecPt3d());
  VecPt3d& pts3d(a_3dpts.back());
  Pt3d p3d;
  for (size_t i = 1; i < pts.size(); ++i)
  {
    double x2d = pts[i].x;
    double z = pts[i].y;
    p3d.z = z;
    p3d.x = a_cl.x + x2d * cos(a_angle);
    p3d.y = a_cl.y + x2d * sin(a_angle);
    // add the point
    pts3d.push_back(p3d);
  }
} // XmUtil::ConvertXsPointsTo3d
//------------------------------------------------------------------------------
/// \brief Converts interpolated cross sections to 3d
/// \param[in,out] a_xs Cross section
/// \param[in] a_factor Value multiplied to each X val
//------------------------------------------------------------------------------
void XmUtil::ScaleCrossSectionXvals(XmStampCrossSection& a_xs, double a_factor)
{
  for (auto& v : a_xs.m_left)
    v.x *= a_factor;
  for (auto& v : a_xs.m_right)
    v.x *= a_factor;
  a_xs.m_leftMax *= a_factor;
  a_xs.m_rightMax *= a_factor;
} // XmUtil::ScaleCrossSectionXvals
//------------------------------------------------------------------------------
/// \brief Makes sure the cross section goes to the maxX value
/// \param[in,out] a_pts 2d points
/// \param[in] a_maxX Max x value
//------------------------------------------------------------------------------
void XmUtil::EnsureVectorAtMaxX(VecPt2d& a_pts, double a_maxX)
{
  if (a_pts.empty())
    return;
  if (a_pts.size() < 2)
  {
    a_pts[0].x = 0;
    a_pts.push_back(Pt2d(a_maxX, a_pts.front().y));
    return;
  }
  if (a_pts.back().x < a_maxX)
  {
    Pt2d p0(a_pts[a_pts.size() - 2]), p1(a_pts.back());
    double t = (a_maxX - p0.x) / (p1.x - p0.x);
    double z = p0.y + t * (p1.y - p0.y);
    a_pts.back() = Pt2d(a_maxX, z);
    return;
  }

  if (a_pts.front().x > a_maxX)
  {
    VecPt2d vp(2);
    vp[0] = Pt2d(0, a_pts.front().y);
    vp[1] = Pt2d(a_maxX, vp[0].y);
    a_pts.swap(vp);
    return;
  }

  // truncate to maxX
  bool done(false);
  for (size_t i = 1; !done && i < a_pts.size(); ++i)
  {
    if (a_pts[i].x > a_maxX)
    {
      Pt2d p0(a_pts[i - 1]), p1(a_pts[i]);
      double t = (a_maxX - p0.x) / (p1.x - p0.x);
      double z = p0.y + t * (p1.y - p0.y);
      Pt2d p3(Pt2d(a_maxX, z));
      a_pts.resize(i);
      a_pts.push_back(p3);
      done = true;
    }
    else if (a_pts[i].x == a_maxX)
    {
      a_pts.resize(i + 1);
      done = true;
    }
  }
} // XmUtil::EnsureVectorAtMaxX
//------------------------------------------------------------------------------
/// \brief Gets the angles of the left and right cross sections relative to
/// the centerline
/// \param[in] a_idx Index of the current cross section
/// \param[in] a_cl Array of locations defining center line
/// \param[out] a_leftAngle The angle of the left cross section relative to
/// the center line.
/// \param[out] a_rightAngle The angle of the right cross section relative to
/// the center line.
//------------------------------------------------------------------------------
void XmUtil::GetAnglesFromCenterLine(size_t a_idx,
                                     const VecPt3d& a_cl,
                                     double& a_leftAngle,
                                     double& a_rightAngle)
{
  a_leftAngle = a_rightAngle = 0;
  if (a_cl.size() < 2)
    return;

  XM_ENSURE_TRUE(a_cl.size() > a_idx);

  Pt3d p(a_cl[a_idx]);
  if (0 == a_idx)
  {
    Pt3d p1(a_cl[a_idx + 1]);
    a_leftAngle = gmPerpendicularAngle(p, p1);
    a_rightAngle = gmPerpendicularAngle(p1, p);
  }
  else if (a_cl.size() - 1 == a_idx)
  {
    Pt3d p1(a_cl[a_idx - 1]);
    a_leftAngle = gmPerpendicularAngle(p1, p);
    a_rightAngle = gmPerpendicularAngle(p, p1);
  }
  else
  {
    Pt3d p0(a_cl[a_idx - 1]);
    Pt3d p1(a_cl[a_idx + 1]);
    a_leftAngle = gmBisectingAngle(p1, p, p0);
    a_rightAngle = gmBisectingAngle(p0, p, p1);
  }
} // XmUtil::GetAnglesFromCenterLine

} // namespace xms

#ifdef CXX_TEST
//------------------------------------------------------------------------------
// Unit Tests
//------------------------------------------------------------------------------
using namespace xms;
#include <xmsstamper/stamper/detail/XmUtil.t.h>

#include <xmscore/testing/TestTools.h>

//------------------------------------------------------------------------------
/// \brief Tests XmUtilUnitTests
//------------------------------------------------------------------------------
void XmUtilUnitTests::test_EnsureVectorAtMaxX()
{
  VecPt2d pts, basePts;

  pts = {{5, 6}};
  XmUtil::EnsureVectorAtMaxX(pts, 3);
  basePts = {{0, 6}, {3, 6}};
  TS_ASSERT_EQUALS_VEC(basePts, pts);

  pts = {{5, 6}, {6, 7}, {7, 8}};
  XmUtil::EnsureVectorAtMaxX(pts, 3);
  basePts = {{0, 6}, {3, 6}};
  TS_ASSERT_EQUALS_VEC(basePts, pts);

  pts = {{5, 6}, {6, 7}, {7, 8}};
  XmUtil::EnsureVectorAtMaxX(pts, 6);
  basePts = {{5, 6}, {6, 7}};
  TS_ASSERT_EQUALS_VEC(basePts, pts);

  pts = {{0, 6}, {5, 6}, {15, 0}};
  XmUtil::EnsureVectorAtMaxX(pts, 10);
  basePts = {{0, 6}, {5, 6}, {10, 3}};
  TS_ASSERT_EQUALS_VEC(basePts, pts);

  pts = {{0, 6}, {5, 6}, {15, 0}};
  XmUtil::EnsureVectorAtMaxX(pts, 20);
  basePts = {{0, 6}, {5, 6}, {20, -3}};
  TS_ASSERT_EQUALS_VEC(basePts, pts);
} // XmUtilUnitTests::test_EnsureVectorAtMaxX

#endif
