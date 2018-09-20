//------------------------------------------------------------------------------
/// \file
/// \ingroup stamping
/// \brief Implements serialization of the data classes in XmStamperIo.h
//
/// \copyright (C) Copyright Aquaveo 2018.
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header

// 3. Standard library headers

// 4. External library headers

// 5. Shared code headers

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

#ifdef CXX_TEST
//------------------------------------------------------------------------------
// Unit Tests
//------------------------------------------------------------------------------
#include <xmsstamper/stamper/detail/XmStampTests.t.h>

#include <fstream>

#include <xmscore/testing/TestTools.h>

#include <xmsstamper/stamper/XmStamper.h>
#include <xmsstamper/stamper/XmStamperIo.h>
#include <xmsinterp/triangulate/TrTin.h>
#include <xmscore/misc/environment.h>

using namespace xms;
namespace
{
//------------------------------------------------------------------------------
/// \brief Builds a XmStamperIo class from an archive
//------------------------------------------------------------------------------
static void iBuildStamperIo(const std::string& a_path, XmStamperIo& a_io)
{
  std::string fname(a_path + "xmsng_StamperIo.txt"), line;
  line.reserve(40960);
  std::ifstream is(fname.c_str());
  char buffer[4096];
  while (is.read(buffer, sizeof(buffer)))
  {
    line.append(buffer, (size_t)is.gcount());
  }
  line.append(buffer, (size_t)is.gcount());
  // std::getline(is, line);
  a_io.FromString(line);
} // iBuildStamperIo
//------------------------------------------------------------------------------
/// \brief Writes a XmStamperIo output to a file
//------------------------------------------------------------------------------
static void iOutputToFile(const std::string& a_fileName, XmStamperIo a_io)
{
  XmStamperIo io;
  io.m_outBreakLines = a_io.m_outBreakLines;
  io.m_outTin = a_io.m_outTin;
  std::fstream os(a_fileName.c_str(), std::ios_base::out);
  os << io.ToString();
} // iOutputToFile
//------------------------------------------------------------------------------
/// \brief Writes a tin to a file
//------------------------------------------------------------------------------
static void iDoTest(const std::string& a_relPath)
{
  std::string path(XMSSTAMPER_TEST_PATH);
  path += "stamping/" + a_relPath;
  XmStamperIo io;
  iBuildStamperIo(path, io);
  BSHP<XmStamper> s = XmStamper::New();
  s->DoStamp(io);
  std::string baseFile(path + "xmsng_base.txt");
#if defined(ENV64BIT)
  std::string b1(path + "xmsng_base64.txt");
  FILE* fp(fopen(b1.c_str(), "r"));
  if (fp)
  {
    fclose(fp);
    baseFile = b1;
  }
#elif defined(ENV32BIT)
  // ensure either ENV32BIT or ENV64BIT is defined
#else
#error "Must define either ENV32BIT or ENV64BIT"
#endif
  std::string outFile(path + "xmsng_out.txt");
  iOutputToFile(outFile, io);
  TS_ASSERT_TXT_FILES_EQUAL(baseFile, outFile);
} // iDoTest

} //  unnamed namespace

//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_WingWall01()
{
  iDoTest("test_WingWall01/");
} // XmStampIntermediateTests::test_WingWall01
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_WingWall02()
{
  iDoTest("test_WingWall02/");
} // XmStampIntermediateTests::test_WingWall02
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_WingWall03()
{
  iDoTest("test_WingWall03/");
} // XmStampIntermediateTests::test_WingWall03
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Tests a cut stamp
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_wingWall04_cut()
{
  iDoTest("test_wingWall04_cut/");
} // XmStampIntermediateTests::test_wingWall04_cut
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_SlopedAbutment01()
{
  iDoTest("test_SlopedAbutment01/");
} // XmStampIntermediateTests::test_SlopedAbutment01
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_SlopedAbutment02()
{
  iDoTest("test_SlopedAbutment02/");
} // XmStampIntermediateTests::test_SlopedAbutment02
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_SlopedAbutment03()
{
  iDoTest("test_SlopedAbutment03/");
} // XmStampIntermediateTests::test_SlopedAbutment03
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_SlopedAbutment04_cut()
{
  iDoTest("test_SlopedAbutment04_cut/");
} // XmStampIntermediateTests::test_SlopedAbutment04_cut
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_SelfIntersect()
{
  std::string path(XMSSTAMPER_TEST_PATH);
  path += "/stamping/test_intersections/";
  XmStamperIo io;
  iBuildStamperIo(path, io);
  BSHP<XmStamper> s = XmStamper::New();
  s->DoStamp(io);
  TS_ASSERT(!io.m_outTin);
  TS_ASSERT(io.m_outBreakLines.empty());
  // clear the warning
  TS_ASSERT_STACKED_ERRORS(
    "---Intersection found in stamp outputs. Stamping operation aborted.\n\n");
} // XmStampIntermediateTests::test_SelfIntersect
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_GuideBank01()
{
  iDoTest("test_GuideBank01/");
} // XmStampIntermediateTests::test_GuideBank01
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_GuideBank02()
{
  iDoTest("test_GuideBank02/");
} // XmStampIntermediateTests::test_GuideBank02
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_GuideBank03()
{
  iDoTest("test_GuideBank03/");
} // XmStampIntermediateTests::test_GuideBank03
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_GuideBank04()
{
  iDoTest("test_GuideBank04/");
} // XmStampIntermediateTests::test_GuideBank04
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_GuideBank05()
{
  iDoTest("test_GuideBank05/");
} // XmStampIntermediateTests::test_GuideBank05
//------------------------------------------------------------------------------
/// \brief Tests stamp operation
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_GuideBank06()
{
  iDoTest("test_GuideBank06/");
} // XmStampIntermediateTests::test_GuideBank06
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. angle on the guidebank end cap
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_GuideBank07()
{
  iDoTest("test_GuideBank07/");
} // XmStampIntermediateTests::test_GuideBank07
//------------------------------------------------------------------------------
/// \brief Tests stamp operation.
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_GuideBank08_cut()
{
  iDoTest("test_GuideBank08_cut/");
} // XmStampIntermediateTests::test_GuideBank08_cut
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Intersection between shoulder and outer
/// edge of a cross section.
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry01()
{
  iDoTest("test_intersectBathymetry01/");
} // XmStampIntermediateTests::test_IntersectBathymetry01
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Intersection between center line and shoulder
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry02()
{
  iDoTest("test_intersectBathymetry02/");
} // XmStampIntermediateTests::test_IntersectBathymetry02
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Intersection between center line and shoulder
/// with a more complex cross section
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry03()
{
  iDoTest("test_intersectBathymetry03/");
} // XmStampIntermediateTests::test_IntersectBathymetry03
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Center line intersects bathymetry
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry04()
{
  iDoTest("test_intersectBathymetry04/");
} // XmStampIntermediateTests::test_IntersectBathymetry04
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Center line intersects bathymetry twice
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry05()
{
  iDoTest("test_intersectBathymetry05/");
} // XmStampIntermediateTests::test_IntersectBathymetry05
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Both end caps are sloped abutments and they
/// intersect the bathymetry
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry06()
{
  iDoTest("test_intersectBathymetry06/");
} // XmStampIntermediateTests::test_IntersectBathymetry06
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Intersection with guidebank centerline.
/// Cuts off the guidebank so no guidebank is stamped.
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry07()
{
  iDoTest("test_intersectBathymetry07/");
} // XmStampIntermediateTests::test_IntersectBathymetry07
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Intersection with guidebank centerline.
/// Cuts off the guidebank partially down the guidebank centerline.
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry08()
{
  iDoTest("test_intersectBathymetry08/");
} // XmStampIntermediateTests::test_IntersectBathymetry08
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Intersection with guidebank cross section
/// beyond the shoulder
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry09()
{
  iDoTest("test_intersectBathymetry09/");
} // XmStampIntermediateTests::test_IntersectBathymetry09
//------------------------------------------------------------------------------
/// \brief Tests stamp operation. Intersection with guidebank centerline.
/// Cuts off the guidebank right before the end.
//------------------------------------------------------------------------------
void XmStampIntermediateTests::test_IntersectBathymetry10()
{
  iDoTest("test_intersectBathymetry10/");
} // XmStampIntermediateTests::test_IntersectBathymetry10

#endif
