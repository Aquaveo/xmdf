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
#include <xmsstamper/stamper/detail/XmStampInterpCrossSection.h>

// 3. Standard library headers
#include <map>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/math/math.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/stl/set.h>
#include <xmsinterp/geometry/geoms.h>
#include <xmscore/misc/Observer.h>
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
/// \brief Implementaion of XmStampInterpCrossSection
class XmStampInterpCrossSectionImpl : public XmStampInterpCrossSection
{
public:
  XmStampInterpCrossSectionImpl();
  ~XmStampInterpCrossSectionImpl();

  virtual void InterpMissingCrossSections(XmStamperIo& a_) override;
  virtual bool ValidCrossSectionsExist(XmStamperIo& a_) override;
  virtual void InterpCs(XmStampCrossSection& a_prev,
                        XmStampCrossSection& a_next,
                        double a_percent,
                        XmStampCrossSection& a_cs) override;
  virtual void InterpFromCenterlineProfile(XmStamperIo& a_io,
                                           XmStamperCenterlineProfile& a_profile) override;

  BSHP<Observer> m_observer; ///< progress observer
  XmStamperIo* m_io;         ///< pointer to the inputs to the stamp operation

  int FindFirstValidCrossSection();
  int FindLastValidCrossSection();
  int FindNextValidCrossSection(int a_idx);
  void InterpCs(int a_prev, int a_next, double a_percent, XmStampCrossSection& a_cs);
  void InterpPts(VecPt2d& a_v1,
                 int a_beg1,
                 int a_end1,
                 VecPt2d& a_v2,
                 int a_beg2,
                 int a_end2,
                 double a_percent,
                 VecPt2d& a_interpPts);
  void CalcTvals(VecPt2d& a_v, int a_beg, int a_end, VecDbl& a_t);
  Pt2d PtFromT(VecPt2d& a_v, int a_beg, VecDbl& a_t, double a_tval);
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmStamperImpl
/// \brief Performs a feature stamp operation
/// The result of the operation is a TIN and a list of breakline segments.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStampInterpCrossSectionImpl::XmStampInterpCrossSectionImpl()
: m_observer()
, m_io(nullptr)
{
} // XmStampInterpCrossSectionImpl::XmStampInterpCrossSectionImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStampInterpCrossSectionImpl::~XmStampInterpCrossSectionImpl()
{
} // XmStampInterpCrossSectionImpl::~XmStampInterpCrossSectionImpl
//------------------------------------------------------------------------------
/// \brief Interpolates the missing cross sections. Modifys the m_cs member of
/// the XmStamperIo class that is passed to this method.
/// \param [in,out] a_ The inputs and outputs for the feature stamp operation
//------------------------------------------------------------------------------
void XmStampInterpCrossSectionImpl::InterpMissingCrossSections(XmStamperIo& a_)
{
  m_io = &a_;

  int first = FindFirstValidCrossSection();
  // copy this cross section to any before it
  for (int i = 0; i < first; ++i)
  {
    m_io->m_cs[i] = m_io->m_cs[first];
  }
  int last = FindLastValidCrossSection();
  // copy this cross section to any after it
  for (size_t i = (size_t)(last + 1); last > 0 && i < m_io->m_cs.size(); ++i)
  {
    m_io->m_cs[i] = m_io->m_cs[last];
  }
  // loop between first and last to fill in the cross sections
  const VecPt3d& pts(m_io->m_centerLine);
  int prev = first, next;
  double prevDist, nextDist, percent;
  for (int i = first + 1; i < last; ++i)
  {
    auto& cs(m_io->m_cs[i]);
    if (cs.m_left.size() > 1 || cs.m_right.size() > 1)
    {
      prev = i;
      continue;
    }

    // get the next valid cross section
    next = FindNextValidCrossSection(i);
    // get distance between current and previous
    prevDist = gmXyDistance(pts[prev], pts[i]);
    // get distance between current and next
    nextDist = gmXyDistance(pts[i], pts[next]);
    percent = prevDist / (prevDist + nextDist);
    // interpolate the cross section
    InterpCs(prev, next, percent, cs);
  }
} // XmStampInterpCrossSectionImpl::InterpMissingCrossSections
//------------------------------------------------------------------------------
/// \brief Interpolates the missing cross sections. Modifys the m_cs member of
/// the XmStamperIo class that is passed to this method.
/// \param [in,out] a_ The inputs and outputs for the feature stamp operation
/// \return true if there are any valid cross sections
//------------------------------------------------------------------------------
bool XmStampInterpCrossSectionImpl::ValidCrossSectionsExist(XmStamperIo& a_)
{
  m_io = &a_;
  if (FindFirstValidCrossSection() > -1)
    return true;
  return false;
} // XmStampInterpCrossSectionImpl::InterpMissingCrossSections
//------------------------------------------------------------------------------
/// \brief Finds the first valid cross section in the inputs.
/// \return The index to the first valid cross section in the inputs. -1 is
/// returned if no valid cross sections exist.
//------------------------------------------------------------------------------
int XmStampInterpCrossSectionImpl::FindFirstValidCrossSection()
{
  return FindNextValidCrossSection(-1);
} // XmStampInterpCrossSectionImpl::FindFirstValidCrossSection
//------------------------------------------------------------------------------
/// \brief Finds the last valid cross section in the inputs.
/// \return The index to the last valid cross section in the inputs. -1 is
/// returned if no valid cross sections exist.
//------------------------------------------------------------------------------
int XmStampInterpCrossSectionImpl::FindLastValidCrossSection()
{
  int csIdx(-1);
  for (size_t i = m_io->m_cs.size(); csIdx == -1 && i > 0; --i)
  {
    const auto& cs(m_io->m_cs[i - 1]);
    if (cs.m_left.size() > 1 || cs.m_right.size() > 1)
    {
      csIdx = static_cast<int>(i - 1);
    }
  }
  return csIdx;
} // XmStampInterpCrossSectionImpl::FindLastValidCrossSection
//------------------------------------------------------------------------------
/// \brief Finds the first valid cross section after a_idx in the inputs.
/// \param[in] a_idx Index used to find the next valid cross section
/// \return The index to the next valid cross section in the inputs. If none
/// exists then -1 is returned.
//------------------------------------------------------------------------------
int XmStampInterpCrossSectionImpl::FindNextValidCrossSection(int a_idx)
{
  int csIdx(-1);
  size_t start(0);
  if (a_idx > 0)
    start = a_idx + 1;
  for (size_t i = start; csIdx == -1 && i < m_io->m_cs.size(); ++i)
  {
    const auto& cs(m_io->m_cs[i]);
    if (cs.m_left.size() > 1 || cs.m_right.size() > 1)
    {
      csIdx = static_cast<int>(i);
    }
  }
  return csIdx;
} // XmStampInterpCrossSectionImpl::FindNextValidCrossSection
//------------------------------------------------------------------------------
/// \brief Interpolates a cross section from 2 other cross sections using a
/// weight (a_percent)
/// \param[in] a_prev Index to the first cross section used to interpolate.
/// \param[in] a_next Index to the second cross section used to interpolate.
/// \param[in] a_percent Interpolation weight applied to the prev/next cross
/// sections.
/// \param[out] a_cs The newly interpolated cross section.
//------------------------------------------------------------------------------
void XmStampInterpCrossSectionImpl::InterpCs(int a_prev,
                                             int a_next,
                                             double a_percent,
                                             XmStampCrossSection& a_cs)
{
  XmStampCrossSection &pCs(m_io->m_cs[a_prev]), &nCs(m_io->m_cs[a_next]);
  InterpCs(pCs, nCs, a_percent, a_cs);
} // XmStampInterpCrossSectionImpl::InterpCs
//------------------------------------------------------------------------------
/// \brief Interpolates a cross section from 2 other cross sections using a
/// weight (a_percent)
/// \param[in] a_prev The first cross section used to interpolate.
/// \param[in] a_next The second cross section used to interpolate.
/// \param[in] a_percent Interpolation weight applied to the prev/next cross
/// sections.
/// \param[out] a_cs The newly interpolated cross section.
//------------------------------------------------------------------------------
void XmStampInterpCrossSectionImpl::InterpCs(XmStampCrossSection& a_prev,
                                             XmStampCrossSection& a_next,
                                             double a_percent,
                                             XmStampCrossSection& a_cs)
{
  XmStampCrossSection &pCs(a_prev), &nCs(a_next);
  XM_ENSURE_TRUE(!pCs.m_left.empty() || !pCs.m_right.empty());
  XM_ENSURE_TRUE(!nCs.m_left.empty() || !nCs.m_right.empty());
  // get the interpolated center line location because the Interp pts
  // method will not do the t = 0 location
  Pt2d pt, p1, p2;
  if (!pCs.m_left.empty())
    p1 = pCs.m_left[0];
  else if (!pCs.m_right.empty())
    p1 = pCs.m_right[0];
  if (!nCs.m_left.empty())
    p2 = nCs.m_left[0];
  else if (!nCs.m_right.empty())
    p2 = nCs.m_right[0];
  pt.x = p1.x - (a_percent * (p1.x - p2.x));
  pt.y = p1.y - (a_percent * (p1.y - p2.y));
  a_cs.m_left.assign(1, pt);
  a_cs.m_right.assign(1, pt);

  // Left side interpolation
  VecPt2d &l1(pCs.m_left), &l2(nCs.m_left), &l3(a_cs.m_left);
  int ls1(pCs.m_idxLeftShoulder), ls2(nCs.m_idxLeftShoulder);
  // interpolate points between the center line and the left shoulder
  InterpPts(l1, 0, ls1, l2, 0, ls2, a_percent, l3);
  a_cs.m_idxLeftShoulder = std::max(0, (int)l3.size() - 1);
  // interpolate points between left shoulder and end
  int lend1((int)pCs.m_left.size() - 1), lend2((int)nCs.m_left.size() - 1);
  InterpPts(l1, ls1, lend1, l2, ls2, lend2, a_percent, l3);
  // interpolate max dist
  a_cs.m_leftMax = a_prev.m_leftMax - (a_percent * (a_prev.m_leftMax - a_next.m_leftMax));

  // Right side interpolation
  VecPt2d &r1(pCs.m_right), &r2(nCs.m_right), &r3(a_cs.m_right);
  int rs1(pCs.m_idxRightShoulder), rs2(nCs.m_idxRightShoulder);
  // interpolate points between the center line and the left shoulder
  InterpPts(r1, 0, rs1, r2, 0, rs2, a_percent, r3);
  a_cs.m_idxRightShoulder = std::max(0, (int)r3.size() - 1);
  // interpolate points between left shoulder and end
  int rend1((int)pCs.m_right.size() - 1), rend2((int)nCs.m_right.size() - 1);
  InterpPts(r1, rs1, rend1, r2, rs2, rend2, a_percent, r3);
  // interpolate max dist
  a_cs.m_rightMax = a_prev.m_rightMax - (a_percent * (a_prev.m_rightMax - a_next.m_rightMax));
} // XmStampInterpCrossSectionImpl::InterpCs
//------------------------------------------------------------------------------
/// \brief Converts XmStamperCenterlineProfile class to XmStamperIo class inputs
/// \param[in,out] a_io The stamping input/output class
/// \param[in] a_profile The stamping centerline profile class
//------------------------------------------------------------------------------
void XmStampInterpCrossSectionImpl::InterpFromCenterlineProfile(
  XmStamperIo& a_io,
  XmStamperCenterlineProfile& a_profile)
{
  XM_ENSURE_TRUE(!a_profile.m_distance.empty());
  XM_ENSURE_TRUE(a_profile.m_distance.size() == a_profile.m_elevation.size());
  XM_ENSURE_TRUE(a_profile.m_distance.size() == a_profile.m_cs.size());
  XM_ENSURE_TRUE(!a_io.m_centerLine.empty());

  // calculate the distance along the center line
  VecDbl dist(a_io.m_centerLine.size(), 0);
  VecPt3d& pts(a_io.m_centerLine);
  for (size_t i = 1; i < pts.size(); ++i)
  {
    Pt3d &p0(pts[i - 1]), &p1(pts[i]);
    dist[i] = Mdist(p0.x, p0.y, p1.x, p1.y);
    dist[i] += dist[i - 1];
  }

  // init the cross sections
  a_io.m_cs.assign(a_io.m_centerLine.size(), XmStampCrossSection());

  VecDbl& d(a_profile.m_distance);
  std::map<double, size_t> mapDistIdx;
  for (size_t i = 0; i < d.size(); ++i)
  {
    mapDistIdx.insert(std::make_pair(d[i], i));
  }

  auto itBegin = mapDistIdx.begin();
  auto itEnd = mapDistIdx.end();
  auto itLast = --mapDistIdx.end();
  // interpolate between values in the profile
  for (size_t i = 0; i < dist.size(); ++i)
  {
    // get closest profile point equal or after current point
    auto it1 = mapDistIdx.lower_bound(dist[i]);
    if (it1 == itEnd || (it1 == itLast && it1->first <= dist[i]))
    { // centerline point after or at the last point in the profile
      a_io.m_centerLine[i].z = a_profile.m_elevation.back();
      a_io.m_cs[i] = a_profile.m_cs.back();
      continue;
    }
    else if (it1->first >= dist[i] && it1 == itBegin)
    { // centerline point before or at the first point in the profile
      a_io.m_centerLine[i].z = a_profile.m_elevation.front();
      a_io.m_cs[i] = a_profile.m_cs.front();
      continue;
    }
    auto it0 = it1;
    if (it0 == itBegin)
    {
      XM_ASSERT(0);
      continue;
    }
    it0--;
    size_t ix0(it0->second), ix1(it1->second);

    // get interp factor
    double percent = (dist[i] - it0->first) / (it1->first - it0->first);
    // interp elevation
    VecDbl& elev(a_profile.m_elevation);
    a_io.m_centerLine[i].z = elev[ix0] + percent * (elev[ix1] - elev[ix0]);
    // interp cross section
    XmStampCrossSection& prev(a_profile.m_cs[ix0]);
    XmStampCrossSection& next(a_profile.m_cs[ix1]);
    XmStampCrossSection& cs(a_io.m_cs[i]);
    InterpCs(prev, next, percent, cs);
  }
} // XmStampInterpCrossSectionImpl::InterpFromCenterlineProfile
//------------------------------------------------------------------------------
/// \brief Interpolates part of 2 cross sections to part of a new cross section.
/// Separate interpolations are done for the cross section between the center
/// line and the shoulder and then from the shoulder to boundary of the cross
/// section.
/// \param[in] a_v1 pts from the first cross section
/// \param[in] a_beg1 Index to the start of the section of a_v1
/// \param[in] a_end1 Index to the end of the section of a_v1
/// \param[in] a_v2 pts from the second cross section
/// \param[in] a_beg2 Index to the start of the section of a_v2
/// \param[in] a_end2 Index to the end of the section of a_v2
/// \param[in] a_percent Interpolation weight applied to the prev/next cross
/// sections.
/// \param[out] a_interpPts Vector of newly interpolated points
//------------------------------------------------------------------------------
void XmStampInterpCrossSectionImpl::InterpPts(VecPt2d& a_v1,
                                              int a_beg1,
                                              int a_end1,
                                              VecPt2d& a_v2,
                                              int a_beg2,
                                              int a_end2,
                                              double a_percent,
                                              VecPt2d& a_interpPts)
{
  VecDbl t1, t2;
  CalcTvals(a_v1, a_beg1, a_end1, t1);
  CalcTvals(a_v2, a_beg2, a_end2, t2);
  // get a union of t values
  SetDbl tvals(t1.begin(), t1.end());
  tvals.insert(t2.begin(), t2.end());

  Pt2d p1, p2, pt;
  tvals.erase(0.0);
  for (const auto& t : tvals)
  {
    p1 = PtFromT(a_v1, a_beg1, t1, t);
    p2 = PtFromT(a_v2, a_beg2, t2, t);
    pt.x = p1.x - (a_percent * (p1.x - p2.x));
    pt.y = p1.y - (a_percent * (p1.y - p2.y));
    a_interpPts.push_back(pt);
  }

} // XmStampInterpCrossSectionImpl::InterpPts
//------------------------------------------------------------------------------
/// \brief Calculates parametric t values from a vector of x,y coords based on x
/// \param[in] a_v vector of x,y
/// \param[in] a_beg Index to the start of the section of a_v
/// \param[in] a_end Index to the end of the section of a_v
/// \param[out] a_t Vector of calculated t values
//------------------------------------------------------------------------------
void XmStampInterpCrossSectionImpl::CalcTvals(VecPt2d& a_v, int a_beg, int a_end, VecDbl& a_t)
{
  if (a_v.empty())
    return;
  a_t.assign(1, 0);
  // calculate parametric t values for each x value
  double diff = a_v[a_end].x - a_v[a_beg].x;
  for (int i = a_beg + 1; i <= a_end; ++i)
  {
    double t = (a_v[i].x - a_v[a_beg].x) / diff;
    a_t.push_back(t);
  }
} // XmStampInterpCrossSectionImpl::CalcTvals
//------------------------------------------------------------------------------
/// \brief Calculates the x,y location of the point at a_tval
/// \param[in] a_v vector of x,y
/// \param[in] a_beg Index to the start of the section of a_v
/// \param[in] a_t Vector of calculated t values
/// \param[in] a_tval Parametric tvalue
/// \return the x,y location of the point at a_tval
//------------------------------------------------------------------------------
Pt2d XmStampInterpCrossSectionImpl::PtFromT(VecPt2d& a_v, int a_beg, VecDbl& a_t, double a_tval)
{
  if (a_v.empty())
    return Pt2d();
  if (a_t.size() == 1 && a_t[0] == 0.0)
    return a_v[0];

  Pt2d r, p1, p2;
  double t1(-1), t2(-1);
  int idx(-1);
  // find where t is
  for (size_t i = 1; idx < 0 && i < a_t.size(); ++i)
  {
    if (a_tval < a_t[i])
    {
      idx = static_cast<int>(i) - 1;
      p1 = a_v[a_beg + idx];
      p2 = a_v[a_beg + idx + 1];
      t1 = a_t[idx];
      t2 = a_t[idx + 1];
    }
    else if (a_tval == a_t[i])
    {
      idx = static_cast<int>(i);
      p1 = a_v[a_beg + idx];
      t1 = a_t[idx];
    }
  }
  // interpolate to t
  if (a_tval == t1)
    return p1;
  double localt = (a_tval - t1) / (t2 - t1);
  r.x = p1.x + localt * (p2.x - p1.x);
  r.y = p1.y + localt * (p2.y - p1.y);
  return r;
} // XmStampInterpCrossSectionImpl::PtFromT

//------------------------------------------------------------------------------
/// \brief Creates a XmStampInterpCrossSection class.
/// \return shared pointer to XmStampInterpCrossSection.
//------------------------------------------------------------------------------
BSHP<XmStampInterpCrossSection> XmStampInterpCrossSection::New()
{
  BSHP<XmStampInterpCrossSection> interp(new XmStampInterpCrossSectionImpl);
  return interp;
} // XmStampInterpCrossSection::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStampInterpCrossSection::XmStampInterpCrossSection()
{
} // XmStampInterpCrossSection::XmStamperXmStampInterpCrossSection
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmStampInterpCrossSection::~XmStampInterpCrossSection()
{
} // XmStampInterpCrossSection::~XmStampInterpCrossSection

} // namespace xms

#ifdef CXX_TEST
//------------------------------------------------------------------------------
// Unit Tests
//------------------------------------------------------------------------------
using namespace xms;
#include <xmsstamper/stamper/detail/XmStampInterpCrossSection.t.h>

#include <xmscore/testing/TestTools.h>

//------------------------------------------------------------------------------
/// \brief Tests symetric cross section interpolation
//------------------------------------------------------------------------------
void XmStampInterpCrossSectionUnitTests::test0()
{
  XmStamperIo io;
  io.m_centerLine = {{0, 0}, {5, 0}, {10, 0}, {15, 0}, {20, 0}, {25, 0}};
  io.m_cs.assign(6, XmStampCrossSection());

  // should fail because there are no valid cross sections
  XmStampInterpCrossSectionImpl ip;
  ip.InterpMissingCrossSections(io);
  for (auto& c : io.m_cs)
  {
    TS_ASSERT(c.m_left.empty());
    TS_ASSERT(c.m_right.empty());
    TS_ASSERT(c.m_idxLeftShoulder == 0);
    TS_ASSERT(c.m_idxRightShoulder == 0);
    TS_ASSERT(c.m_leftMax == 0);
    TS_ASSERT(c.m_rightMax == 0);
  }

  XmStampCrossSection cs, cs1;
  io.m_stampingType = 0;

  cs.m_left = {{0, 10}, {1, 11}, {2, 12}, {4, 11}, {5, 10}, {10, 5}, {15, 0}};
  cs.m_idxLeftShoulder = 4;
  cs.m_leftMax = 16;
  cs.m_right = cs.m_left;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  cs.m_rightMax = cs.m_leftMax;
  io.m_cs[1] = cs;

  cs1.m_left = {{0, 20}, {4, 19}, {6, 18}, {8, 18}, {10, 20}, {15, 15}, {20, 10}};
  cs1.m_leftMax = 19;
  cs1.m_idxLeftShoulder = 4;
  cs1.m_right = cs1.m_left;
  cs1.m_idxRightShoulder = cs1.m_idxLeftShoulder;
  cs1.m_rightMax = cs1.m_leftMax;
  io.m_cs[4] = cs1;

  ip.InterpMissingCrossSections(io);
  // values copied from cs[1]
  TS_ASSERT_EQUALS_VEC(io.m_cs[1].m_left, io.m_cs[0].m_left);
  TS_ASSERT_EQUALS(io.m_cs[1].m_idxLeftShoulder, io.m_cs[0].m_idxLeftShoulder);
  TS_ASSERT_EQUALS(io.m_cs[1].m_leftMax, io.m_cs[0].m_leftMax);
  TS_ASSERT_EQUALS(io.m_cs[1].m_rightMax, io.m_cs[0].m_rightMax);
  TS_ASSERT_EQUALS_VEC(io.m_cs[1].m_right, io.m_cs[0].m_right);
  TS_ASSERT_EQUALS(4, io.m_cs[0].m_idxRightShoulder);
  // values copied from cs[4]
  TS_ASSERT_EQUALS_VEC(io.m_cs[4].m_left, io.m_cs[5].m_left);
  TS_ASSERT_EQUALS(io.m_cs[4].m_idxLeftShoulder, io.m_cs[5].m_idxLeftShoulder);
  TS_ASSERT_EQUALS(io.m_cs[4].m_leftMax, io.m_cs[5].m_leftMax);
  TS_ASSERT_EQUALS(io.m_cs[4].m_rightMax, io.m_cs[5].m_rightMax);
  TS_ASSERT_EQUALS_VEC(io.m_cs[4].m_right, io.m_cs[5].m_right);
  TS_ASSERT_EQUALS(io.m_cs[4].m_idxRightShoulder, io.m_cs[5].m_idxRightShoulder);
  VecPt2d baseCs = {{0.00, 13.33}, {1.33, 13.83}, {2.67, 14.33}, {4.0, 13.67},
                    {5.33, 13.33}, {6.67, 13.33}, {11.67, 8.33}, {16.67, 3.33}};
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[2].m_left, 1e-2);
  TS_ASSERT_EQUALS(5, io.m_cs[2].m_idxLeftShoulder);
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[2].m_right, 1e-2);
  TS_ASSERT_EQUALS(5, io.m_cs[2].m_idxRightShoulder);
  VecPt2d baseCs1 = {{0, 16.67},    {1.67, 16.67}, {3.33, 16.67},  {5.0, 15.83},
                     {6.67, 15.67}, {8.33, 16.67}, {13.33, 11.67}, {18.33, 6.67}};
  TS_ASSERT_DELTA_VECPT2D(baseCs1, io.m_cs[3].m_left, 1e-2);
  TS_ASSERT_EQUALS(5, io.m_cs[3].m_idxLeftShoulder);
  TS_ASSERT_DELTA_VECPT2D(baseCs1, io.m_cs[3].m_right, 1e-2);
  TS_ASSERT_EQUALS(5, io.m_cs[3].m_idxRightShoulder);

  cs.m_idxLeftShoulder = 0;
  cs.m_left.clear();
  cs.m_leftMax = 0;
  io.m_cs.assign(6, XmStampCrossSection());
  io.m_cs[1] = cs;
  io.m_cs[4] = cs1;
  ip.InterpMissingCrossSections(io);

  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[2].m_right, 1e-2);
  TS_ASSERT_EQUALS(5, io.m_cs[2].m_idxRightShoulder);
  baseCs = {{0, 13.33}, {1.33, 6.33}, {2, 6}, {2.67, 6}, {3.33, 6.67}, {5, 5}, {6.67, 3.33}};
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[2].m_left, 1e-2);
  TS_ASSERT_DELTA(6.33, io.m_cs[2].m_leftMax, 1e-2);

  TS_ASSERT_DELTA_VECPT2D(baseCs1, io.m_cs[3].m_right, 1e-2);
  TS_ASSERT_EQUALS(5, io.m_cs[3].m_idxRightShoulder);
  baseCs1 = {{0, 16.67},    {2.67, 12.67}, {4, 12},      {5.33, 12},
             {6.67, 13.33}, {10, 10},      {13.33, 6.67}};
  TS_ASSERT_DELTA_VECPT2D(baseCs1, io.m_cs[3].m_left, 1e-2);
  TS_ASSERT_DELTA(12.66, io.m_cs[3].m_leftMax, 1e-2);

} // XmStampInterpCrossSectionUnitTests::test0
//! [snip_test_Example_XmStamper_Test1]
//------------------------------------------------------------------------------
/// \brief Tests symmetric cross section interpolation
//------------------------------------------------------------------------------
void XmStampInterpCrossSectionUnitTests::test1()
{
  XmStamperIo io;
  io.m_centerLine = {{0, 0}, {5, 0}, {10, 0}, {15, 0}, {20, 0}, {25, 0}};
  io.m_cs.assign(6, XmStampCrossSection());
  XmStampCrossSection cs;
  io.m_stampingType = 0;
  cs.m_left = {{0, 10}, {1, 11}, {2, 12}, {4, 11}, {5, 10}, {10, 5}, {15, 0}};
  cs.m_idxLeftShoulder = 0;
  cs.m_right = cs.m_left;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs[1] = cs;
  cs.m_left = {{0, 20}, {4, 19}, {6, 18}, {8, 18}, {10, 20}, {15, 15}, {20, 10}};
  cs.m_idxLeftShoulder = 4;
  cs.m_right = cs.m_left;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs[4] = cs;

  XmStampInterpCrossSectionImpl ip;
  ip.InterpMissingCrossSections(io);
  TS_ASSERT_EQUALS_VEC(io.m_cs[1].m_left, io.m_cs[0].m_left);
  TS_ASSERT_EQUALS(0, io.m_cs[0].m_idxLeftShoulder);
  TS_ASSERT_EQUALS_VEC(io.m_cs[1].m_right, io.m_cs[0].m_right);
  TS_ASSERT_EQUALS(0, io.m_cs[0].m_idxRightShoulder);
  TS_ASSERT_EQUALS_VEC(io.m_cs[4].m_left, io.m_cs[5].m_left);
  TS_ASSERT_EQUALS(4, io.m_cs[5].m_idxLeftShoulder);
  TS_ASSERT_EQUALS_VEC(io.m_cs[4].m_right, io.m_cs[5].m_right);
  TS_ASSERT_EQUALS(4, io.m_cs[5].m_idxRightShoulder);
  VecPt2d baseCs = {{0.00, 13.33}, {1.33, 13.0},  {2.0, 12.67},  {2.67, 12.67},
                    {3.33, 13.33}, {4.22, 13.78}, {5.11, 14.22}, {6.89, 13.11},
                    {7.78, 12.22}, {10.0, 10.0},  {12.22, 7.78}, {16.67, 3.33}};
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[2].m_left, 1e-2);
  TS_ASSERT_EQUALS(4, io.m_cs[2].m_idxLeftShoulder);
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[2].m_right, 1e-2);
  TS_ASSERT_EQUALS(4, io.m_cs[2].m_idxRightShoulder);
  baseCs = {{0.00, 16.67},  {2.67, 16.0},  {4.0, 15.33},   {5.33, 15.33},
            {6.67, 16.67},  {7.44, 16.56}, {8.22, 16.44},  {9.78, 15.22},
            {10.56, 14.44}, {12.5, 12.5},  {14.44, 10.56}, {18.33, 6.67}};
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[3].m_left, 1e-2);
  TS_ASSERT_EQUALS(4, io.m_cs[3].m_idxLeftShoulder);
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[3].m_right, 1e-2);
  TS_ASSERT_EQUALS(4, io.m_cs[3].m_idxRightShoulder);
} // XmStampInterpCrossSectionUnitTests::test0
//! [snip_test_Example_XmStamper_Test1]
//------------------------------------------------------------------------------
/// \brief Tests symetric cross section interpolation
//------------------------------------------------------------------------------
void XmStampInterpCrossSectionUnitTests::testInterpFromCenterlineProfile0()
{
  XmStamperCenterlineProfile p;
  XmStamperIo io;

  p.m_distance = {5, 15, 25};
  p.m_elevation = {100, 150, 200};
  XmStampCrossSection cs;
  cs.m_left = {{0, 10}, {1, 11}, {2, 12}, {4, 11}, {5, 10}, {10, 5}, {15, 0}};
  cs.m_idxLeftShoulder = 0;
  cs.m_right = cs.m_left;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  cs.m_leftMax = cs.m_rightMax = 20;
  p.m_cs.push_back(cs);

  cs.m_left = {{0, 20}, {4, 19}, {6, 18}, {8, 18}, {10, 20}, {15, 15}, {20, 10}};
  cs.m_idxLeftShoulder = 4;
  cs.m_right = cs.m_left;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  cs.m_leftMax = cs.m_rightMax = 25;
  p.m_cs.push_back(cs);

  p.m_cs.push_back(p.m_cs.front());

  io.m_centerLine = {{0, 0}, {10, 0}, {20, 0}, {30, 0}};

  XmStampInterpCrossSectionImpl ip;
  ip.InterpFromCenterlineProfile(io, p);
  VecPt3d basePts = {{0, 0, 100}, {10, 0, 125}, {20, 0, 175}, {30, 0, 200}};
  TS_ASSERT_EQUALS_VEC(basePts, io.m_centerLine);

  {
    XmStampCrossSection &c0(io.m_cs[0]), &c1(p.m_cs[0]);
    TS_ASSERT_EQUALS(c1.m_idxLeftShoulder, c0.m_idxLeftShoulder);
    TS_ASSERT_EQUALS(c1.m_idxRightShoulder, c0.m_idxRightShoulder);
    TS_ASSERT_EQUALS(c1.m_left, c0.m_left);
    TS_ASSERT_EQUALS(c1.m_right, c0.m_right);
    TS_ASSERT_EQUALS(c1.m_leftMax, c0.m_leftMax);
    TS_ASSERT_EQUALS(c1.m_rightMax, c0.m_rightMax);
  }
  {
    XmStampCrossSection &c0(io.m_cs[3]), &c1(p.m_cs[2]);
    TS_ASSERT_EQUALS(c1.m_idxLeftShoulder, c0.m_idxLeftShoulder);
    TS_ASSERT_EQUALS(c1.m_idxRightShoulder, c0.m_idxRightShoulder);
    TS_ASSERT_EQUALS(c1.m_left, c0.m_left);
    TS_ASSERT_EQUALS(c1.m_right, c0.m_right);
    TS_ASSERT_EQUALS(c1.m_leftMax, c0.m_leftMax);
    TS_ASSERT_EQUALS(c1.m_rightMax, c0.m_rightMax);
  }

  VecPt2d csPts = {{0, 15},       {2, 14.5},      {3, 14},       {4, 14},
                   {5, 15},       {5.83, 15.17},  {6.67, 15.33}, {8.33, 14.17},
                   {9.17, 13.33}, {11.25, 11.25}, {13.33, 9.17}, {17.5, 5}};
  XmStampCrossSection oCs;
  oCs.m_left = oCs.m_right = csPts;
  oCs.m_idxLeftShoulder = oCs.m_idxRightShoulder = 4;
  oCs.m_leftMax = oCs.m_rightMax = 22.5;

  {
    XmStampCrossSection &c0(io.m_cs[1]), &c1(oCs);
    TS_ASSERT_EQUALS(c1.m_idxLeftShoulder, c0.m_idxLeftShoulder);
    TS_ASSERT_EQUALS(c1.m_idxRightShoulder, c0.m_idxRightShoulder);
    TS_ASSERT_DELTA_VECPT2D(c1.m_left, c0.m_left, 1e-2);
    TS_ASSERT_DELTA_VECPT2D(c1.m_right, c0.m_right, 1e-2);
    TS_ASSERT_EQUALS(c1.m_leftMax, c0.m_leftMax);
    TS_ASSERT_EQUALS(c1.m_rightMax, c0.m_rightMax);
  }
  {
    XmStampCrossSection &c0(io.m_cs[2]), &c1(oCs);
    TS_ASSERT_EQUALS(c1.m_idxLeftShoulder, c0.m_idxLeftShoulder);
    TS_ASSERT_EQUALS(c1.m_idxRightShoulder, c0.m_idxRightShoulder);
    TS_ASSERT_DELTA_VECPT2D(c1.m_left, c0.m_left, 1e-2);
    TS_ASSERT_DELTA_VECPT2D(c1.m_right, c0.m_right, 1e-2);
    TS_ASSERT_EQUALS(c1.m_leftMax, c0.m_leftMax);
    TS_ASSERT_EQUALS(c1.m_rightMax, c0.m_rightMax);
  }

  // same test with specified begin and end distances same as centerline
  io.m_cs.clear();
  p.m_distance[0] = 0.0;
  p.m_distance[2] = 30.0;
  ip.InterpFromCenterlineProfile(io, p);
  basePts = {{0, 0, 100}, {10, 0, 133.333}, {20, 0, 166.667}, {30, 0, 200}};
  TS_ASSERT_DELTA_VECPT2D(basePts, io.m_centerLine, 1e-2);

  {
    XmStampCrossSection &c0(io.m_cs[0]), &c1(p.m_cs[0]);
    TS_ASSERT_EQUALS(c1.m_idxLeftShoulder, c0.m_idxLeftShoulder);
    TS_ASSERT_EQUALS(c1.m_idxRightShoulder, c0.m_idxRightShoulder);
    TS_ASSERT_EQUALS(c1.m_left, c0.m_left);
    TS_ASSERT_EQUALS(c1.m_right, c0.m_right);
    TS_ASSERT_EQUALS(c1.m_leftMax, c0.m_leftMax);
    TS_ASSERT_EQUALS(c1.m_rightMax, c0.m_rightMax);
  }
  {
    XmStampCrossSection &c0(io.m_cs[3]), &c1(p.m_cs[2]);
    TS_ASSERT_EQUALS(c1.m_idxLeftShoulder, c0.m_idxLeftShoulder);
    TS_ASSERT_EQUALS(c1.m_idxRightShoulder, c0.m_idxRightShoulder);
    TS_ASSERT_EQUALS(c1.m_left, c0.m_left);
    TS_ASSERT_EQUALS(c1.m_right, c0.m_right);
    TS_ASSERT_EQUALS(c1.m_leftMax, c0.m_leftMax);
    TS_ASSERT_EQUALS(c1.m_rightMax, c0.m_rightMax);
  }

  csPts = {{0, 16.67},     {2.67, 16},    {4, 15.33},     {5.33, 15.33},
           {6.67, 16.67},  {7.44, 16.56}, {8.22, 16.44},  {9.78, 15.22},
           {10.56, 14.44}, {12.5, 12.5},  {14.44, 10.56}, {18.33, 6.67}};
  oCs.m_left = oCs.m_right = csPts;
  oCs.m_idxLeftShoulder = oCs.m_idxRightShoulder = 4;
  oCs.m_leftMax = oCs.m_rightMax = 23.33;

  {
    XmStampCrossSection &c0(io.m_cs[1]), &c1(oCs);
    TS_ASSERT_EQUALS(c1.m_idxLeftShoulder, c0.m_idxLeftShoulder);
    TS_ASSERT_EQUALS(c1.m_idxRightShoulder, c0.m_idxRightShoulder);
    TS_ASSERT_DELTA_VECPT2D(c1.m_left, c0.m_left, 1e-2);
    TS_ASSERT_DELTA_VECPT2D(c1.m_right, c0.m_right, 1e-2);
    TS_ASSERT_DELTA(c1.m_leftMax, c0.m_leftMax, 1e-2);
    TS_ASSERT_DELTA(c1.m_rightMax, c0.m_rightMax, 1e-2);
  }
  {
    XmStampCrossSection &c0(io.m_cs[2]), &c1(oCs);
    TS_ASSERT_EQUALS(c1.m_idxLeftShoulder, c0.m_idxLeftShoulder);
    TS_ASSERT_EQUALS(c1.m_idxRightShoulder, c0.m_idxRightShoulder);
    TS_ASSERT_DELTA_VECPT2D(c1.m_left, c0.m_left, 1e-2);
    TS_ASSERT_DELTA_VECPT2D(c1.m_right, c0.m_right, 1e-2);
    TS_ASSERT_DELTA(c1.m_leftMax, c0.m_leftMax, 1e-2);
    TS_ASSERT_DELTA(c1.m_rightMax, c0.m_rightMax, 1e-2);
  }
} // XmStampInterpCrossSectionUnitTests::testInterpFromCenterlineProfile0
//------------------------------------------------------------------------------
/// \brief Test tutorial for symmetric cross section interpolation
//------------------------------------------------------------------------------
//! [snip_test_Example_XmStamper_testCrossSectionTutorial]
void XmStampInterpCrossSectionUnitTests::testCrossSectionTutorial()
{
  XmStamperIo io;
  io.m_centerLine = { { 0, 0 },{ 5, 0 },{ 10, 0 },{ 15, 0 },{ 20, 0 },{ 25, 0 } };
  io.m_cs.assign(6, XmStampCrossSection());
  XmStampCrossSection cs;
  io.m_stampingType = 0;
  cs.m_left = { { 0, 10 },{ 1, 11 },{ 2, 12 },{ 4, 11 },{ 5, 10 },{ 10, 5 },{ 15, 0 } };
  cs.m_idxLeftShoulder = 2;
  cs.m_right = cs.m_left;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs[1] = cs;
  cs.m_left = { { 0, 20 },{ 4, 19 },{ 6, 18 },{ 8, 18 },{ 10, 20 },{ 15, 15 },{ 20, 10 } };
  cs.m_idxLeftShoulder = 3;
  cs.m_right = cs.m_left;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs[4] = cs;

  XmStampInterpCrossSectionImpl ip;
  ip.InterpMissingCrossSections(io);

  TS_ASSERT_EQUALS(2, io.m_cs[0].m_idxLeftShoulder);
  TS_ASSERT_EQUALS(2, io.m_cs[1].m_idxLeftShoulder);
  TS_ASSERT_EQUALS(3, io.m_cs[2].m_idxLeftShoulder);
  TS_ASSERT_EQUALS(3, io.m_cs[3].m_idxLeftShoulder);
  TS_ASSERT_EQUALS(3, io.m_cs[4].m_idxLeftShoulder);
  TS_ASSERT_EQUALS(3, io.m_cs[5].m_idxLeftShoulder);

  TS_ASSERT_EQUALS(2, io.m_cs[0].m_idxRightShoulder);
  TS_ASSERT_EQUALS(2, io.m_cs[1].m_idxRightShoulder);
  TS_ASSERT_EQUALS(3, io.m_cs[2].m_idxRightShoulder);
  TS_ASSERT_EQUALS(3, io.m_cs[3].m_idxRightShoulder);
  TS_ASSERT_EQUALS(3, io.m_cs[4].m_idxRightShoulder);
  TS_ASSERT_EQUALS(3, io.m_cs[5].m_idxRightShoulder);
  
  TS_ASSERT_EQUALS_VEC(io.m_cs[1].m_left, io.m_cs[0].m_left);
  TS_ASSERT_EQUALS_VEC(io.m_cs[1].m_right, io.m_cs[0].m_right);
  TS_ASSERT_EQUALS_VEC(io.m_cs[4].m_left, io.m_cs[5].m_left);
  TS_ASSERT_EQUALS_VEC(io.m_cs[4].m_right, io.m_cs[5].m_right);

  VecPt2d baseCs = { 
    { 0.00, 13.33 },{ 2.00, 13.66 },{ 3.00, 13.66 },{ 4.00, 14.00 },
    { 5.94, 13.94 },{ 6.11, 13.88 },{ 6.92, 13.07 },{ 11.38, 8.61 },
    { 11.79, 8.20 },{ 16.66, 3.33 },
  };
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[2].m_left, 0.02);
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[2].m_right, 0.02);

  baseCs = {
    { 0.00, 16.66 },{ 3.00, 16.33 },{ 4.50, 15.83 },{ 6.00, 16.00 },
    { 7.89, 16.89 },{ 8.05, 16.94 },{ 8.84, 16.15 },{ 13.19, 11.80 },
    { 13.58, 11.41 },{ 18.33, 6.66 },
  };
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[3].m_left, 0.02);
  TS_ASSERT_DELTA_VECPT2D(baseCs, io.m_cs[3].m_right, 0.02);
} // XmStampInterpCrossSectionUnitTests::testCrossSectionTutorial
//! [snip_test_Example_XmStamper_testCrossSectionTutorial]

#endif
