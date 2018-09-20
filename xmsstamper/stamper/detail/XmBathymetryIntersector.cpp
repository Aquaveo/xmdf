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
#include <xmsstamper/stamper/detail/XmBathymetryIntersector.h>

// 3. Standard library headers
#include <cfloat>

// 4. External library headers
#include <boost/make_shared.hpp>

// 5. Shared code headers
#include <xmscore/math/math.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/misc/DynBitset.h>
#include <xmscore/misc/xmstype.h>
#include <xmsinterp/geometry/geoms.h>
#include <xmsinterp/geometry/GmMultiPolyIntersector.h>
#include <xmsinterp/geometry/GmMultiPolyIntersectionSorterTerse.h>
#include <xmsinterp/geometry/GmTriSearch.h>
#include <xmsstamper/stamper/detail/XmStamper3dPts.h>
#include <xmsstamper/stamper/XmStamperIo.h>
#include <xmsinterp/triangulate/TrTin.h>
#include <xmscore/misc/XmConst.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \brief Implementaion of XmBathymetryIntersector
class XmBathymetryIntersectorImpl : public XmBathymetryIntersector
{
public:
  XmBathymetryIntersectorImpl(BSHP<TrTin> a_tin, BSHP<TrTin> a_stamp);
  ~XmBathymetryIntersectorImpl();

  virtual void IntersectCenterLine(XmStamperIo& a_io) override;
  virtual void DecomposeCenterLine(XmStamperIo& a_io, std::vector<XmStamperIo>& a_vIo) override;
  virtual void IntersectXsects(XmStamper3dPts& a_pts) override;
  virtual void IntersectEndCaps(XmStamperIo& a_io, XmStamper3dPts& a_pts) override;

  void ClassifyPoints(VecPt3d& a_pts, VecInt& a_ptLocation);
  void CreateIntersector();
  void Intersect3dPts(VecPt3d& a_pts);
  void IntersectXsectSide(VecPt3d& a_cl, VecPt3d2d& a_side);
  void IntersectSlopedAbutment(XmStamperIo& a_io, XmStamper3dPts& a_pts, bool a_first);
  void IntersectGuideBank(XmStamperIo& a_io, XmStamper3dPts& a_pts, bool a_first);

  BSHP<TrTin> m_tin;   ///< TIN defining Bathemetry surface
  BSHP<TrTin> m_stamp; ///< TIN of the stamp
  Pt3d m_min;          ///< min x,y,z of stamp
  Pt3d m_max;          ///< max x,y,z of stamp
  BSHP<GmMultiPolyIntersector>
    m_intersect;   ///< polygon intersector for intersecting objects with the bathemetry TIN
  VecInt m_triIds; ///< the ids of the triangles in the intersector
  double m_xyTol;  ///< xy tolerance for geometry comparisons
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmBathymetryIntersectorImpl
/// \brief Intersects bathemetry with feature stamp
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
/// \param a_tin The TIN that is intersected with cross sections and the
/// center line of the stamp.
/// \param[in] a_stamp TIN of the stamp
//------------------------------------------------------------------------------
XmBathymetryIntersectorImpl::XmBathymetryIntersectorImpl(BSHP<TrTin> a_tin, BSHP<TrTin> a_stamp)
: m_tin(a_tin)
, m_stamp(a_stamp)
, m_xyTol(1e-9)
{
  if (m_tin)
  {
    Pt3d pMin, pMax;
    m_tin->GetExtents(pMin, pMax);
    double len = Mdist(pMin.x, pMin.y, pMax.x, pMax.y);
    m_xyTol = len * 1e-9;
  }
} // XmBathymetryIntersectorImpl::XmBathymetryIntersectorImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmBathymetryIntersectorImpl::~XmBathymetryIntersectorImpl()
{
} // XmBathymetryIntersectorImpl::~XmBathymetryIntersectorImpl
//------------------------------------------------------------------------------
/// \brief Intersects the center line from a feature stamp operation with
/// the bathemetry. This can potentially create new points along the center
/// line.
/// \param[in,out] a_io XmStamperIo class used in the feature stamp operation
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::IntersectCenterLine(XmStamperIo& a_io)
{
  // get the bounds of the center line
  if (!m_intersect)
    CreateIntersector();
  if (!m_intersect)
    return;
  // intersect the center line in 2d with the tin
  VecPt3d& pts(m_tin->Points());
  VecInt& tris(m_tin->Triangles());
  VecInt triIds;
  VecDbl tVals;
  VecPt3d &line(a_io.m_centerLine), line1;
  std::vector<XmStampCrossSection> vXs, ioXs(a_io.m_cs);
  if (ioXs.size() != line.size())
    ioXs.resize(line.size());
  for (size_t i = 1; i < line.size(); ++i)
  {
    Pt3d &p0(line[i - 1]), &p1(line[i]);
    line1.push_back(p0);
    vXs.push_back(ioXs[i - 1]);
    m_intersect->TraverseLineSegment(p0.x, p0.y, p1.x, p1.y, triIds, tVals);
    for (size_t j = 0; j < triIds.size(); ++j)
    {
      if (triIds[j] < 0)
        continue;
      // get the triangle id for the triangle in the TIN
      int idx = m_triIds[triIds[j] - 1];
      // do a 3d intersection with the segment and the triangle
      int idx0(tris[idx]), idx1(tris[idx + 1]), idx2(tris[idx + 2]);
      Pt3d iPt;
      int rval = gmIntersectTriangleAndLineSegment(p0, p1, pts[idx0], pts[idx1], pts[idx2], iPt);
      if (1 == rval)
      {
        if (!gmEqualPointsXY(p0, iPt, m_xyTol) && !gmEqualPointsXY(p1, iPt, m_xyTol) &&
            !gmEqualPointsXY(line1.back(), iPt, m_xyTol))
        {
          line1.push_back(iPt);
          vXs.push_back(XmStampCrossSection());
        }
      }
    }
  }
  if (line.size() > 1)
  {
    line1.push_back(line.back());
    vXs.push_back(ioXs.back());
  }
  line.swap(line1);
  a_io.m_cs.swap(vXs);
} // XmBathymetryIntersectorImpl::IntersectCenterLine
//------------------------------------------------------------------------------
/// \brief Intersects the center line from a feature stamp operation with
/// the bathemetry. This can potentially create new points along the center
/// line.
/// \param[in] a_io XmStamperIo class used in the feature stamp operation
/// \param[out] a_vIo array of Io classes that have split up the centerline
/// based on where it intersects the bathemetry and removes sections of the
/// center line based on the type of stamp: cut/fill.
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::DecomposeCenterLine(XmStamperIo& a_io,
                                                      std::vector<XmStamperIo>& a_vIo)
{
  a_vIo.resize(0);
  VecInt ptLoc;
  // classify the points of the center line as 1,0,-1,-2 (above, on, below, out)
  ClassifyPoints(a_io.m_centerLine, ptLoc);

  // check if all the points are above or below
  bool allAbove(1);
  for (auto& i : ptLoc)
  {
    if (i != 1 || i != -2)
      allAbove = false;
  }
  // if this is a cut and all points are above then abort
  if (allAbove && a_io.m_stampingType == 0)
    return;

  bool allBelow(1);
  for (auto& i : ptLoc)
  {
    if (i != -1 || i != -2)
      allBelow = false;
  }
  // if this is a fill and all points are below then abort
  if (allBelow && a_io.m_stampingType == 1)
    return;

  // decompose the center line

  bool done(false);
  size_t start(0);
  size_t lastIdx(a_io.m_centerLine.size() - 1);
  ;
  while (!done)
  {
    VecPt3d& cl(a_io.m_centerLine);

    // find the intersection
    size_t idx(0);
    for (size_t i = start + 1; idx == 0 && i < ptLoc.size(); ++i)
    {
      if (ptLoc[i] == 0)
        idx = i;
    }
    if (0 == idx)
      idx = lastIdx;
    // check the location of the segment connected to the intersection relative
    // to the TIN
    Pt3d &p0(cl[start]), &p1(cl[idx]), pt;
    if (idx == lastIdx)
      pt = p0 - ((p0 - p1) * 10 * m_xyTol);
    else
      pt = p1 - ((p1 - p0) * 10 * m_xyTol);
    VecPt3d vPt(1, pt);
    VecInt vPtLoc;
    ClassifyPoints(vPt, vPtLoc);

    if ( // location is above and we are doing a cut
      (vPtLoc[0] == 1 && a_io.m_stampingType == 0)
      // location is below and we are doing a fill
      || (vPtLoc[0] == -1 && a_io.m_stampingType == 1))
    { // skip this section of the center line
    }
    else
    { // start and the beginning of the center line
      XmStamperIo io = a_io;
      io.m_centerLine.resize(idx - start + 1);
      io.m_cs.resize(idx - start + 1);
      if (start != 0)
        a_io.m_firstEndCap = XmStamperEndCap(); // remove the first end cap
      if (idx != lastIdx)
        io.m_lastEndCap = XmStamperEndCap(); // remove the last end cap
      for (size_t j = 0, i = start; i <= idx; ++i, ++j)
      {
        io.m_centerLine[j] = a_io.m_centerLine[i];
        io.m_cs[j] = a_io.m_cs[i];
      }
      a_vIo.push_back(io);
    }
    start = idx;
    if (start == lastIdx)
      done = true;
  }

} // XmBathymetryIntersectorImpl::DecomposeCenterLine
//------------------------------------------------------------------------------
/// \brief Intersects the center line from a feature stamp operation with
/// the bathemetry. This can potentially create new points along the center
/// line.
/// \param[in] a_pts point locations
/// \param[out] a_ptLocation the location of the point relative to the TIN
/// 1 (above), 0 (on), -1 (below) OR -2 (outside)
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::ClassifyPoints(VecPt3d& a_pts, VecInt& a_ptLocation)
{
  a_ptLocation.assign(a_pts.size(), -2);
  if (!m_intersect)
    CreateIntersector();
  if (!m_intersect)
    return;
  VecInt triIds;
  VecDbl tVals;
  VecPt3d& pts(m_tin->Points());
  VecInt& tris(m_tin->Triangles());
  for (size_t i = 0; i < a_pts.size(); ++i)
  {
    Pt3d& p0(a_pts[i]);
    // get the triangle with the point
    m_intersect->TraverseLineSegment(p0.x, p0.y, p0.x, p0.y, triIds, tVals);
    if (!triIds.empty())
    {
      // get the triangle id for the triangle in the TIN
      int idx = m_triIds[triIds.front() - 1];
      // get the points that make the triangle
      int idx0(tris[idx]), idx1(tris[idx + 1]), idx2(tris[idx + 2]);
      // interpolate the z from the triangle
      Pt3d nrm(0, 0, 1), orig, bary, &pt0(pts[idx0]), &pt1(pts[idx1]), &pt2(pts[idx2]);
      double coef[6];
      int dir;
      gmBaryPrepare(&pt0, &pt1, &pt2, &nrm, &orig, coef, &dir, true);
      gmCartToBary(&p0, &orig, coef, dir, &bary);
      double interpZ = pt0.z * bary.x + pt1.z * bary.y + pt2.z * bary.z;
      if (EQ_TOL(interpZ, p0.z, FLT_EPSILON))
        a_ptLocation[i] = 0;
      else if (interpZ > p0.z)
        a_ptLocation[i] = -1;
      else
        a_ptLocation[i] = 1;
    }
  }
} // XmBathymetryIntersectorImpl::ClassifyPoints
//------------------------------------------------------------------------------
/// \brief Intersects cross section points. When a cross section intersects
/// the bathemetry it stops at that location and the rest of the cross section
/// is discarded.
/// \param[in,out] a_pts 3d points representing the cross sections
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::IntersectXsects(XmStamper3dPts& a_pts)
{
  if (!m_intersect)
    CreateIntersector();
  if (!m_intersect)
    return;

  IntersectXsectSide(a_pts.m_xsPts.m_centerLine, a_pts.m_xsPts.m_left);
  IntersectXsectSide(a_pts.m_xsPts.m_centerLine, a_pts.m_xsPts.m_right);
} // XmBathymetryIntersectorImpl::IntersectXsects
//------------------------------------------------------------------------------
/// \brief Intersects end cap cross section points. When a cross section
/// intersects the bathemetry it stops at that location and the rest of the
/// cross section is discarded.
/// \param[in] a_io Stamper io class
/// \param[in,out] a_pts 3d points representing the cross sections
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::IntersectEndCaps(XmStamperIo& a_io, XmStamper3dPts& a_pts)
{
  XM_ENSURE_TRUE(!a_io.m_cs.empty());
  if (!m_intersect)
    CreateIntersector();
  if (!m_intersect)
    return;

  int type = a_io.m_firstEndCap.m_type;
  if (type == 1)
    IntersectSlopedAbutment(a_io, a_pts, true);
  else if (type == 0)
    IntersectGuideBank(a_io, a_pts, true);

  type = a_io.m_lastEndCap.m_type;
  if (type == 1)
    IntersectSlopedAbutment(a_io, a_pts, false);
  else if (type == 0)
    IntersectGuideBank(a_io, a_pts, false);
} // XmBathymetryIntersectorImpl::IntersectEndCaps
//------------------------------------------------------------------------------
/// \brief Intersects left or right side of a cross section with bathymetry
/// \param a_cl: ???
/// \param a_side: ???
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::IntersectXsectSide(VecPt3d& a_cl, VecPt3d2d& a_side)
{
  VecPt3d2d& xs(a_side);
  if (xs.empty())
    return;
  VecPt3d& cl(a_cl);
  XM_ENSURE_TRUE(xs.size() == cl.size());
  for (size_t i = 0; i < cl.size(); ++i)
  {
    VecPt3d line;
    line.reserve(xs[i].size() + 1);
    line.push_back(cl[i]);
    for (auto& p : xs[i])
      line.push_back(p);
    Intersect3dPts(line);
    line.erase(line.begin());
    xs[i] = line;
  }
} // XmBathymetryIntersectorImpl::IntersectXsectSide
//------------------------------------------------------------------------------
/// \brief Creates a multi poly intersector if one does not exist
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::CreateIntersector()
{
  if (m_intersect)
    m_intersect.reset();

  const VecPt3d& pts(m_tin->Points());
  VecInt &tris(m_tin->Triangles()), vTri(3, 0);

  DynBitset ptFlags;
  ptFlags.resize(pts.size(), true);

  // classify points
  BSHP<GmTriSearch> tSearch = GmTriSearch::New();
  if (m_stamp)
  {
    BSHP<VecInt> tPtr(new VecInt());
    {
      VecInt& tmp(m_stamp->Triangles());
      tPtr->reserve(tmp.size());
      for (const auto& t : tmp)
        tPtr->push_back((int)t);
    }
    tSearch->TrisToSearch(m_stamp->PointsPtr(), tPtr);
    for (size_t i = 0; i < pts.size(); ++i)
    {
      if (XM_NONE == tSearch->TriContainingPt(pts[i]))
        ptFlags[i] = 0;
    }
  }

  // classify triangles
  Pt3d pMin, pMax;
  VecInt tIdxes;
  int triCount(0);
  DynBitset triFlags;
  triFlags.resize(tris.size() / 3, 0);
  for (size_t i = 0, cnt = 0; i < tris.size(); i += 3, ++cnt)
  {
    int ix0(tris[i + 0]), ix1(tris[i + 1]), ix2(tris[i + 2]);
    if (ptFlags[ix0] || ptFlags[ix1] || ptFlags[ix2])
    {
      triFlags[cnt] = 1;
      ++triCount;
    }
    else if (!triFlags[cnt])
    { // get the triangle bounding box
      pMin = XM_DBL_HIGHEST;
      pMax = XM_DBL_LOWEST;
      gmAddToExtents(pts[ix0], pMin, pMax);
      gmAddToExtents(pts[ix1], pMin, pMax);
      gmAddToExtents(pts[ix2], pMin, pMax);
      tSearch->TriEnvelopesOverlap(pMin, pMax, tIdxes);
      if (!tIdxes.empty())
      {
        triFlags[cnt] = 1;
        ++triCount;
      }
    }
  }

  VecInt2d polys(triCount, vTri);
  m_triIds.resize(triCount);
  for (size_t i = 0, cnt = 0; i < tris.size(); i += 3)
  {
    if (!triFlags[i / 3])
      continue;

    // see if the boxes overlap
    // Pt3d pMin(XM_DBL_HIGHEST), pMax(XM_DBL_LOWEST);
    // gmAddToExtents(pts[tris[i + 0]], pMin, pMax);
    // gmAddToExtents(pts[tris[i + 1]], pMin, pMax);
    // gmAddToExtents(pts[tris[i + 2]], pMin, pMax);
    // if (!gmBoxesOverlap2d(m_min, m_max, pMin, pMax)) continue;

    m_triIds[cnt] = (int)i;
    // polys.push_back(vTri);
    // polys.back()[0] = tris[i + 0];
    // polys.back()[1] = tris[i + 1];
    // polys.back()[2] = tris[i + 2];

    polys[cnt][0] = tris[i + 0];
    polys[cnt][1] = tris[i + 1];
    polys[cnt][2] = tris[i + 2];
    ++cnt;
  }

  BSHP<GmMultiPolyIntersectionSorterTerse> sorterTerse =
    boost::make_shared<GmMultiPolyIntersectionSorterTerse>();
  BSHP<GmMultiPolyIntersectionSorter> sorter = BDPC<GmMultiPolyIntersectionSorter>(sorterTerse);
  m_intersect = GmMultiPolyIntersector::New(pts, polys, sorter);
} // XmBathymetryIntersectorImpl::CreateIntersector
//------------------------------------------------------------------------------
/// \brief Intersects a line with a surface
/// \param a_pts: ???
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::Intersect3dPts(VecPt3d& a_pts)
{
  VecPt3d& pts(m_tin->Points());
  VecInt& tris(m_tin->Triangles());
  VecInt triIds;
  VecDbl tVals;
  VecPt3d &line(a_pts), line1;
  bool done = false;
  if (line.size() > 0)
    line1.push_back(line[0]);
  for (size_t i = 1; !done && i < line.size(); ++i)
  {
    Pt3d &p0(line[i - 1]), &p1(line[i]);
    m_intersect->TraverseLineSegment(p0.x, p0.y, p1.x, p1.y, triIds, tVals);
    for (size_t j = 0; !done && j < triIds.size(); ++j)
    {
      if (triIds[j] < 0)
        continue;

      // get the triangle id for the triangle in the TIN
      int idx = m_triIds[triIds[j] - 1];
      // do a 3d intersection with the segment and the triangle
      int idx0(tris[idx]), idx1(tris[idx + 1]), idx2(tris[idx + 2]);
      Pt3d iPt;
      Pt3d &t0(pts[idx0]), &t1(pts[idx1]), &t2(pts[idx2]);
      int rval = gmIntersectTriangleAndLineSegment(p0, p1, t0, t1, t2, iPt);
      if (1 == rval)
      {
        if (!gmEqualPointsXY(p0, iPt, m_xyTol))
          line1.push_back(iPt);
        done = true;
      }
    }
    if (!done)
      line1.push_back(p1);
  }
  line.swap(line1);
} // XmBathymetryIntersectorImpl::Intersect3dPts
//------------------------------------------------------------------------------
/// \brief Intersects sloped abutment end cap cross section points.
/// \param[in] a_io Stamper io class
/// \param[in,out] a_pts 3d points representing the cross sections
/// \param[in] a_first flag indicating if this is the first end cap
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::IntersectSlopedAbutment(XmStamperIo& a_io,
                                                          XmStamper3dPts& a_pts,
                                                          bool a_first)
{
  int leftShoulder = a_io.m_cs[0].m_idxLeftShoulder;
  int rightShoulder = a_io.m_cs[0].m_idxRightShoulder;
  Pt3d clPt(a_io.m_centerLine[0]);
  VecPt3d *xsLeft(&a_pts.m_xsPts.m_left[0]), *xsRight(&a_pts.m_xsPts.m_right[0]);
  VecPt3d2d *lPtr(&a_pts.m_first_endcap.m_left), *rPtr(&a_pts.m_first_endcap.m_right);
  if (!a_first)
  {
    leftShoulder = a_io.m_cs.back().m_idxLeftShoulder;
    rightShoulder = a_io.m_cs.back().m_idxRightShoulder;
    clPt = a_io.m_centerLine.back();
    xsLeft = &a_pts.m_xsPts.m_left.back();
    xsRight = &a_pts.m_xsPts.m_right.back();
    lPtr = &a_pts.m_last_endcap.m_left;
    rPtr = &a_pts.m_last_endcap.m_right;
  }

  {
    Pt3d ls(clPt);
    if (leftShoulder - 1 < (int)xsLeft->size())
      ls = (*xsLeft)[leftShoulder - 1];

    VecPt3d cl(lPtr->size(), ls);
    IntersectXsectSide(cl, *lPtr);
  }
  {
    Pt3d rs(clPt);
    if (rightShoulder - 1 < (int)xsRight->size())
      rs = (*xsRight)[rightShoulder - 1];

    VecPt3d cl(rPtr->size(), rs);
    IntersectXsectSide(cl, *rPtr);
  }
} // XmBathymetryIntersectorImpl::IntersectSlopedAbutment
//------------------------------------------------------------------------------
/// \brief Intersects sloped abutment end cap cross section points.
/// \param[in] a_io Xmamper io class
/// \param[in,out] a_pts 3d points representing the cross sections
/// \param[in] a_first flag indicating if this is the first end cap
//------------------------------------------------------------------------------
void XmBathymetryIntersectorImpl::IntersectGuideBank(XmStamperIo& a_io,
                                                     XmStamper3dPts& a_pts,
                                                     bool a_first)
{
  int leftShoulder = a_io.m_cs[0].m_idxLeftShoulder;
  int rightShoulder = a_io.m_cs[0].m_idxRightShoulder;
  Pt3d clPt(a_io.m_centerLine[0]);
  VecPt3d *xsLeft(&a_pts.m_xsPts.m_left[0]), *xsRight(&a_pts.m_xsPts.m_right[0]),
    *clPtr(&a_pts.m_first_endcap.m_centerLine);
  VecPt3d2d *lPtr(&a_pts.m_first_endcap.m_left), *rPtr(&a_pts.m_first_endcap.m_right);
  if (!a_first)
  {
    leftShoulder = a_io.m_cs.back().m_idxLeftShoulder;
    rightShoulder = a_io.m_cs.back().m_idxRightShoulder;
    clPt = a_io.m_centerLine.back();
    xsLeft = &a_pts.m_xsPts.m_left.back();
    xsRight = &a_pts.m_xsPts.m_right.back();
    clPtr = &a_pts.m_last_endcap.m_centerLine;
    lPtr = &a_pts.m_last_endcap.m_left;
    rPtr = &a_pts.m_last_endcap.m_right;
  }

  // first cut off the guidebank any where the center line intersects the
  // bathymetry
  VecPt3d cl(1, clPt);
  cl.insert(cl.end(), clPtr->begin(), clPtr->end());
  Intersect3dPts(cl);
  if (cl.back() != clPtr->back())
  {
    size_t s(0);
    if (cl.size() > 2)
      s = cl.size() - 2;
    clPtr->resize(s);
    lPtr->resize(s);
    rPtr->resize(s);
  }
  if (clPtr->empty())
    return;

  // intersect cross sections
  VecPt3d vCl(*clPtr);
  vCl.resize(lPtr->size(), vCl.back());
  IntersectXsectSide(vCl, *lPtr);    // left side
  IntersectXsectSide(*clPtr, *rPtr); // right side

} // XmBathymetryIntersectorImpl::IntersectGuideBank

//------------------------------------------------------------------------------
/// \brief Creates a XmStampInterpCrossSection class
/// \param[in] a_tin The tin defining the bathymetry
/// \param[in] a_stamp The tin defined by the stamp
/// \return Shared ptr to a BathymetryIntersector
//------------------------------------------------------------------------------
BSHP<XmBathymetryIntersector> XmBathymetryIntersector::New(BSHP<TrTin> a_tin, BSHP<TrTin> a_stamp)
{
  BSHP<XmBathymetryIntersector> p(new XmBathymetryIntersectorImpl(a_tin, a_stamp));
  return p;
} // XmBathymetryIntersector::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmBathymetryIntersector::XmBathymetryIntersector()
{
} // XmBathymetryIntersector::XmBathymetryIntersector
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
XmBathymetryIntersector::~XmBathymetryIntersector()
{
} // XmBathymetryIntersector::~XmBathymetryIntersector

} // namespace xms

#ifdef CXX_TEST
//------------------------------------------------------------------------------
// Unit Tests
//------------------------------------------------------------------------------
using namespace xms;
#include <xmsstamper/stamper/detail/XmBathymetryIntersector.t.h>

#include <xmscore/testing/TestTools.h>
#include <xmsinterp/triangulate/TrTin.t.h>

//------------------------------------------------------------------------------
/// \brief Tests XmBathymetryIntersectorUnitTests
//------------------------------------------------------------------------------
void XmBathymetryIntersectorUnitTests::testCreateClass()
{
  BSHP<TrTin> t, s;
  Pt3d pMin, pMax;
  BSHP<XmBathymetryIntersector> p = XmBathymetryIntersector::New(t, s);
  TS_ASSERT(p);
} // stXmampInterpCrossSectionTests::testCreateClass
//------------------------------------------------------------------------------
/// \brief Tests Intersecting center line with TIN
//------------------------------------------------------------------------------
void XmBathymetryIntersectorUnitTests::testIntersectCenterLine()
{
  BSHP<TrTin> tin = trBuildTestTin(), stTin;
  VecPt3d& pts(tin->Points());
  pts[3].z = pts[4].z = pts[7].z = pts[8].z = 10.0;
  XmStamperIo io;
  io.m_centerLine = {{7.5, -1, 5}, {7.5, 9, 5}, {15, 6, 5}};

  XmBathymetryIntersectorImpl b(tin, stTin);
  b.IntersectCenterLine(io);
  VecPt3d basePts = {{7.5, -1, 5}, {7.5, 2.5, 5}, {7.5, 9, 5}, {12.5, 7, 5}, {15, 6, 5}};
  double tol(1e-2);
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_centerLine, tol);
} // stXmampInterpCrossSectionTests::testIntersectCenterLine
//------------------------------------------------------------------------------
/// \brief Tests Intersecting cross sections with TIN
//------------------------------------------------------------------------------
void XmBathymetryIntersectorUnitTests::testIntersectXsects()
{
  BSHP<TrTin> tin = trBuildTestTin(), stTin;
  VecPt3d& pts(tin->Points());
  pts[3].z = pts[4].z = pts[7].z = pts[8].z = 10.0;

  XmStamper3dPts xpts;
  xpts.m_xsPts.m_centerLine = {{5, 9, 11}};
  xpts.m_xsPts.m_left.push_back(VecPt3d());
  xpts.m_xsPts.m_left[0] = {{6, 9, 11}, {7, 9, 11}, {8, 9, 11}, {9, 9, 9}};

  XmBathymetryIntersectorImpl b(tin, stTin);
  b.IntersectXsects(xpts);
  VecPt3d basePts = {{6, 9, 11}, {7, 9, 11}, {8, 9, 11}, {8.5, 9, 10}};
  double tol(1e-2);
  TS_ASSERT_DELTA_VECPT3D(basePts, xpts.m_xsPts.m_left[0], tol);
} // XmBathymetryIntersectorUnitTests::testIntersectXsects
//------------------------------------------------------------------------------
/// \brief Tests point classification compared to the TIN (on, below, above,
/// outside)
//------------------------------------------------------------------------------
void XmBathymetryIntersectorUnitTests::testClassifyPoints()
{
  BSHP<TrTin> tin = trBuildTestTin(), stTin;
  XmBathymetryIntersectorImpl b(tin, stTin);
  VecPt3d pts = {{0, 0, 0}, {4, 6, 0}, {5, 5, 11}, {6, 6, -1}, {9, 9, 9.9}};
  VecInt ptLoc;
  b.ClassifyPoints(pts, ptLoc);
  VecInt baseLoc = {-2, 0, 1, -2, 1};
  TS_ASSERT_EQUALS_VEC(baseLoc, ptLoc);

  {
    VecPt3d& pts(tin->Points());
    pts[3].z = pts[4].z = pts[7].z = pts[8].z = 10.0;
  }
  b.ClassifyPoints(pts, ptLoc);
  baseLoc = {-2, -1, 1, -2, -1};
  TS_ASSERT_EQUALS_VEC(baseLoc, ptLoc);

} // XmBathymetryIntersectorUnitTests::testClassifyPoints
//------------------------------------------------------------------------------
/// \brief Splits up a center line based on intersections
//------------------------------------------------------------------------------
void XmBathymetryIntersectorUnitTests::testDescomposeCenterLine()
{
  BSHP<TrTin> tin = trBuildTestTin(), stTin;
  VecPt3d& pts(tin->Points());
  pts[1].z = 10;

  XmStamperIo io;
  io.m_centerLine = {{5, -1, 5}, {11, 4, 5}};
  io.m_cs.assign(io.m_centerLine.size(), XmStampCrossSection());

  XmBathymetryIntersectorImpl b(tin, stTin);
  b.IntersectCenterLine(io);
  std::vector<XmStamperIo> vIo;
  b.DecomposeCenterLine(io, vIo);
  TS_ASSERT_EQUALS(1, vIo.size());
  if (vIo.size() == 1)
  {
    VecPt3d basePts = {{7.5, 1.083, 5}, {9.199, 2.5, 5}};
    TS_ASSERT_DELTA_VECPT3D(basePts, vIo[0].m_centerLine, 1e-3);
  }

  vIo.clear();
  io.m_stampingType = 1;
  b.DecomposeCenterLine(io, vIo);
  TS_ASSERT_EQUALS(2, vIo.size());
  if (vIo.size() == 2)
  {
    VecPt3d basePts = {{5, -1, 5}, {7.5, 1.083, 5}};
    TS_ASSERT_DELTA_VECPT3D(basePts, vIo[0].m_centerLine, 1e-3);
    basePts = {{9.199, 2.5, 5}, {11, 4, 5}};
    TS_ASSERT_DELTA_VECPT3D(basePts, vIo[1].m_centerLine, 1e-3);
  }

  vIo.clear();
  io.m_stampingType = 1; // fill
  io.m_centerLine = {{6, 2, -1}, {11, 4, -1}};
  b.DecomposeCenterLine(io, vIo);
  TS_ASSERT_EQUALS(0, vIo.size());

  io.m_stampingType = 0; // cut
  b.DecomposeCenterLine(io, vIo);
  TS_ASSERT_EQUALS(1, vIo.size());

  vIo.clear();
  io.m_centerLine[0].z = 50; // set elevations above bathymetry
  io.m_centerLine[1].z = 50;
  b.DecomposeCenterLine(io, vIo);
  TS_ASSERT_EQUALS(0, vIo.size());

} // XmBathymetryIntersectorUnitTests::testDescomposeCenterLine

#endif
