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
#include <xmsstamper/stamper/detail/XmSlopedAbutmentUtil.h>

// 3. Standard library headers

// 4. External library headers

// 5. Shared code headers
#include <xmscore/misc/XmError.h>
#include <xmsinterp/geometry/geoms.h>
#include <xmscore/misc/xmstype.h>
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
/// \brief Implementaion of stSlopedAbutment
class XmSlopedAbutmentUtilImpl : public XmSlopedAbutmentUtil
{
public:
  XmSlopedAbutmentUtilImpl();
  ~XmSlopedAbutmentUtilImpl();

  virtual void DoConvertTo3d(bool a_first, XmStamperIo& a_io, XmStamper3dPts& a_3dpts) override;
  virtual void GetEndCapEndPoints(cs3dPtIdx& a_ptIdx,
                                  VecInt& a_firstEndCapEndPts,
                                  VecInt& a_lastEndCapEndPts,
                                  XmStamperIo& a_io) override;
  virtual void GetEndCapBreakLines(XmStamperIo& a_io,
                                   cs3dPtIdx& a_ptIdx,
                                   VecInt& a_blTypes) override;

  /// \cond
  enum { SA_LEFT = 0, SA_RIGHT };
  /// \endcond
  bool Setup();
  bool SlopedAbutmentTo2dCrossSection();
  void EndCapCrossSectionShouldersTo2dCrossSection();
  void ComputeTransitionAngles();
  void InterpolateCrossSections(int a_side);
  void ConvertTo3d(int a_side);
  void EnsureCrossSectionAtMaxX(XmStampCrossSection& a_xs);

  bool m_first;             ///< flag indicating if this is from the first end of the stamp
  XmStamperIo* m_io;        ///< io class that has the stamping inputs
  XmStamper3dPts* m_3dpts;  ///< class that holds the 3d points from the stamp coversion
  XmStamperEndCap m_cap;    ///< copy of the end cap
  XmSlopedAbutment m_sa;    ///< copy of the sloped abutment
  XmStampCrossSection m_cs; ///< copy of the cross section at the end cap
  /// cross section made from m_cs but only includes points from the left
  /// shoulder out to the end of the left side
  XmStampCrossSection m_csLeft;
  /// cross section made from m_cs but only includes points from the right
  /// shoulder out to the end of the left side
  XmStampCrossSection m_csRight;
  /// the first of 2 points defining the direction of the center line at this end cap
  Pt3d m_pt1;
  /// the second of 2 points defining the direction of the center line at this end cap
  Pt3d m_pt2;
  /// a 2d cross section built from the sloped abutment info
  XmStampCrossSection m_saXsect;
  /// the direction of the center line at the end cap;
  double m_angleCenterLine = 0.0;
  /// transition angle between the sloped abutment and the left side of the
  /// cross section at the end cap
  double m_angleLeftTransition = 0.0;
  /// incremental angle to transition between sloped abutment and the left side
  /// of the cross section at the end cap. The target increment in 15 degrees.
  double m_angleLeftIncrement = 0.0;
  int m_nDivLeft = 0; ///< number of divisions for left side transition
  /// transition angle between the sloped abutment and the right side of the
  /// cross section at the end cap
  double m_angleRightTransition = 0.0;
  /// incremental angle to transition between sloped abutment and the left side
  /// of the cross section at the end cap. The target increment in 15 degrees.
  double m_angleRightIncrement = 0.0;
  int m_nDivRight = 0; ///< number of divisions for right side transition
  /// interpolated cross sections for the left side transition
  std::vector<XmStampCrossSection> m_interpLeft;
  VecDbl m_interpAnglesLeft; ///< angles for the interpolated left cross sections
  /// interpolated cross sections for the right side transition
  std::vector<XmStampCrossSection> m_interpRight;
  VecDbl m_interpAnglesRight; ///< angles for the interpolated left cross sections
  Pt3d m_leftShoulder3d;      ///< location of the left shoulder in 3d
  Pt3d m_rightShoulder3d;     ///< location of the right shoulder in 3d
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmSlopedAbutmentUtilImpl
/// \brief Converts a sloped abutment to 3d points for feature stamping
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmSlopedAbutmentUtilImpl::XmSlopedAbutmentUtilImpl()
: m_first(true)
, m_io(nullptr)
, m_3dpts(nullptr)
{
} // XmSlopedAbutmentUtilImpl::XmSlopedAbutmentUtilImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmSlopedAbutmentUtilImpl::~XmSlopedAbutmentUtilImpl()
{
} // XmSlopedAbutmentUtilImpl::~XmSlopedAbutmentUtilImpl
//------------------------------------------------------------------------------
/// \brief Converts an end cap to 3d pts defining the geometry
/// \param[in] a_first - flag indicating if this is the first end cap
/// (otherwise it is the last endcap)
/// \param[in] a_io - stamper io class
/// \param[out] a_3dpts - points filled in by the method
//------------------------------------------------------------------------------
void XmSlopedAbutmentUtilImpl::DoConvertTo3d(bool a_first,
                                             XmStamperIo& a_io,
                                             XmStamper3dPts& a_3dpts)
{
  m_first = a_first;
  m_io = &a_io;
  m_3dpts = &a_3dpts;

  XM_ENSURE_TRUE(a_io.m_centerLine.size() > 1);
  XM_ENSURE_TRUE(a_3dpts.m_xsPts.m_left.size() > 1);
  XM_ENSURE_TRUE(a_3dpts.m_xsPts.m_right.size() > 1);

  if (a_first)
    m_cap = a_io.m_firstEndCap;
  else
    m_cap = a_io.m_lastEndCap;
  XM_ENSURE_TRUE(m_cap.m_type == 1);
  m_sa = m_cap.m_slopedAbutment;
  XM_ENSURE_TRUE(m_sa.m_slope.size() > 1); // return if no slope defined

  Setup();
  InterpolateCrossSections(SA_LEFT);
  InterpolateCrossSections(SA_RIGHT);
  ConvertTo3d(SA_LEFT);
  ConvertTo3d(SA_RIGHT);
} // XmSlopedAbutmentUtilImpl::DoConvert
//------------------------------------------------------------------------------
/// \brief Sets up variables to perform the conversion
/// \return true on success.
//------------------------------------------------------------------------------
bool XmSlopedAbutmentUtilImpl::Setup()
{
  if (m_first)
  {
    m_cs = m_io->m_cs[0];
    m_pt1 = m_io->m_centerLine[0];
    m_pt2 = m_io->m_centerLine[1];

    m_leftShoulder3d = m_rightShoulder3d = m_pt1;
    size_t ix = static_cast<size_t>(m_cs.m_idxLeftShoulder);
    if (ix > 0)
    {
      ix--;
      XM_ENSURE_TRUE(!m_3dpts->m_xsPts.m_left.empty(), false);
      XM_ENSURE_TRUE(ix < m_3dpts->m_xsPts.m_left[0].size(), false);
      m_leftShoulder3d = m_3dpts->m_xsPts.m_left[0][ix];
    }
    ix = static_cast<size_t>(m_cs.m_idxRightShoulder);
    if (ix > 0)
    {
      ix--;
      XM_ENSURE_TRUE(!m_3dpts->m_xsPts.m_right.empty(), false);
      XM_ENSURE_TRUE(ix < m_3dpts->m_xsPts.m_right[0].size(), false);
      m_rightShoulder3d = m_3dpts->m_xsPts.m_right[0][ix];
    }
  }
  else
  {
    m_cs = m_io->m_cs.back();
    m_pt2 = m_io->m_centerLine.back();
    size_t ix = m_io->m_centerLine.size() - 2;
    m_pt1 = m_io->m_centerLine[ix];

    m_leftShoulder3d = m_rightShoulder3d = m_pt2;
    ix = static_cast<size_t>(m_cs.m_idxLeftShoulder);
    if (ix > 0)
    {
      ix--;
      XM_ENSURE_TRUE(!m_3dpts->m_xsPts.m_left.empty(), false);
      XM_ENSURE_TRUE(ix < m_3dpts->m_xsPts.m_left.back().size(), false);
      m_leftShoulder3d = m_3dpts->m_xsPts.m_left.back()[ix];
    }
    ix = static_cast<size_t>(m_cs.m_idxRightShoulder);
    if (ix > 0)
    {
      ix--;
      XM_ENSURE_TRUE(!m_3dpts->m_xsPts.m_right.empty(), false);
      XM_ENSURE_TRUE(ix < m_3dpts->m_xsPts.m_right.back().size(), false);
      m_rightShoulder3d = m_3dpts->m_xsPts.m_right.back()[ix];
    }
  }
  EndCapCrossSectionShouldersTo2dCrossSection();
  XM_ENSURE_TRUE(SlopedAbutmentTo2dCrossSection(), false);
  ComputeTransitionAngles();
  return true;
} // XmSlopedAbutmentUtilImpl::Setup
//------------------------------------------------------------------------------
/// \brief Sets up variables to perform the conversion
/// \return true on success.
//------------------------------------------------------------------------------
bool XmSlopedAbutmentUtilImpl::SlopedAbutmentTo2dCrossSection()
{
  m_saXsect.m_idxLeftShoulder = m_saXsect.m_idxRightShoulder = 0;
  m_saXsect.m_leftMax = m_saXsect.m_rightMax = m_sa.m_maxX;
  VecPt2d pts(m_sa.m_slope);
  XM_ENSURE_TRUE(pts.size() > 1, false);
  // extend to the max x
  double x1 = pts[pts.size() - 2].x;
  double x2 = pts.back().x;
  double x3 = m_sa.m_maxX;
  if (x3 > x2)
  {
    double z1 = pts[pts.size() - 2].y;
    double z2 = pts.back().y;
    double z3 = ((x3 - x1) / (x2 - x1) * (z2 - z1)) + z1;
    Pt2d p(x3, z3);
    pts.back() = p;
  }
  m_saXsect.m_left = m_saXsect.m_right = pts;
  return true;
} // XmSlopedAbutmentUtilImpl::SlopedAbutmentTo2dCrossSection
//------------------------------------------------------------------------------
/// \brief Makes 2d cross sections from the end cap cross section but only
/// includes the points from the shoulder out to the end of the cross section
//------------------------------------------------------------------------------
void XmSlopedAbutmentUtilImpl::EndCapCrossSectionShouldersTo2dCrossSection()
{
  m_csLeft = XmStampCrossSection();
  size_t ix = static_cast<size_t>(m_cs.m_idxLeftShoulder);
  double x = m_cs.m_left[ix].x;
  for (size_t i = ix; i < m_cs.m_left.size(); ++i)
  {
    m_csLeft.m_left.push_back(m_cs.m_left[i]);
    m_csLeft.m_left.back().x -= x;
  }
  m_csLeft.m_leftMax = m_cs.m_leftMax - x;

  ix = static_cast<size_t>(m_cs.m_idxRightShoulder);
  x = m_cs.m_right[ix].x;
  for (size_t i = ix; i < m_cs.m_right.size(); ++i)
  {
    m_csRight.m_right.push_back(m_cs.m_right[i]);
    m_csRight.m_right.back().x -= x;
  }
  m_csRight.m_rightMax = m_cs.m_rightMax - x;
} // XmSlopedAbutmentUtilImpl::EndCapCrossSectionShouldersTo2dCrossSection
//------------------------------------------------------------------------------
/// \brief Sets up variables to perform the conversion
//------------------------------------------------------------------------------
void XmSlopedAbutmentUtilImpl::ComputeTransitionAngles()
{
  Pt3d vector = gmCreateVector(m_pt2, m_pt1);
  double mag;
  gmComponentMagnitudes(&vector.x, &vector.y, &mag, &m_angleCenterLine, true);
  m_angleCenterLine += m_cap.m_angle;

  if (m_first)
  {
    m_angleLeftTransition = 90.0 + m_cap.m_angle;
    m_angleRightTransition = 90.0 - m_cap.m_angle;
  }
  else
  {
    m_angleLeftTransition = 90.0 - m_cap.m_angle;
    m_angleRightTransition = 90.0 + m_cap.m_angle;
  }
  m_nDivLeft = static_cast<int>(m_angleLeftTransition / 15.0);
  m_angleLeftIncrement = m_angleLeftTransition / m_nDivLeft;
  m_nDivRight = static_cast<int>(m_angleRightTransition / 15.0);
  m_angleRightIncrement = m_angleRightTransition / m_nDivRight;
} // XmSlopedAbutmentUtilImpl::ComputeTransitionAngles
//------------------------------------------------------------------------------
/// \brief Interpolates cross sections to define the transition between the
/// sloped abutment and the end cap cross section from the shoulder out to the
/// end of the cross section
/// \param[in] a_side Indicates the left or right side (SA_LEFT, SA_RIGHT)
//------------------------------------------------------------------------------
void XmSlopedAbutmentUtilImpl::InterpolateCrossSections(int a_side)
{
  BSHP<XmStampInterpCrossSection> interp = XmStampInterpCrossSection::New();

  double angleFactor(-1.0);

  // left side
  int nDiv = m_nDivLeft;
  XmStampCrossSection next = m_csLeft;
  double maxLen = next.m_leftMax;
  std::vector<XmStampCrossSection>* vCs = &m_interpLeft;
  double angleIncrement = m_angleLeftIncrement;
  VecDbl* vAngles = &m_interpAnglesLeft;
  if (a_side == SA_RIGHT)
  {
    angleFactor = 1.0;
    nDiv = m_nDivRight;
    next = m_csRight;
    maxLen = next.m_rightMax;
    vCs = &m_interpRight;
    angleIncrement = m_angleRightIncrement;
    vAngles = &m_interpAnglesRight;
  }
  XmStampCrossSection saXsect = m_saXsect;
  double saMaxLen = saXsect.m_leftMax;
  EnsureCrossSectionAtMaxX(saXsect);
  XmUtil::ScaleCrossSectionXvals(saXsect, 1 / saMaxLen);
  EnsureCrossSectionAtMaxX(next);
  XmUtil::ScaleCrossSectionXvals(next, 1 / maxLen);

  if (!m_first)
    angleFactor *= -1.0;
  XmStampCrossSection cs;
  vCs->push_back(m_saXsect);
  vAngles->push_back(m_angleCenterLine);
  for (int i = 1; i < nDiv; ++i)
  {
    double percent = (static_cast<double>(i)) / nDiv;
    interp->InterpCs(saXsect, next, percent, cs);
    double len = saMaxLen + percent * (maxLen - saMaxLen);
    XmUtil::ScaleCrossSectionXvals(cs, len);
    vCs->push_back(cs);
    double angle = m_angleCenterLine + (i * angleIncrement * angleFactor);
    vAngles->push_back(angle);
  }
} // XmSlopedAbutmentUtilImpl::InterpolateCrossSections
//------------------------------------------------------------------------------
/// \brief Converts interpolated cross sections to 3d
/// \param[in] a_side Indicates the left or right side (SA_LEFT, SA_RIGHT)
//------------------------------------------------------------------------------
void XmSlopedAbutmentUtilImpl::ConvertTo3d(int a_side)
{
  stXs3dPts* xs3dPtr(&m_3dpts->m_first_endcap);
  if (!m_first)
    xs3dPtr = &m_3dpts->m_last_endcap;
  VecPt3d2d* vPtr(&xs3dPtr->m_left);

  std::vector<XmStampCrossSection>* vCs(&m_interpLeft);
  VecDbl* angles(&m_interpAnglesLeft);
  // left shoulder location
  Pt3d p0 = m_leftShoulder3d;

  if (a_side == SA_RIGHT)
  {
    vCs = &m_interpRight;
    angles = &m_interpAnglesRight;
    p0 = m_rightShoulder3d;
    vPtr = &xs3dPtr->m_right;
  }

  VecPt3d2d& out3dPts(*vPtr);
  for (size_t i = 0; i < vCs->size(); ++i)
  {
    XmStampCrossSection& cs((*vCs)[i]);
    double angleRad((*angles)[i] * XM_PI / 180.0);
    if (!m_first)
      angleRad += XM_PI;
    double maxX(cs.m_leftMax);
    VecPt2d* vPtr(&cs.m_left);
    if (a_side == SA_RIGHT)
    {
      maxX = cs.m_rightMax;
      vPtr = &cs.m_right;
    }
    VecPt2d& pts(*vPtr);
    XmUtil::ConvertXsPointsTo3d(p0, pts, maxX, angleRad, out3dPts);
  }
} // XmSlopedAbutmentUtilImpl::ConvertTo3d
//------------------------------------------------------------------------------
/// \brief Makes sure the cross section goes to the maxX value
/// \param[in,out] a_xs Cross section
//------------------------------------------------------------------------------
void XmSlopedAbutmentUtilImpl::EnsureCrossSectionAtMaxX(XmStampCrossSection& a_xs)
{
  XmUtil::EnsureVectorAtMaxX(a_xs.m_left, a_xs.m_leftMax);
  XmUtil::EnsureVectorAtMaxX(a_xs.m_right, a_xs.m_rightMax);
} // XmSlopedAbutmentUtilImpl::EnsureCrossSectionAtMaxX
//------------------------------------------------------------------------------
/// \brief creates a breakline representing the outer polygon of the stamp
/// \param[in] a_ptIdx The indexes of the points in the TIN
/// \param[out] a_firstEndCapEndPts first end cap ending points
/// \param[out] a_lastEndCapEndPts last end cap ending points
/// \param[in,out] a_io StamperIo class
//------------------------------------------------------------------------------
void XmSlopedAbutmentUtilImpl::GetEndCapEndPoints(cs3dPtIdx& a_ptIdx,
                                                  VecInt& a_firstEndCapEndPts,
                                                  VecInt& a_lastEndCapEndPts,
                                                  XmStamperIo& a_io)
{
  if (a_io.m_lastEndCap.m_type == 1)
  { // last end cap
    {
      VecInt2d& vLeft(a_ptIdx.m_last_end_cap.m_left);
      auto it = vLeft.rbegin();
      auto end = vLeft.rend();
      for (; it != end; ++it)
      {
        if (!it->empty())
          a_lastEndCapEndPts.push_back(it->back());
      }
    }
    {
      VecInt2d& vRight(a_ptIdx.m_last_end_cap.m_right);
      auto it = vRight.begin();
      auto end = vRight.end();
      for (; it != end; ++it)
      {
        if (!it->empty())
          a_lastEndCapEndPts.push_back(it->back());
      }
    }
  }
  if (a_io.m_firstEndCap.m_type == 1)
  { // first end cap
    {
      VecInt2d& vRight(a_ptIdx.m_first_end_cap.m_right);
      auto it = vRight.rbegin();
      auto end = vRight.rend();
      for (; it != end; ++it)
      {
        if (!it->empty())
          a_firstEndCapEndPts.push_back(it->back());
      }
    }
    {
      VecInt2d& vLeft(a_ptIdx.m_first_end_cap.m_left);
      auto it = vLeft.begin();
      auto end = vLeft.end();
      for (; it != end; ++it)
      {
        if (!it->empty())
          a_firstEndCapEndPts.push_back(it->back());
      }
    }
  }
} // XmSlopedAbutmentUtilImpl::GetEndCapEndPoints
//------------------------------------------------------------------------------
/// \brief breaklines from the end cap
/// \param[in,out] a_io StamperIo class
/// \param[in] a_ptIdx The indexes of the points in the TIN created by stamping
/// \param[out] a_blTypes The type of break line (center, xsect, shoulder)
//------------------------------------------------------------------------------
void XmSlopedAbutmentUtilImpl::GetEndCapBreakLines(XmStamperIo& a_io,
                                                   cs3dPtIdx& a_ptIdx,
                                                   VecInt& a_blTypes)
{
  VecInt2d& b(a_io.m_outBreakLines);
  bool firstIsSlopedAbutment(a_io.m_firstEndCap.m_type == 1);
  bool lastIsSlopedAbutment(a_io.m_lastEndCap.m_type == 1);
  // get endcap end points
  VecInt firstEndCapEndPts, lastEndCapEndPts;
  GetEndCapEndPoints(a_ptIdx, firstEndCapEndPts, lastEndCapEndPts, a_io);
  int ix;
  if (firstIsSlopedAbutment)
  {
    ix = a_ptIdx.m_xsPts.m_right.front().back();
    firstEndCapEndPts.insert(firstEndCapEndPts.begin(), ix);
    ix = a_ptIdx.m_xsPts.m_left.front().back();
    firstEndCapEndPts.push_back(ix);
    b.push_back(firstEndCapEndPts);
    a_blTypes.push_back(XmBreaklines::BL_END);
  }
  if (lastIsSlopedAbutment)
  {
    ix = a_ptIdx.m_xsPts.m_left.back().back();
    lastEndCapEndPts.insert(lastEndCapEndPts.begin(), ix);
    ix = a_ptIdx.m_xsPts.m_right.back().back();
    lastEndCapEndPts.push_back(ix);
    b.push_back(lastEndCapEndPts);
    a_blTypes.push_back(XmBreaklines::BL_END);
  }

  // get other endcap breaklines
  auto myLambda = [](VecInt2d& v2d, int ix, VecInt2d& outbl, VecInt& type) {
    for (size_t i = 0; i < v2d.size(); ++i)
    {
      VecInt& v(v2d[i]);
      VecInt bl(1, ix);
      bl.insert(bl.begin(), v.rbegin(), v.rend());
      outbl.push_back(bl);
      type.push_back(XmBreaklines::BL_XSECT);
    }
  };

  int leftShoulder, rightShoulder;
  if (firstIsSlopedAbutment)
  {
    // get left shoulder index for first xsect
    leftShoulder = a_io.m_cs.front().m_idxLeftShoulder;
    ix = a_ptIdx.m_xsPts.m_left.front()[leftShoulder - 1];
    myLambda(a_ptIdx.m_first_end_cap.m_left, ix, b, a_blTypes);
    // get right shoulder index for first xsect
    rightShoulder = a_io.m_cs.front().m_idxRightShoulder;
    ix = a_ptIdx.m_xsPts.m_right.front()[rightShoulder - 1];
    myLambda(a_ptIdx.m_first_end_cap.m_right, ix, b, a_blTypes);
  }

  if (lastIsSlopedAbutment)
  {
    // left shoulder index for last cross section
    leftShoulder = a_io.m_cs.back().m_idxLeftShoulder;
    ix = a_ptIdx.m_xsPts.m_left.back()[leftShoulder - 1];
    myLambda(a_ptIdx.m_last_end_cap.m_left, ix, b, a_blTypes);
    // right shoulder index for last cross section
    rightShoulder = a_io.m_cs.back().m_idxLeftShoulder;
    ix = a_ptIdx.m_xsPts.m_right.back()[rightShoulder - 1];
    myLambda(a_ptIdx.m_last_end_cap.m_right, ix, b, a_blTypes);
  }
} // XmSlopedAbutmentUtilImpl::GetEndCapBreakLines

//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmSlopedAbutmentUtil::XmSlopedAbutmentUtil()
{
} // XmSlopedAbutmentUtil::XmSlopedAbutmentUtil
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmSlopedAbutmentUtil::~XmSlopedAbutmentUtil()
{
} // XmSlopedAbutmentUtil::~XmSlopedAbutmentUtil
//------------------------------------------------------------------------------
/// \brief Creates a XmSlopedAbutmentUtil class
/// \return shared pointer to XmSlopedAbutmentUtil.
//------------------------------------------------------------------------------
BSHP<XmSlopedAbutmentUtil> XmSlopedAbutmentUtil::New()
{
  BSHP<XmSlopedAbutmentUtil> p(new XmSlopedAbutmentUtilImpl());
  return p;
} // StBathemetryIntersector::New

} // namespace xms
