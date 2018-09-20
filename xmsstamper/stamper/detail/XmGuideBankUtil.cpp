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
#include <xmsstamper/stamper/detail/XmGuideBankUtil.h>

// 3. Standard library headers
#include <cmath>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/misc/XmError.h>
#include <xmsinterp/geometry/geoms.h>
#include <xmsstamper/stamper/XmStamperIo.h>
#include <xmsstamper/stamper/detail/XmBreaklines.h>
#include <xmsstamper/stamper/detail/XmStamper3dPts.h>
#include <xmsstamper/stamper/detail/XmStampInterpCrossSection.h>
#include <xmsstamper/stamper/detail/XmUtil.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \brief Implementaion of XmGuideBankUtil
class XmGuideBankUtilImpl : public XmGuideBankUtil
{
public:
  XmGuideBankUtilImpl();
  ~XmGuideBankUtilImpl();

  virtual void DoConvertTo3d(bool a_first, XmStamperIo& a_io, XmStamper3dPts& a_3dpts) override;
  virtual void GetEndCapEndPoints(cs3dPtIdx& a_ptIdx,
                                  VecInt& a_firstEndCapEndPts,
                                  VecInt& a_lastEndCapEndPts,
                                  XmStamperIo& a_io) override;
  virtual void GetEndCapBreakLines(XmStamperIo& a_io,
                                   cs3dPtIdx& a_ptIdx,
                                   VecInt& a_blTypes) override;

  void GuideBankCenterLine();
  void AdjustEndCapCrossSection();
  void CrossSectionTo3dPts();
  void GuideBankEndCap();
  void BreakLinesAddCenterLine(XmStamperIo& a_io, cs3dPtIdx& a_ptIdx, VecInt& a_blTypes);
  void BreakLinesAddEndPoints(XmStamperIo& a_io, cs3dPtIdx& a_ptIdx, VecInt& a_blTypes);
  void BreakLinesAddCrossSections(XmStamperIo& a_io, cs3dPtIdx& a_ptIdx, VecInt& a_blTypes);
  void BreakLinesAddEndCap(XmStamperIo& a_io, cs3dPtIdx& a_ptIdx, VecInt& a_blTypes);
  void BreakLinesAddShoulders(XmStamperIo& a_io, cs3dPtIdx& a_ptIdx, VecInt& a_blTypes);

  bool m_first;            ///< flag indicating if this is from the first end of the stamp
  XmStamperIo* m_io;       ///< io class that has the stamping inputs
  XmStamper3dPts* m_3dpts; ///< class that holds the 3d points from the stamp coversion

  VecPt3d m_centerline;     ///< centerline for the guidebank
  XmStampCrossSection m_cs; ///< cross section for the guidebank
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmGuideBankUtilImpl
/// \brief Converts a sloped abutment to 3d points for feature stamping
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmGuideBankUtilImpl::XmGuideBankUtilImpl()
: m_first(true)
, m_io(nullptr)
, m_3dpts(nullptr)
{
} // XmGuideBankUtilImpl::XmGuideBankUtilImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmGuideBankUtilImpl::~XmGuideBankUtilImpl()
{
} // XmGuideBankUtilImpl::~XmGuideBankUtilImpl
//------------------------------------------------------------------------------
/// \brief Converts an end cap to 3d pts defining the geometry
/// \param[in] a_first - flag indicating if this is the first end cap
/// (otherwise it is the last endcap)
/// \param[in] a_io - stamper io class
/// \param[out] a_3dpts - points filled in by the method
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::DoConvertTo3d(bool a_first, XmStamperIo& a_io, XmStamper3dPts& a_3dpts)
{
  m_first = a_first;
  m_io = &a_io;
  m_3dpts = &a_3dpts;
  XM_ENSURE_TRUE(m_io->m_centerLine.size() > 1);

  GuideBankCenterLine();
  AdjustEndCapCrossSection();
  CrossSectionTo3dPts();
  GuideBankEndCap();

  // FillOutputs();
} // XmGuideBankUtilImpl::DoConvert
//------------------------------------------------------------------------------
/// \brief Creates the centerline for the guidebank
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::GuideBankCenterLine()
{
  // get the 2 points from the stamp centerline so we can get the angle of the
  // guidebank centerline
  Pt3d p0, p1;
  XmStamperEndCap cap;
  double angleFactor(1);
  if (m_first)
  {
    p0 = m_io->m_centerLine[0];
    p1 = m_io->m_centerLine[1];
    cap = m_io->m_firstEndCap;
    if (cap.m_guidebank.m_side == 0)
      angleFactor = -1.0; // LEFT SIDE
  }
  else
  {
    p0 = m_io->m_centerLine.back();
    size_t ix = m_io->m_centerLine.size() - 2;
    p1 = m_io->m_centerLine[ix];
    cap = m_io->m_lastEndCap;
    if (cap.m_guidebank.m_side == 1)
      angleFactor = -1.0; // RIGHT SIDE
  }
  double relMajorAngle = (90.0 + cap.m_angle) * angleFactor;
  double relMinorAngle = 90.0 * angleFactor;

  Pt3d vec = gmCreateVector(p1, p0);
  double mag(0), centerLineAngle(0);
  gmComponentMagnitudes(&vec.x, &vec.y, &mag, &centerLineAngle, true);

  // get the angle for the major axis of the guidebank
  double majorAngle = centerLineAngle + relMajorAngle;
  majorAngle = gmConvertAngleToBetween0And360(majorAngle);
  // get the angle for the minor axis of the guidebank
  double minorAngle = majorAngle + relMinorAngle;
  minorAngle = gmConvertAngleToBetween0And360(minorAngle);

  // calculate the locations of the centerline points

  // calculate dx and dy based on the minor axis
  XmGuidebank& gb(cap.m_guidebank);
  double dy(0), dx = gb.m_radius2 / (double)(gb.m_nPts - 1);
  double dx1, dy1;
  gmComponentMagnitudes(&dx1, &dy1, &dx, &minorAngle, false);
  double prevdx2(0), prevdy2(0.0);
  VecPt3d pts(gb.m_nPts, p0);
  for (int iPt = 1; iPt < gb.m_nPts; ++iPt)
  {
    // calculate the dx and dy based on the major axis
    dy = gb.m_radius1 * sqrt(1.0 - pow(gb.m_radius2 - dx * iPt, 2.0) / pow(gb.m_radius2, 2.0));
    double dx2(0), dy2(0);
    gmComponentMagnitudes(&dx2, &dy2, &dy, &majorAngle, false);

    pts[iPt].x = pts[iPt - 1].x + dx1 + (dx2 - prevdx2);
    pts[iPt].y = pts[iPt - 1].y + dy1 + (dy2 - prevdy2);
    prevdx2 = dx2;
    prevdy2 = dy2;
  }

  m_centerline = pts;
  pts.erase(pts.begin());
  if (m_first)
    m_3dpts->m_first_endcap.m_centerLine = pts;
  else
    m_3dpts->m_last_endcap.m_centerLine = pts;

} // XmGuideBankUtilImpl::GuideBankCenterLine
//------------------------------------------------------------------------------
/// \brief Creates a cross section to be used for the guide bank
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::AdjustEndCapCrossSection()
{
  XmStamperEndCap cap;
  XmStampCrossSection cs;
  if (m_first)
  {
    cap = m_io->m_firstEndCap;
    cs = m_io->m_cs[0];
    // we need to swap the sides of the cross section
    XmStampCrossSection c1 = cs;
    cs.m_idxLeftShoulder = c1.m_idxRightShoulder;
    cs.m_idxRightShoulder = c1.m_idxLeftShoulder;
    cs.m_left = c1.m_right;
    cs.m_right = c1.m_left;
    cs.m_leftMax = c1.m_rightMax;
    cs.m_rightMax = c1.m_leftMax;
  }
  else
  {
    cap = m_io->m_lastEndCap;
    cs = m_io->m_cs.back();
  }
  XmGuidebank& gb(cap.m_guidebank);
  // adjust the cross section for guide bank width
  double halfWidth = gb.m_width / 2;

  auto myLambda = [](double halfWidth, int idx, VecPt2d& v, double& maxX) {
    if (idx < (int)v.size())
    {
      double oldHalfWidth = v[idx].x;
      double factor = halfWidth / oldHalfWidth;
      for (int i = 0; i < idx; ++i)
      {
        v[i].x *= factor;
      }
      factor = oldHalfWidth - halfWidth;
      for (int i = idx; i < (int)v.size(); ++i)
      {
        v[i].x -= factor;
      }
      maxX -= factor;
      XmUtil::EnsureVectorAtMaxX(v, maxX);
    }
  };

  // adjust left
  myLambda(halfWidth, cs.m_idxLeftShoulder, cs.m_left, cs.m_leftMax);
  // adjust right
  myLambda(halfWidth, cs.m_idxRightShoulder, cs.m_right, cs.m_rightMax);
  m_cs = cs;
} // XmGuideBankUtilImpl::AdjustEndCapCrossSection
//------------------------------------------------------------------------------
/// \brief Creates a cross section to be used for the guide bank
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::CrossSectionTo3dPts()
{
  XmStampCrossSection& cs(m_cs);
  VecPt3d2d *left(&m_3dpts->m_first_endcap.m_left), *right(&m_3dpts->m_first_endcap.m_right);
  if (!m_first)
  {
    left = &m_3dpts->m_last_endcap.m_left;
    right = &m_3dpts->m_last_endcap.m_right;
  }
  for (size_t i = 1; i < m_centerline.size(); ++i)
  {
    Pt3d& p(m_centerline[i]);
    double leftAngle, rightAngle;
    XmUtil::GetAnglesFromCenterLine(i, m_centerline, leftAngle, rightAngle);
    XmUtil::ConvertXsPointsTo3d(p, cs.m_left, cs.m_leftMax, leftAngle, *left);
    XmUtil::ConvertXsPointsTo3d(p, cs.m_right, cs.m_rightMax, rightAngle, *right);
  }
} // XmGuideBankUtilImpl::CrossSectionTo3dPts
//------------------------------------------------------------------------------
/// \brief Makes the cross sections for the end cap of the guide bank
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::GuideBankEndCap()
{
  XM_ENSURE_TRUE(m_centerline.size() > 1);
  // average the left and right side to make the cross section that extends
  // out along the center line of the guidebank
  XmStampCrossSection leftCs(m_cs), rightCs(m_cs);
  double leftLen = leftCs.m_leftMax;
  double rightLen = rightCs.m_rightMax;
  leftCs.m_idxRightShoulder = leftCs.m_idxLeftShoulder;
  leftCs.m_right = leftCs.m_left;
  leftCs.m_rightMax = leftCs.m_leftMax;
  rightCs.m_idxLeftShoulder = rightCs.m_idxRightShoulder;
  rightCs.m_left = rightCs.m_right;
  rightCs.m_leftMax = rightCs.m_rightMax;
  XmUtil::ScaleCrossSectionXvals(leftCs, 1 / leftCs.m_leftMax);
  XmUtil::ScaleCrossSectionXvals(rightCs, 1 / rightCs.m_rightMax);

  double factor(1);
  // get vector for end of center line
  size_t ix = m_centerline.size() - 2;
  Pt3d p0(m_centerline.back()), p1(m_centerline[ix]);
  Pt3d vec = gmCreateVector(p0, p1);
  double mag(0), clAngle(0);
  gmComponentMagnitudes(&vec.x, &vec.y, &mag, &clAngle, true);
  double startAngle = clAngle + (90.0 * factor);
  ;
  startAngle = gmConvertAngleToBetween0And360(startAngle);

  auto myLambda = [](VecPt3d& pts, VecPt2d& v, double angle, Pt3d& p0, double factor) {
    pts.assign(v.size() - 1, p0);
    for (size_t i = 1; i < v.size(); ++i)
    {
      Pt3d d;
      gmComponentMagnitudes(&d.x, &d.y, &v[i].x, &angle, false);
      pts[i - 1].x = p0.x - (d.x * factor);
      pts[i - 1].y = p0.y - (d.y * factor);
      pts[i - 1].z = v[i].y;
    }
  };

  // interpolate cross sections
  BSHP<XmStampInterpCrossSection> interp = XmStampInterpCrossSection::New();
  VecDbl vLen(11);
  std::vector<XmStampCrossSection> vCs(11);
  double incr = 1.0 / 12.0, percent(0);
  double angle = startAngle;
  // add to the left
  VecPt3d2d* leftpts3d(&m_3dpts->m_first_endcap.m_left);
  if (!m_first)
    leftpts3d = &m_3dpts->m_last_endcap.m_left;
  // interp the lengths and cross sections
  for (int i = 0; i < 11; ++i)
  {
    percent += incr;
    angle -= 15.0;
    angle = gmConvertAngleToBetween0And360(angle);

    vLen[i] = leftLen + percent * (leftLen - rightLen);
    interp->InterpCs(leftCs, rightCs, percent, vCs[i]);
    XmUtil::ScaleCrossSectionXvals(vCs[i], vLen[i]);

    VecPt3d pts;
    myLambda(pts, vCs[i].m_left, angle, p0, factor);
    leftpts3d->push_back(pts);
  }
} // XmGuideBankUtilImpl::GuideBankEndCap
//------------------------------------------------------------------------------
/// \brief creates a breakline representing the outer polygon of the stamp
/// \param[in] a_ptIdx The indexes of the points in the TIN
/// \param[out] a_firstEndCapEndPts first end cap ending points
/// \param[out] a_lastEndCapEndPts last end cap ending points
/// \param[in,out] a_io StamperIo class
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::GetEndCapEndPoints(cs3dPtIdx& a_ptIdx,
                                             VecInt& a_firstEndCapEndPts,
                                             VecInt& a_lastEndCapEndPts,
                                             XmStamperIo& a_io)
{
  if (a_io.m_lastEndCap.m_type == 0)
  { // last end cap
    {
      VecInt2d& vLeft(a_ptIdx.m_last_end_cap.m_left);
      auto it = vLeft.begin();
      auto end = vLeft.end();
      for (; it != end; ++it)
      {
        if (!it->empty())
          a_lastEndCapEndPts.push_back(it->back());
      }
    }
    {
      VecInt2d& vRight(a_ptIdx.m_last_end_cap.m_right);
      auto it = vRight.rbegin();
      auto end = vRight.rend();
      for (; it != end; ++it)
      {
        if (!it->empty())
          a_lastEndCapEndPts.push_back(it->back());
      }
    }
  }
  if (a_io.m_firstEndCap.m_type == 0)
  { // first end cap
    {
      VecInt2d& vRight(a_ptIdx.m_first_end_cap.m_right);
      auto it = vRight.begin();
      auto end = vRight.end();
      for (; it != end; ++it)
      {
        if (!it->empty())
          a_firstEndCapEndPts.push_back(it->back());
      }
    }
    {
      VecInt2d& vLeft(a_ptIdx.m_first_end_cap.m_left);
      auto it = vLeft.rbegin();
      auto end = vLeft.rend();
      for (; it != end; ++it)
      {
        if (!it->empty())
          a_firstEndCapEndPts.push_back(it->back());
      }
    }
  }
} // XmGuideBankUtilImpl::GuideBankEndCapEndPoints
//------------------------------------------------------------------------------
/// \brief breaklines from the end cap
/// \param[in,out] a_io StamperIo class
/// \param[in] a_ptIdx The indexes of the points in the TIN created by stamping
/// \param[out] a_blTypes The type of break line
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::GetEndCapBreakLines(XmStamperIo& a_io,
                                              cs3dPtIdx& a_ptIdx,
                                              VecInt& a_blTypes)
{
  BreakLinesAddCenterLine(a_io, a_ptIdx, a_blTypes);
  BreakLinesAddEndPoints(a_io, a_ptIdx, a_blTypes);
  BreakLinesAddCrossSections(a_io, a_ptIdx, a_blTypes);
  BreakLinesAddEndCap(a_io, a_ptIdx, a_blTypes);
  BreakLinesAddShoulders(a_io, a_ptIdx, a_blTypes);
} // XmGuideBankUtilImpl::GetEndCapBreakLines
//------------------------------------------------------------------------------
/// \brief breaklines from the center line
/// \param[in,out] a_io StamperIo class
/// \param[in] a_ptIdx The indexes of the points in the TIN created by stamping
/// \param[out] a_blTypes The type of break line
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::BreakLinesAddCenterLine(XmStamperIo& a_io,
                                                  cs3dPtIdx& a_ptIdx,
                                                  VecInt& a_blTypes)
{
  VecInt2d& b(a_io.m_outBreakLines);
  VecInt bl;
  bool firstIsGuideBank(a_io.m_firstEndCap.m_type == 0);
  bool lastIsGuideBank(a_io.m_lastEndCap.m_type == 0);
  if (firstIsGuideBank)
  {
    if (!a_ptIdx.m_first_end_cap.m_centerLine.empty())
    {
      bl.resize(0);
      bl.push_back(a_ptIdx.m_centerLine[0]);
      auto beg = a_ptIdx.m_first_end_cap.m_centerLine.begin();
      auto end = a_ptIdx.m_first_end_cap.m_centerLine.end();
      bl.insert(bl.end(), beg, end);
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_CENTERLINE);
    }
  }
  if (lastIsGuideBank)
  {
    if (!a_ptIdx.m_last_end_cap.m_centerLine.empty())
    {
      bl.resize(0);
      bl.push_back(a_ptIdx.m_centerLine.back());
      auto beg = a_ptIdx.m_last_end_cap.m_centerLine.begin();
      auto end = a_ptIdx.m_last_end_cap.m_centerLine.end();
      bl.insert(bl.end(), beg, end);
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_CENTERLINE);
    }
  }
} // XmGuideBankUtilImpl::BreakLinesAddCenterLine
//------------------------------------------------------------------------------
/// \brief breaklines the outer edge of the stamp
/// \param[in,out] a_io StamperIo class
/// \param[in] a_ptIdx The indexes of the points in the TIN created by stamping
/// \param[out] a_blTypes The type of break line
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::BreakLinesAddEndPoints(XmStamperIo& a_io,
                                                 cs3dPtIdx& a_ptIdx,
                                                 VecInt& a_blTypes)
{
  VecInt2d& b(a_io.m_outBreakLines);
  bool firstIsGuideBank(a_io.m_firstEndCap.m_type == 0);
  bool lastIsGuideBank(a_io.m_lastEndCap.m_type == 0);
  VecInt bl, firstEndCapEndPts, lastEndCapEndPts;
  GetEndCapEndPoints(a_ptIdx, firstEndCapEndPts, lastEndCapEndPts, a_io);

  {
    VecInt& cl(a_ptIdx.m_first_end_cap.m_centerLine);
    VecInt2d& vl(a_ptIdx.m_first_end_cap.m_left);
    if (firstIsGuideBank && !cl.empty())
    { // breaklines for the end points
      bl.resize(0);
      bl.push_back(a_ptIdx.m_xsPts.m_left.front().back());
      auto beg = firstEndCapEndPts.begin();
      auto end = beg + (a_ptIdx.m_first_end_cap.m_centerLine.size() - 1);
      bl.insert(bl.end(), beg, end);
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_END);

      if (vl.size() > cl.size())
      {
        bl.resize(0);
        beg = end - 1;
        end = beg + 14;
        bl.insert(bl.end(), beg, end);
        b.push_back(bl);
        a_blTypes.push_back(XmBreaklines::BL_END);
      }

      bl.resize(0);
      beg = end - 1;
      end = firstEndCapEndPts.end();
      bl.insert(bl.end(), beg, end);
      bl.push_back(a_ptIdx.m_xsPts.m_right.front().back());
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_END);
    }
  }
  {
    VecInt& cl(a_ptIdx.m_last_end_cap.m_centerLine);
    VecInt2d& vl(a_ptIdx.m_last_end_cap.m_left);
    if (lastIsGuideBank && !cl.empty())
    { // breaklines for the end points
      bl.resize(0);
      bl.push_back(a_ptIdx.m_xsPts.m_left.back().back());
      auto beg = lastEndCapEndPts.begin();
      auto end = beg + (a_ptIdx.m_last_end_cap.m_centerLine.size());
      bl.insert(bl.end(), beg, end);
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_END);

      if (vl.size() > cl.size())
      {
        bl.resize(0);
        beg = end - 1;
        end = beg + 13;
        bl.insert(bl.end(), beg, end);
        b.push_back(bl);
        a_blTypes.push_back(XmBreaklines::BL_END);
      }

      bl.resize(0);
      end = lastEndCapEndPts.end();
      beg = end - (a_ptIdx.m_last_end_cap.m_centerLine.size());
      bl.insert(bl.end(), beg, end);
      bl.push_back(a_ptIdx.m_xsPts.m_right.back().back());
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_END);
    }
  }
} // XmGuideBankUtilImpl::BreakLinesAddEndPoints
//------------------------------------------------------------------------------
/// \brief breaklines for cross sections
/// \param[in,out] a_io StamperIo class
/// \param[in] a_ptIdx The indexes of the points in the TIN created by stamping
/// \param[out] a_blTypes The type of break line
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::BreakLinesAddCrossSections(XmStamperIo& a_io,
                                                     cs3dPtIdx& a_ptIdx,
                                                     VecInt& a_blTypes)
{
  VecInt2d& b(a_io.m_outBreakLines);
  bool firstIsGuideBank(a_io.m_firstEndCap.m_type == 0);
  bool lastIsGuideBank(a_io.m_lastEndCap.m_type == 0);
  VecInt bl;
  if (firstIsGuideBank)
  { // breaklines for cross sections
    VecInt& v(a_ptIdx.m_first_end_cap.m_centerLine);
    for (size_t i = 0; i < v.size(); ++i)
    {
      bl.resize(0);
      // reverse add the left side
      {
        auto beg = a_ptIdx.m_first_end_cap.m_left[i].rbegin();
        auto end = a_ptIdx.m_first_end_cap.m_left[i].rend();
        bl.insert(bl.end(), beg, end);
      }
      // add centerline
      bl.push_back(v[i]);
      // add right side
      {
        auto beg = a_ptIdx.m_first_end_cap.m_right[i].begin();
        auto end = a_ptIdx.m_first_end_cap.m_right[i].end();
        bl.insert(bl.end(), beg, end);
      }
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_XSECT);
    }
  }
  if (lastIsGuideBank)
  { // breaklines for cross sections
    VecInt& v(a_ptIdx.m_last_end_cap.m_centerLine);
    for (size_t i = 0; i < v.size(); ++i)
    {
      bl.resize(0);
      // reverse add the left side
      {
        auto beg = a_ptIdx.m_last_end_cap.m_left[i].rbegin();
        auto end = a_ptIdx.m_last_end_cap.m_left[i].rend();
        bl.insert(bl.end(), beg, end);
      }
      // add centerline
      bl.push_back(v[i]);
      // add right side
      {
        auto beg = a_ptIdx.m_last_end_cap.m_right[i].begin();
        auto end = a_ptIdx.m_last_end_cap.m_right[i].end();
        bl.insert(bl.end(), beg, end);
      }
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_XSECT);
    }
  }
} // XmGuideBankUtilImpl::BreakLinesAddCrossSections
//------------------------------------------------------------------------------
/// \brief breaklines for end cap
/// \param[in,out] a_io StamperIo class
/// \param[in] a_ptIdx The indexes of the points in the TIN created by stamping
/// \param[out] a_blTypes The type of break line
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::BreakLinesAddEndCap(XmStamperIo& a_io,
                                              cs3dPtIdx& a_ptIdx,
                                              VecInt& a_blTypes)
{
  VecInt2d& b(a_io.m_outBreakLines);
  bool firstIsGuideBank(a_io.m_firstEndCap.m_type == 0);
  bool lastIsGuideBank(a_io.m_lastEndCap.m_type == 0);
  VecInt bl;
  if (firstIsGuideBank)
  {
    VecInt& v(a_ptIdx.m_first_end_cap.m_centerLine);
    VecInt2d& v1(a_ptIdx.m_first_end_cap.m_left);
    if (v1.size() > v.size())
    {
      // breaklines for the end cap
      for (size_t i = v.size(); i < v1.size(); ++i)
      {
        bl.resize(0);
        bl.push_back(v.back());
        auto beg = v1[i].begin();
        auto end = v1[i].end();
        bl.insert(bl.end(), beg, end);
        b.push_back(bl);
        a_blTypes.push_back(XmBreaklines::BL_XSECT);
      }
    }
  }
  if (lastIsGuideBank)
  {
    VecInt& v(a_ptIdx.m_last_end_cap.m_centerLine);
    VecInt2d& v1(a_ptIdx.m_last_end_cap.m_left);
    if (v1.size() > v.size())
    {
      // breaklines for the end cap
      for (size_t i = v.size(); i < v1.size(); ++i)
      {
        bl.resize(0);
        bl.push_back(v.back());
        auto beg = v1[i].begin();
        auto end = v1[i].end();
        bl.insert(bl.end(), beg, end);
        b.push_back(bl);
        a_blTypes.push_back(XmBreaklines::BL_XSECT);
      }
    }
  }
} // XmGuideBankUtilImpl::BreakLinesAddEndCap
//------------------------------------------------------------------------------
/// \brief breaklines for end cap
/// \param[in,out] a_io StamperIo class
/// \param[in] a_ptIdx The indexes of the points in the TIN created by stamping
/// \param[out] a_blTypes The type of break line
//------------------------------------------------------------------------------
void XmGuideBankUtilImpl::BreakLinesAddShoulders(XmStamperIo& a_io,
                                                 cs3dPtIdx& a_ptIdx,
                                                 VecInt& a_blTypes)
{
  VecInt2d& b(a_io.m_outBreakLines);
  bool firstIsGuideBank(a_io.m_firstEndCap.m_type == 0);
  bool lastIsGuideBank(a_io.m_lastEndCap.m_type == 0);
  VecInt bl;
  if (firstIsGuideBank && !a_ptIdx.m_first_end_cap.m_centerLine.empty())
  {
    VecInt& v(a_ptIdx.m_first_end_cap.m_centerLine);
    VecInt2d& v1(a_ptIdx.m_first_end_cap.m_left);
    VecInt2d& vR(a_ptIdx.m_first_end_cap.m_right);

    // Left and right are backwards for the first guide bank
    // breaklines for the shoulders
    int rShoulder(a_io.m_cs.front().m_idxLeftShoulder - 1),
      lShoulder(a_io.m_cs.front().m_idxRightShoulder - 1);
    // left side
    bl.resize(0);
    bl.push_back(a_ptIdx.m_xsPts.m_right.front()[lShoulder]);
    for (size_t i = 0; i < v.size(); ++i)
    {
      bl.push_back(v1[i][lShoulder]);
    }
    b.push_back(bl);
    a_blTypes.push_back(XmBreaklines::BL_SHOULDER);
    // end cap shoulder
    if (v1.size() > v.size())
    {
      bl.resize(0);
      for (size_t i = v.size() - 1; i < v1.size(); ++i)
      {
        bl.push_back(v1[i][lShoulder]);
      }
      bl.push_back(vR.back()[rShoulder]);
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_SHOULDER);
    }
    // right shoulder
    bl.resize(0);
    bl.push_back(a_ptIdx.m_xsPts.m_left.front()[rShoulder]);
    for (size_t i = 0; i < vR.size(); ++i)
    {
      bl.push_back(vR[i][rShoulder]);
    }
    b.push_back(bl);
    a_blTypes.push_back(XmBreaklines::BL_SHOULDER);
  }
  if (lastIsGuideBank && !a_ptIdx.m_last_end_cap.m_centerLine.empty())
  {
    VecInt& v(a_ptIdx.m_last_end_cap.m_centerLine);
    VecInt2d& v1(a_ptIdx.m_last_end_cap.m_left);
    VecInt2d& vR(a_ptIdx.m_last_end_cap.m_right);

    // breaklines for the shoulders
    int lShoulder(a_io.m_cs.back().m_idxLeftShoulder - 1),
      rShoulder(a_io.m_cs.back().m_idxRightShoulder - 1);
    // left side
    bl.resize(0);
    bl.push_back(a_ptIdx.m_xsPts.m_left.back()[lShoulder]);
    for (size_t i = 0; i < v.size(); ++i)
    {
      bl.push_back(v1[i][lShoulder]);
    }
    b.push_back(bl);
    a_blTypes.push_back(XmBreaklines::BL_SHOULDER);
    // end cap shoulder
    if (v1.size() > v.size())
    {
      bl.resize(0);
      for (size_t i = v.size() - 1; i < v1.size(); ++i)
      {
        bl.push_back(v1[i][lShoulder]);
      }
      bl.push_back(vR.back()[rShoulder]);
      b.push_back(bl);
      a_blTypes.push_back(XmBreaklines::BL_SHOULDER);
    }
    // right shoulder
    bl.resize(0);
    bl.push_back(a_ptIdx.m_xsPts.m_right.back()[rShoulder]);
    for (size_t i = 0; i < vR.size(); ++i)
    {
      bl.push_back(vR[i][rShoulder]);
    }
    b.push_back(bl);
    a_blTypes.push_back(XmBreaklines::BL_SHOULDER);
  }
} // XmGuideBankUtilImpl::BreakLinesAddShoulders

//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmGuideBankUtil::XmGuideBankUtil()
{
} // XmGuideBankUtil::XmGuideBankUtil
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmGuideBankUtil::~XmGuideBankUtil()
{
} // XmGuideBankUtil::~XmGuideBankUtil
//------------------------------------------------------------------------------
/// \brief Creates a XmStampInterpCrossSection class
/// \return XmGuideBankUtil.
//------------------------------------------------------------------------------
BSHP<XmGuideBankUtil> XmGuideBankUtil::New()
{
  BSHP<XmGuideBankUtil> p(new XmGuideBankUtilImpl());
  return p;
} // XmGuideBankUtil::New

} // namespace xms
