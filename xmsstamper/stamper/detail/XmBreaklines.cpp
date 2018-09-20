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
#include <xmsstamper/stamper/detail/XmBreaklines.h>

// 3. Standard library headers

// 4. External library headers
#pragma warning(push)
#pragma warning(disable : 4512) // boost code: no assignment operator
#include <boost/geometry/index/rtree.hpp>
#pragma warning(pop)

// 5. Shared code headers
#include <xmscore/misc/XmLog.h>
#include <xmsinterp/geometry/geoms.h>
#include <xmsinterp/geometry/GmBoostTypes.h> // GmBstPoly3d, XmBstRing
#include <xmsstamper/stamper/detail/XmGuideBankUtil.h>
#include <xmsstamper/stamper/detail/XmSlopedAbutmentUtil.h>
#include <xmsstamper/stamper/detail/XmStamper3dPts.h>
#include <xmsstamper/stamper/XmStamperIo.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------
typedef std::pair<GmBstBox3d, int> ValueBox;              ///< Pair used in rtree
typedef bgi::rtree<ValueBox, bgi::quadratic<8>> RtreeBox; ///< Rtree typedef

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \brief Implementaion of XmBreaklines
class XmBreaklinesImpl : public XmBreaklines
{
public:
  XmBreaklinesImpl();
  ~XmBreaklinesImpl();

  virtual bool CreateBreaklines(XmStamperIo& a_io, cs3dPtIdx& a_ptIdx, VecInt& a_blTypes) override;
  //------------------------------------------------------------------------------
  /// \brief Returns the outer polygon of the stamp operation
  /// \return The outer polyogn.
  //------------------------------------------------------------------------------
  virtual const VecInt& GetOuterPolygon() override { return m_outerPoly; }
  bool BreaklinesIntersect(const VecInt2d& a_bl, const VecPt3d& a_pts) override;

  void CreateOuterPolygonBreakline(cs3dPtIdx& a_ptIdx,
                                   VecInt& a_leftCsEndPts,
                                   VecInt& a_rightCsEndPts,
                                   VecInt& a_firstXsPts,
                                   VecInt& a_lastXsPts,
                                   XmStamperIo& a_io);
  void GetEndCapEndPoints(cs3dPtIdx& a_ptIdx,
                          VecInt& a_firstEndCapEndPts,
                          VecInt& a_lastEndCapEndPts,
                          XmStamperIo& a_io);

  VecInt m_outerPoly; ///< outer polygon of the stamp operation
  /// spatial index to check intersections of breaklines
  BSHP<RtreeBox> m_rtree;
  /// utility class for Sloped Abutment End Caps
  BSHP<XmSlopedAbutmentUtil> m_slopedAbutment;
  /// utility class for Guidebank End Caps
  BSHP<XmGuideBankUtil> m_guideBank;
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmBreaklinesImpl
/// \brief Creates breaklines for the stamp operation
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmBreaklinesImpl::XmBreaklinesImpl()
: m_slopedAbutment(XmSlopedAbutmentUtil::New())
, m_guideBank(XmGuideBankUtil::New())
{
} // XmBreaklinesImpl::XmBreaklinesImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmBreaklinesImpl::~XmBreaklinesImpl()
{
} // XmBreaklinesImpl::~XmBreaklinesImpl
//------------------------------------------------------------------------------
/// \brief Creates breaklines
/// \param[in,out] a_io XmamperIo class
/// \param[in] a_ptIdx The indexes of the points in the TIN created by stamping
/// \param[out] a_blTypes The type of breakline (centerline, shoulder, xsect)
/// \return true if breaklines successfully created.
//------------------------------------------------------------------------------
bool XmBreaklinesImpl::CreateBreaklines(XmStamperIo& a_io, cs3dPtIdx& a_ptIdx, VecInt& a_blTypes)
{
  a_blTypes.resize(0);
  VecInt2d& b(a_io.m_outBreakLines);
  // break line for the center line
  b.push_back(a_ptIdx.m_centerLine);
  a_blTypes.push_back(BL_CENTERLINE);
  // break line for each cross section
  VecInt leftCsEndPts, rightCsEndPts, leftShoulder, rightShoulder;
  VecInt firstXsPts, lastXsPts;
  for (size_t i = 0; i < a_ptIdx.m_centerLine.size(); ++i)
  {
    VecInt ixs;
    // reverse add the left side
    {
      auto p = a_ptIdx.m_xsPts.m_left[i].rbegin();
      auto end = a_ptIdx.m_xsPts.m_left[i].rend();
      for (; p != end; ++p)
        ixs.push_back(*p);
    }
    // put in the center line point
    ixs.push_back(a_ptIdx.m_centerLine[i]);
    // forward add the right side
    {
      auto p = a_ptIdx.m_xsPts.m_right[i].begin();
      auto end = a_ptIdx.m_xsPts.m_right[i].end();
      for (; p != end; ++p)
        ixs.push_back(*p);
    }
    b.push_back(ixs);
    a_blTypes.push_back(BL_XSECT);
    if (i == 0)
    {
      firstXsPts = ixs;
      a_blTypes.back() = BL_END;
    }
    if (i + 1 == a_ptIdx.m_centerLine.size())
    {
      lastXsPts = ixs;
      a_blTypes.back() = BL_END;
    }

    // left side shoulder and endpts
    {
      VecInt& v(a_ptIdx.m_xsPts.m_left[i]);
      size_t leftIdx = static_cast<size_t>(a_io.m_cs[i].m_idxLeftShoulder - 1);
      if (!v.empty())
      {
        // break line for the end points of the cross sections
        leftCsEndPts.push_back(v.back());
        // break line for the shoulders of the cross sections
        if (leftIdx < v.size())
          leftShoulder.push_back(v[leftIdx]);
        else
          leftShoulder.push_back(v.back());
      }
      else
      {
        leftCsEndPts.push_back(a_ptIdx.m_centerLine[i]);
        leftShoulder.push_back(a_ptIdx.m_centerLine[i]);
      }
    }
    // right side shoulder and endpts
    {
      VecInt& v(a_ptIdx.m_xsPts.m_right[i]);
      size_t rightIdx = static_cast<size_t>(a_io.m_cs[i].m_idxRightShoulder - 1);
      if (!v.empty())
      {
        // break line for the end points of the cross sections
        rightCsEndPts.push_back(v.back());
        // break line for the shoulders of the cross sections
        if (rightIdx < v.size())
          rightShoulder.push_back(v[rightIdx]);
        else
          rightShoulder.push_back(v.back());
      }
      else
      {
        rightCsEndPts.push_back(a_ptIdx.m_centerLine[i]);
        rightShoulder.push_back(a_ptIdx.m_centerLine[i]);
      }
    }
  }
  b.push_back(leftCsEndPts);
  a_blTypes.push_back(BL_END);
  b.push_back(rightCsEndPts);
  a_blTypes.push_back(BL_END);
  b.push_back(leftShoulder);
  a_blTypes.push_back(BL_SHOULDER);
  b.push_back(rightShoulder);
  a_blTypes.push_back(BL_SHOULDER);

  m_slopedAbutment->GetEndCapBreakLines(a_io, a_ptIdx, a_blTypes);
  m_guideBank->GetEndCapBreakLines(a_io, a_ptIdx, a_blTypes);

  CreateOuterPolygonBreakline(a_ptIdx, leftCsEndPts, rightCsEndPts, firstXsPts, lastXsPts, a_io);

  return true;
} // XmBreaklinesImpl::CreateBreaklines
//------------------------------------------------------------------------------
/// \brief creates a breakline representing the outer polygon of the stamp
/// \param[in] a_ptIdx The indexes of the points in the TIN
/// \param[in] a_leftCsEndPts Left side breakline of the stamp
/// \param[in] a_rightCsEndPts Right side breakline of the stamp
/// \param[in] a_firstXsPts Breakline of the first cross section
/// \param[in] a_lastXsPts Breakline of the last cross section
/// \param[in,out] a_io StamperIo class
//------------------------------------------------------------------------------
void XmBreaklinesImpl::CreateOuterPolygonBreakline(cs3dPtIdx& a_ptIdx,
                                                   VecInt& a_leftCsEndPts,
                                                   VecInt& a_rightCsEndPts,
                                                   VecInt& a_firstXsPts,
                                                   VecInt& a_lastXsPts,
                                                   XmStamperIo& a_io)
{
  // get endcap end points
  VecInt firstEndCapEndPts, lastEndCapEndPts;
  GetEndCapEndPoints(a_ptIdx, firstEndCapEndPts, lastEndCapEndPts, a_io);
  // make a break line of the outer boundary
  {
    VecInt ixs(a_leftCsEndPts);
    if (!lastEndCapEndPts.empty())
      ixs.insert(ixs.end(), lastEndCapEndPts.begin(), lastEndCapEndPts.end());
    else
      ixs.insert(ixs.end(), a_lastXsPts.begin(), a_lastXsPts.end());
    ixs.insert(ixs.end(), a_rightCsEndPts.rbegin(), a_rightCsEndPts.rend());
    if (!firstEndCapEndPts.empty())
      ixs.insert(ixs.end(), firstEndCapEndPts.begin(), firstEndCapEndPts.end());
    else
      ixs.insert(ixs.end(), a_firstXsPts.rbegin(), a_firstXsPts.rend());
    ixs.push_back(ixs.front());
    // remove any consective points with the same index
    auto last = std::unique(ixs.begin(), ixs.end());
    ixs.erase(last, ixs.end());
    m_outerPoly.swap(ixs);
  }
} // XmBreaklinesImpl::CreateOuterPolygonBreakline
//------------------------------------------------------------------------------
/// \brief creates a breakline representing the outer polygon of the stamp
/// \param[in] a_ptIdx The indexes of the points in the TIN
/// \param[out] a_firstEndCapEndPts first end cap ending points
/// \param[out] a_lastEndCapEndPts last end cap ending points
/// \param[in,out] a_io StamperIo class
//------------------------------------------------------------------------------
void XmBreaklinesImpl::GetEndCapEndPoints(cs3dPtIdx& a_ptIdx,
                                          VecInt& a_firstEndCapEndPts,
                                          VecInt& a_lastEndCapEndPts,
                                          XmStamperIo& a_io)
{
  a_firstEndCapEndPts.resize(0);
  a_lastEndCapEndPts.resize(0);
  m_slopedAbutment->GetEndCapEndPoints(a_ptIdx, a_firstEndCapEndPts, a_lastEndCapEndPts, a_io);
  m_guideBank->GetEndCapEndPoints(a_ptIdx, a_firstEndCapEndPts, a_lastEndCapEndPts, a_io);
} // XmBreaklinesImpl::GetEndCapEndPoints
//------------------------------------------------------------------------------
/// \brief Check if any breakline segments intersect
/// \param[in] a_bl Breaklines (indexes to the points)
/// \param[in] a_pts The point locations created in the stamp operation
/// \return true if any breakline segments intersect a segment with a non shared
/// point index
//------------------------------------------------------------------------------
bool XmBreaklinesImpl::BreaklinesIntersect(const VecInt2d& a_bl, const VecPt3d& a_pts)
{
  const VecInt2d& vbl(a_bl);
  // create vector of breakline segments
  std::pair<int, int> p;
  std::vector<std::pair<int, int>> vSegs;
  Pt3d bMin, bMax;
  ValueBox aBox;
  std::vector<ValueBox> vBoxes;
  for (const auto& bl : vbl)
  {
    for (size_t i = 1; i < bl.size(); ++i)
    {
      p.first = bl[i - 1];
      p.second = bl[i];
      const Pt3d &p0(a_pts[p.first]), &p1(a_pts[p.second]);
      if (p0.x < p1.x)
      {
        bMin.x = p0.x;
        bMax.x = p1.x;
      }
      else
      {
        bMin.x = p1.x;
        bMax.x = p0.x;
      }
      if (p0.y < p1.y)
      {
        bMin.y = p0.y;
        bMax.y = p1.y;
      }
      else
      {
        bMin.y = p1.y;
        bMax.y = p0.y;
      }

      GmBstBox3d bb(bMin, bMax);
      aBox.first = GmBstBox3d(bMin, bMax);
      aBox.second = static_cast<int>(vSegs.size());
      vBoxes.push_back(aBox);
      vSegs.push_back(p);
    }
  }
  m_rtree.reset(new RtreeBox(vBoxes.begin(), vBoxes.end()));

  // check intersections
  std::vector<ValueBox> result;
  for (size_t i = 0; i < vSegs.size(); ++i)
  {
    result.resize(0);
    const Pt3d &p0(a_pts[vSegs[i].first]), &p1(a_pts[vSegs[i].second]);

    m_rtree->query(bgi::intersects(vBoxes[i].first), std::back_inserter(result));
    for (size_t j = 0; j < result.size(); ++j)
    {
      int ix = result[j].second;
      // is this the current "i" segment
      if (ix == (int)i)
        continue;
      // see if the result shares a point with the "i" segment
      if (vSegs[i].first == vSegs[ix].first || vSegs[i].first == vSegs[ix].second ||
          vSegs[i].second == vSegs[ix].first || vSegs[i].second == vSegs[ix].second)
      {
        continue;
      }
      // check if the lines intersect
      const Pt3d &p2(a_pts[vSegs[ix].first]), &p3(a_pts[vSegs[ix].second]);
      if (gmLinesIntersect(p0, p1, p2, p3))
      {
        return true;
      }
    }
  }
  return false;
} // XmBreaklinesImpl::BreaklinesIntersect

//------------------------------------------------------------------------------
/// \brief Creates a XmStampInterpCrossSection class
/// \return shared pointer to XmBreaklines
//------------------------------------------------------------------------------
BSHP<XmBreaklines> XmBreaklines::New()
{
  BSHP<XmBreaklines> p(new XmBreaklinesImpl());
  return p;
} // XmBreaklines::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmBreaklines::XmBreaklines()
{
} // XmBreaklines::XmBreaklines
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmBreaklines::~XmBreaklines()
{
} // XmBreaklines::~XmBreaklines

} // namespace xms
