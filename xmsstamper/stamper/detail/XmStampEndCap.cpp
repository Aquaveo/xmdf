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
#include <xmsstamper/stamper/detail/XmStampEndCap.h>

// 3. Standard library headers

// 4. External library headers

// 5. Shared code headers
#include <xmscore/math/math.h>
#include <xmscore/misc/XmError.h>
//#include <xmscore/stl/set.h>
#include <xmsinterp/geometry/geoms.h>
//#include <xmscore/misc/Observer.h>
#include <xmscore/misc/xmstype.h>
#include <xmsstamper/stamper/detail/XmGuideBankUtil.h>
#include <xmsstamper/stamper/detail/XmSlopedAbutmentUtil.h>
#include <xmsstamper/stamper/detail/XmStamper3dPts.h>
#include <xmsstamper/stamper/XmStamperIo.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \brief Implementaion of XmStampEndCap
class XmStampEndCapImpl : public XmStampEndCap
{
public:
  XmStampEndCapImpl();
  ~XmStampEndCapImpl();

  virtual void RotateEndCapCrossSections(XmStamperIo& a_io,
                                         VecPt3d2d& a_left,
                                         VecPt3d2d& a_right) override;
  virtual void ConvertTo3dPts(XmStamperIo& a_io, XmStamper3dPts& a_3dpts) override;

  void AdjustCrossSection(bool a_first, XmStamperIo& a_cap, VecPt3d2d& a_left, VecPt3d2d& a_right);
  void RotateCrossSection(double a_angle, Pt3d& a_cl, VecPt3d& a_cs);
  void RotateCrossSectionForType(bool a_first,
                                 Pt3d& a_clPt,
                                 XmStamperEndCap& a_cap,
                                 VecPt3d& a_left,
                                 VecPt3d& a_right,
                                 int a_leftShoulderIdx,
                                 int a_rightShoulderIdx);
  void GuidebankRotate(bool a_first,
                       Pt3d& a_clPt,
                       XmStamperEndCap& a_cap,
                       VecPt3d& a_left,
                       VecPt3d& a_right,
                       int a_leftShoulderIdx,
                       int a_rightShoulderIdx);
  void SlopedAbutmentRotateShoulders(double a_capAngle, int a_shoulderIdx, VecPt3d& a_cs);
  void WingWallRotate(bool a_first,
                      double a_capAngle,
                      double a_wwAngle,
                      int a_shoulderIdx,
                      VecPt3d& a_cs);
  void EndCapTo3dPts(bool a_first, XmStamperIo& a_io, XmStamper3dPts& a_3dpts);
  void GuidebankEndCapTo3dPts(bool a_first, XmStamperIo& a_io, XmStamper3dPts& a_3dpts);
  void SlopedAbutmentEndCapTo3dPts(bool a_first, XmStamperIo& a_io, XmStamper3dPts& a_3dpts);
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmStampEndCapImpl
/// \brief end cap operations for feature stamping
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStampEndCapImpl::XmStampEndCapImpl()
{
} // XmStampEndCapImpl::XmStampEndCapImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStampEndCapImpl::~XmStampEndCapImpl()
{
} // XmStampEndCapImpl::~XmStampEndCapImpl
//------------------------------------------------------------------------------
/// \brief Rotates cross section points based on the angle specified for an
/// end cap
/// \param[in] a_io - stamper io class
/// \param[in,out] a_left - left side of cross section at end cap
/// \param[in,out] a_right - right side of cross section at end cap
//------------------------------------------------------------------------------
void XmStampEndCapImpl::RotateEndCapCrossSections(XmStamperIo& a_io,
                                                  VecPt3d2d& a_left,
                                                  VecPt3d2d& a_right)
{
  bool firstEndCap(true), lastEndCap(false);
  AdjustCrossSection(firstEndCap, a_io, a_left, a_right);
  AdjustCrossSection(lastEndCap, a_io, a_left, a_right);
} // XmStampEndCapImpl::RotateEndCapCrossSections
//------------------------------------------------------------------------------
/// \brief Rotates cross section points based on the angle specified for an
/// end cap
/// \param[in] a_io - stamper io class
/// \param[out] a_3dpts - 3d points for the stamp operation
//------------------------------------------------------------------------------
void XmStampEndCapImpl::ConvertTo3dPts(XmStamperIo& a_io, XmStamper3dPts& a_3dpts)
{
  bool firstEndCap(true), lastEndCap(false);
  EndCapTo3dPts(firstEndCap, a_io, a_3dpts);
  EndCapTo3dPts(lastEndCap, a_io, a_3dpts);
} // XmStampEndCapImpl::RotateEndCapCrossSections
//------------------------------------------------------------------------------
/// \brief Rotates cross section points based on the angle specified for an
/// end cap
/// \param[in] a_first - flag indicating if this is the first or last end cap
/// \param[in] a_io - stamper io class
/// \param[in,out] a_left - left side of cross section at end cap
/// \param[in,out] a_right - right side of cross section at end cap
//------------------------------------------------------------------------------
void XmStampEndCapImpl::AdjustCrossSection(bool a_first,
                                           XmStamperIo& a_io,
                                           VecPt3d2d& a_left,
                                           VecPt3d2d& a_right)
{
  XmStamperEndCap* cap(nullptr);
  int leftShoulderIdx(0), rightShoulderIdx(0);
  VecPt3d *left(nullptr), *right(nullptr);
  Pt3d clPt;
  if (a_first)
  {
    cap = &a_io.m_firstEndCap;
    left = &a_left[0];
    leftShoulderIdx = a_io.m_cs[0].m_idxLeftShoulder;
    right = &a_right[0];
    rightShoulderIdx = a_io.m_cs[0].m_idxRightShoulder;
    clPt = a_io.m_centerLine.front();
  }
  else
  {
    cap = &a_io.m_lastEndCap;
    left = &a_left.back();
    leftShoulderIdx = a_io.m_cs.back().m_idxLeftShoulder;
    right = &a_right.back();
    rightShoulderIdx = a_io.m_cs.back().m_idxRightShoulder;
    clPt = a_io.m_centerLine.back();
  }
  RotateCrossSection(cap->m_angle, clPt, *left);
  RotateCrossSection(cap->m_angle, clPt, *right);

  RotateCrossSectionForType(a_first, clPt, *cap, *left, *right, leftShoulderIdx, rightShoulderIdx);
} // XmStampEndCapImpl::AdjustCrossSection
//------------------------------------------------------------------------------
/// \brief Rotates cross section points based on the angle specified for an
/// end cap
/// \param[in] a_angle - angle in degrees for the end cap -45 to 45
/// \param[in] a_clPt - the center line point
/// \param[in,out] a_cs - 3d locations for cross section
//------------------------------------------------------------------------------
void XmStampEndCapImpl::RotateCrossSection(double a_angle, Pt3d& a_clPt, VecPt3d& a_cs)
{
  if (0.0 == a_angle || a_cs.empty())
    return;

  Pt3d p0(a_clPt);

  double oldMag(0), oldAngle(0);
  for (size_t i = 0; i < a_cs.size(); ++i)
  {
    Pt3d p1(a_cs[i]), p2(p1);
    // calculate the angle and magnitude
    p2.x = p1.x - p0.x;
    p2.y = p1.y - p0.y;
    gmComponentMagnitudes(&p2.x, &p2.y, &oldMag, &oldAngle, true);

    // new angle
    double newAngle = oldAngle + a_angle;
    // new magnitude
    double newMag = oldMag / cos(a_angle * (XM_PI / 180.0));

    // calculate new x,y points
    gmComponentMagnitudes(&p2.x, &p2.y, &newMag, &newAngle, false);
    a_cs[i].x = p2.x + p0.x;
    a_cs[i].y = p2.y + p0.y;
  }
} // XmStampEndCapImpl::RotateCrossSection
//------------------------------------------------------------------------------
/// \brief Rotates cross section points based on the angle specified for an
/// end cap
/// \param[in] a_first - flag indicating if this is the first or last end cap
/// \param[in] a_clPt - the center line point
/// \param[in] a_cap - end cap definition
/// \param[in,out] a_left - left side of cross section at end cap
/// \param[in, out] a_right - right side of cross section at end cap
/// \param[in] a_leftShoulderIdx - index in the left cross section of the
/// shoulder
/// \param[in] a_rightShoulderIdx - index in the right cross section of the
/// shoulder
//------------------------------------------------------------------------------
void XmStampEndCapImpl::RotateCrossSectionForType(bool a_first,
                                                  Pt3d& a_clPt,
                                                  XmStamperEndCap& a_cap,
                                                  VecPt3d& a_left,
                                                  VecPt3d& a_right,
                                                  int a_leftShoulderIdx,
                                                  int a_rightShoulderIdx)
{
  int t = a_cap.m_type;
  XM_ENSURE_TRUE(0 == t || 1 == t || 2 == t);

  if (0 == a_cap.m_type) // guidebank
  {
    GuidebankRotate(a_first, a_clPt, a_cap, a_left, a_right, a_leftShoulderIdx, a_rightShoulderIdx);
  }
  else if (1 == a_cap.m_type) // sloped abutment
  {
    SlopedAbutmentRotateShoulders(a_cap.m_angle, a_leftShoulderIdx, a_left);
    SlopedAbutmentRotateShoulders(a_cap.m_angle, a_rightShoulderIdx, a_right);
  }
  else
  {
    WingWallRotate(a_first, a_cap.m_angle, a_cap.m_wingWall.m_wingWallAngle, a_leftShoulderIdx,
                   a_left);
    WingWallRotate(!a_first, a_cap.m_angle, a_cap.m_wingWall.m_wingWallAngle, a_rightShoulderIdx,
                   a_right);
  }
} // XmStampEndCapImpl::RotateCrossSection
//------------------------------------------------------------------------------
/// \brief Rotates cross section points based on the angle specified for an
/// end cap
/// \param[in] a_first - flag indicating if this is the first or last end cap
/// \param[in] a_clPt - the center line point
/// \param[in] a_cap - end cap definition
/// \param[in,out] a_left - left side of cross section at end cap
/// \param[in, out] a_right - right side of cross section at end cap
/// \param[in] a_leftShoulderIdx - index in the left cross section of the
/// shoulder
/// \param[in] a_rightShoulderIdx - index in the right cross section of the
/// shoulder
//------------------------------------------------------------------------------
void XmStampEndCapImpl::GuidebankRotate(bool a_first,
                                        Pt3d& a_clPt,
                                        XmStamperEndCap& a_cap,
                                        VecPt3d& a_left,
                                        VecPt3d& a_right,
                                        int a_leftShoulderIdx,
                                        int a_rightShoulderIdx)
{
  XM_ENSURE_TRUE(static_cast<size_t>(a_leftShoulderIdx) < a_left.size());
  XM_ENSURE_TRUE(static_cast<size_t>(a_rightShoulderIdx) < a_right.size());
  // get cs width between the CL and shoulder
  Pt3d p0(a_clPt), p1(a_left[a_leftShoulderIdx - 1]), p2(a_right[a_rightShoulderIdx - 1]);
  double leftWidth = Mdist(p0.x, p0.y, p1.x, p1.y);
  double rightWidth = Mdist(p0.x, p0.y, p2.x, p2.y);
  bool gbLeft = a_cap.m_guidebank.m_side == 0;

  // switch angle if this is the first cs and guidebank is left side OR
  // last cs and guidebank is right side
  double gbWidth = a_cap.m_guidebank.m_width;
  double gbAngle = a_cap.m_angle;
  double rightAngle(0), leftAngle(0);
  if ((a_first && gbLeft) || (!a_first && !gbLeft))
  {
    rightAngle = atan(gbWidth / 2 / leftWidth) * 180.0 / XM_PI;
    leftAngle = -atan(gbWidth / 2 / rightWidth) * 180.0 / XM_PI;
  }
  else
  {
    leftAngle = atan(gbWidth / 2 / leftWidth) * 180.0 / XM_PI;
    rightAngle = -atan(gbWidth / 2 / rightWidth) * 180.0 / XM_PI;
  }

  double oldMag(0), oldAngle(0), newMag(0), newAngle(0);
  // rotate for the left guidebank
  for (size_t i = 0; i < a_left.size(); ++i)
  {
    p1 = a_left[i] - p0;
    gmComponentMagnitudes(&p1.x, &p1.y, &oldMag, &oldAngle, true);
    oldAngle -= gbAngle;
    oldMag *= cos(gbAngle * XM_PI / 180.0);

    newAngle = oldAngle + gbAngle + leftAngle;
    newMag = oldMag / cos((gbAngle + leftAngle) * XM_PI / 180.0);

    // calc new xy
    gmComponentMagnitudes(&p2.x, &p2.y, &newMag, &newAngle, false);

    a_left[i].x = p2.x + p0.x;
    a_left[i].y = p2.y + p0.y;
  }

  // rotate for right guidebank
  for (size_t i = 0; i < a_right.size(); ++i)
  {
    p1 = a_right[i] - p0;
    gmComponentMagnitudes(&p1.x, &p1.y, &oldMag, &oldAngle, true);
    oldAngle -= gbAngle;
    oldMag *= cos(gbAngle * XM_PI / 180.0);

    newAngle = oldAngle + gbAngle - rightAngle;
    newMag = oldMag / cos((gbAngle - rightAngle) * XM_PI / 180.0);

    // calc new xy
    gmComponentMagnitudes(&p2.x, &p2.y, &newMag, &newAngle, false);

    a_right[i].x = p2.x + p0.x;
    a_right[i].y = p2.y + p0.y;
  }

  // do more past the shoulders
  if ((a_first && gbLeft) || (!a_first && !gbLeft))
  {
    rightAngle = 45.0 - rightAngle;
    leftAngle = -45.0 - leftAngle;
  }
  else
  {
    leftAngle = 45.0 - leftAngle;
    rightAngle = -45.0 - rightAngle;
  }

  p0 = a_left[a_leftShoulderIdx - 1];
  // rotate left wing wall
  size_t start = static_cast<size_t>(a_leftShoulderIdx);
  for (size_t i = start; i < a_left.size(); ++i)
  {
    p1 = a_left[i] - p0;
    gmComponentMagnitudes(&p1.x, &p1.y, &oldMag, &oldAngle, true);
    oldAngle -= gbAngle;
    oldMag *= cos(gbAngle * XM_PI / 180.0);

    newAngle = oldAngle + gbAngle + leftAngle;
    newMag = oldMag / cos((gbAngle + leftAngle) * XM_PI / 180.0);

    // calc new xy
    gmComponentMagnitudes(&p2.x, &p2.y, &newMag, &newAngle, false);

    a_left[i].x = p2.x + p0.x;
    a_left[i].y = p2.y + p0.y;
  }

  p0 = a_right[a_rightShoulderIdx - 1];
  // rotate right wing wall
  start = static_cast<size_t>(a_rightShoulderIdx);
  for (size_t i = start; i < a_right.size(); ++i)
  {
    p1 = a_right[i] - p0;
    gmComponentMagnitudes(&p1.x, &p1.y, &oldMag, &oldAngle, true);
    oldAngle -= gbAngle;
    oldMag *= cos(gbAngle * XM_PI / 180.0);

    newAngle = oldAngle + gbAngle - rightAngle;
    newMag = oldMag / cos((gbAngle - rightAngle) * XM_PI / 180.0);

    // calc new xy
    gmComponentMagnitudes(&p2.x, &p2.y, &newMag, &newAngle, false);

    a_right[i].x = p2.x + p0.x;
    a_right[i].y = p2.y + p0.y;
  }
} // XmStampEndCapImpl::GuidebankRotate
//------------------------------------------------------------------------------
/// \brief Rotates cross section points based on the angle specified for an
/// end cap
/// \param[in] a_capAngle - angle in degrees for the end cap -45 to 45
/// \param[in] a_shoulderIdx - index in the cross section of the shoulder
/// \param[in,out] a_cs - 3d locations for cross section
//------------------------------------------------------------------------------
void XmStampEndCapImpl::SlopedAbutmentRotateShoulders(double a_capAngle,
                                                      int a_shoulderIdx,
                                                      VecPt3d& a_cs)
{
  if (0 == a_capAngle)
    return;
  XM_ENSURE_TRUE(static_cast<size_t>(a_shoulderIdx) < a_cs.size());
  Pt3d p0 = a_cs[a_shoulderIdx - 1];

  double oldMag(0), oldAngle(0);
  size_t start = static_cast<size_t>(a_shoulderIdx);
  for (size_t i = start; i < a_cs.size(); ++i)
  {
    Pt3d p1 = a_cs[i] - p0;
    gmComponentMagnitudes(&p1.x, &p1.y, &oldMag, &oldAngle, true);
    oldAngle -= a_capAngle;
    oldMag *= cos(a_capAngle * XM_PI / 180.0);

    // calc the new xy
    Pt3d p2;
    gmComponentMagnitudes(&p2.x, &p2.y, &oldMag, &oldAngle, false);

    a_cs[i].x = p2.x + p0.x;
    a_cs[i].y = p2.y + p0.y;
  }
} // XmStampEndCapImpl::SlopedAbutmentRotateShoulders
//------------------------------------------------------------------------------
/// \brief Rotates cross section points based on the angle specified for an
/// end cap
/// \param[in] a_first - flag indicating if this is the first or last end cap
/// \param[in] a_capAngle - angle in degrees for the end cap -45 to 45
/// \param[in] a_wwAngle - angle for wing wall
/// \param[in] a_shoulderIdx - index in the cross section of the shoulder
/// \param[in,out] a_cs - 3d locations for cross section
//------------------------------------------------------------------------------
void XmStampEndCapImpl::WingWallRotate(bool a_first,
                                       double a_capAngle,
                                       double a_wwAngle,
                                       int a_shoulderIdx,
                                       VecPt3d& a_cs)
{
  if (0 == a_capAngle && 0 == a_wwAngle)
    return;

  XM_ENSURE_TRUE_NO_ASSERT(static_cast<size_t>(a_shoulderIdx) < a_cs.size());

  if (a_first)
    a_wwAngle *= -1.0;
  Pt3d p0 = a_cs[a_shoulderIdx - 1];
  // rotate the wing wall
  double oldMag(0), oldAngle(0), newAngle(0), newMag(0);
  size_t start = static_cast<size_t>(a_shoulderIdx);
  for (size_t i = start; i < a_cs.size(); ++i)
  {
    Pt3d p1 = a_cs[i] - p0;
    gmComponentMagnitudes(&p1.x, &p1.y, &oldMag, &oldAngle, true);
    oldAngle -= a_capAngle;
    oldMag *= cos(a_capAngle * XM_PI / 180.0);
    // calc new angle and magnitude
    newAngle = oldAngle + a_capAngle + a_wwAngle;
    newMag = oldMag / cos((a_capAngle + a_wwAngle) * XM_PI / 180.0);
    // calc new xy
    Pt3d p2;
    gmComponentMagnitudes(&p2.x, &p2.y, &newMag, &newAngle, false);
    a_cs[i].x = p2.x + p0.x;
    a_cs[i].y = p2.y + p0.y;
  }
} // XmStampEndCapImpl::WingWallRotate
//------------------------------------------------------------------------------
/// \brief Converts an end cap to 3d pts defining the geometry
/// \param[in] a_first - flag indicating if this is the first end cap
/// (otherwise it is the last endcap)
/// \param[in] a_io - stamper io class
/// \param[out] a_3dpts - points filled in by the method
//------------------------------------------------------------------------------
void XmStampEndCapImpl::EndCapTo3dPts(bool a_first, XmStamperIo& a_io, XmStamper3dPts& a_3dpts)
{
  stXs3dPts* pts(&a_3dpts.m_first_endcap);
  XmStamperEndCap cap(a_io.m_firstEndCap);
  if (!a_first)
  {
    pts = &a_3dpts.m_last_endcap;
    cap = a_io.m_lastEndCap;
  }
  pts->m_left.resize(0);
  pts->m_right.resize(0);
  // do nothing for a wing wall
  if (cap.m_type == 2)
    return;
  // make sure the cap is a guidebank or a sloped abutment
  XM_ENSURE_TRUE(cap.m_type == 0 || cap.m_type == 1);

  if (cap.m_type == 0)
    GuidebankEndCapTo3dPts(a_first, a_io, a_3dpts);
  else
    SlopedAbutmentEndCapTo3dPts(a_first, a_io, a_3dpts);

} // XmStampEndCapImpl::EndCapTo3dPts
//------------------------------------------------------------------------------
/// \brief Converts an end cap to 3d pts defining the geometry
/// \param[in] a_first - flag indicating if this is the first end cap
/// (otherwise it is the last endcap)
/// \param[in] a_io - stamper io class
/// \param[out] a_3dpts - points filled in by the method
//------------------------------------------------------------------------------
void XmStampEndCapImpl::GuidebankEndCapTo3dPts(bool a_first,
                                               XmStamperIo& a_io,
                                               XmStamper3dPts& a_3dpts)
{
  BSHP<XmGuideBankUtil> gb = XmGuideBankUtil::New();
  gb->DoConvertTo3d(a_first, a_io, a_3dpts);
} // XmStampEndCapImpl::EndCapTo3dPts
//------------------------------------------------------------------------------
/// \brief Converts an end cap to 3d pts defining the geometry
/// \param[in] a_first - flag indicating if this is the first end cap
/// (otherwise it is the last endcap)
/// \param[in] a_io - stamper io class
/// \param[out] a_3dpts - points filled in by the method
//------------------------------------------------------------------------------
void XmStampEndCapImpl::SlopedAbutmentEndCapTo3dPts(bool a_first,
                                                    XmStamperIo& a_io,
                                                    XmStamper3dPts& a_3dpts)
{
  BSHP<XmSlopedAbutmentUtil> sa = XmSlopedAbutmentUtil::New();
  sa->DoConvertTo3d(a_first, a_io, a_3dpts);
} // XmStampEndCapImpl::SlopedAbutmentEndCapTo3dPts

//------------------------------------------------------------------------------
/// \brief Creates a XmStampInterpCrossSection class
/// \return shared pointer to XmStampEndCap.
//------------------------------------------------------------------------------
BSHP<XmStampEndCap> XmStampEndCap::New()
{
  BSHP<XmStampEndCap> interp(new XmStampEndCapImpl);
  return interp;
} // XmStampEndCap::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStampEndCap::XmStampEndCap()
{
} // XmStampEndCap::XmStampEndCap
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStampEndCap::~XmStampEndCap()
{
} // XmStampEndCap::~XmStampEndCap

} // namespace xms
