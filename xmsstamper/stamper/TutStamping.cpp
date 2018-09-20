//------------------------------------------------------------------------------
/// \file
/// \ingroup tutorial
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
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

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------
} // namespace xms

#if CXX_TEST
////////////////////////////////////////////////////////////////////////////////
// UNIT TESTS
////////////////////////////////////////////////////////////////////////////////
#include <xmsstamper/stamper/TutStamping.t.h>

#include <xmscore/testing/TestTools.h> // TS_ASSERT_EQUALS

#include <xmsinterp/geometry/geoms.h>
#include <xmsstamper/stamper/XmStamper.h>
#include <xmsstamper/stamper/XmStamperIo.h>
#include <xmsinterp/triangulate/TrTin.h>

////////////////////////////////////////////////////////////////////////////////
/// \class TutStampingUnitTests
/// \brief Tests stamping code
////////////////////////////////////////////////////////////////////////////////
//! [snip_testStampFillEmbankment]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampFillEmbankment()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {0, 10, 15}};

  // define a cross sections at each point on the center line. This will
  // be a symetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {{0, 0, 15},   {0, 10, 15}, {-5, 0, 15}, {-20, 0, 0}, {-5, 10, 15},
                          {-20, 10, 0}, {5, 0, 15},  {20, 0, 0},  {5, 10, 15}, {20, 10, 0}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-9);

  TS_ASSERT_EQUALS(7, io.m_outBreakLines.size());
  xms::VecInt baseLines = {0, 1}; // centerline
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[0]);
  baseLines = {3, 2, 0, 6, 7}; // first cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[1]);
  baseLines = {5, 4, 1, 8, 9}; // second cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[2]);
  baseLines = {3, 5}; // left toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[3]);
  baseLines = {7, 9}; // right toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[4]);
  baseLines = {2, 4}; // left shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[5]);
  baseLines = {6, 8}; // right shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[6]);
} // TutStampingUnitTests::test_StampFillEmbankment
//! [snip_testStampFillEmbankment]
//! [snip_testStampCutEmbankment]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple cut embankment
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampCutEmbankment()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "cut" stamp
  io.m_stampingType = 0;

  // define the center line
  io.m_centerLine = {{0, 0, 0}, {0, 10, 0}};

  // define a cross sections at each point on the center line. This will
  // be a symetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 0}, {5, 0}, {6, 1}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {{0, 0, 0},     {0, 10, 0}, {-5, 0, 0},  {-20, 0, 15}, {-5, 10, 0},
                          {-20, 10, 15}, {5, 0, 0},  {20, 0, 15}, {5, 10, 0},   {20, 10, 15}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-9);

  TS_ASSERT_EQUALS(7, io.m_outBreakLines.size());
  xms::VecInt baseLines = {0, 1}; // centerline
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[0]);
  baseLines = {3, 2, 0, 6, 7}; // first cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[1]);
  baseLines = {5, 4, 1, 8, 9}; // second cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[2]);
  baseLines = {3, 5}; // left toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[3]);
  baseLines = {7, 9}; // right toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[4]);
  baseLines = {2, 4}; // left shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[5]);
  baseLines = {6, 8}; // right shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[6]);
} // TutStampingUnitTests::test_StampCutEmbankment
//! [snip_testStampCutEmbankment]
//! [snip_testStampWingWall]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment with a wing wall end cap
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampWingWall()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {20, 20, 15}};

  // define a cross sections at each point on the center line. This will
  // be a symetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // wing wall is the default end cap

  // set the angles on the wing wall
  io.m_firstEndCap.m_angle = 15;
  io.m_firstEndCap.m_wingWall.m_wingWallAngle = 15;

  // set the angles on the wing wall
  io.m_lastEndCap.m_angle = -15;
  io.m_lastEndCap.m_wingWall.m_wingWallAngle = 10;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {{0, 0, 15},       {20, 20, 15}, {-4.5, 2.6, 15}, {-15.1, 13.2, 0},
                          {17.4, 24.5, 15}, {7.7, 36, 0}, {4.5, -2.6, 15}, {21.2, -7.1, 0},
                          {22.6, 15.5, 15}, {28.2, 0, 0}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-1);

  TS_ASSERT_EQUALS(7, io.m_outBreakLines.size());
  xms::VecInt baseLines = {0, 1}; // centerline
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[0]);
  baseLines = {3, 2, 0, 6, 7}; // first cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[1]);
  baseLines = {5, 4, 1, 8, 9}; // second cross section
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[2]);
  baseLines = {3, 5}; // left toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[3]);
  baseLines = {7, 9}; // right toe
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[4]);
  baseLines = {2, 4}; // left shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[5]);
  baseLines = {6, 8}; // right shoulder
  TS_ASSERT_EQUALS_VEC(baseLines, io.m_outBreakLines[6]);
} // TutStampingUnitTests::test_StampWingWall
//! [snip_testStampWingWall]
//! [snip_testStampSlopedAbutment]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment with sloped abutment
/// end cap
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampSlopedAbutment()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {20, 20, 15}};

  // define a cross sections at each point on the center line. This will
  // be a symetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // change to a sloped abutment end cap
  io.m_firstEndCap.m_type = 1;

  // set the slope and max length for the end cap
  io.m_firstEndCap.m_slopedAbutment.m_slope = {{0, 15}, {1, 14}};
  io.m_firstEndCap.m_slopedAbutment.m_maxX = 10;

  // make the last end cap the same as the first
  io.m_lastEndCap = io.m_firstEndCap;
  // change the angle on the last end cap
  io.m_lastEndCap.m_angle = 20;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {
    {0, 0, 15},         {20, 20, 15},       {-3.5, 3.5, 15},   {-14.1, 14.1, 0},
    {15.2, 22.2, 15},   {4.6, 32.9, 0},     {3.5, -3.5, 15},   {14.1, -14.1, 0},
    {24.8, 17.8, 15},   {35.4, 7.1, 0},     {-10.6, -3.5, 5},  {-12.9, -1.9, 4.2},
    {-14.8, 0.5, 3.3},  {-16, 3.5, 2.5},    {-16.4, 7, 1.7},   {-15.8, 10.6, 0.8},
    {-3.5, -10.6, 5.0}, {-1.9, -12.9, 4.2}, {0.5, -14.8, 3.3}, {3.5, -16.0, 2.5},
    {7.0, -16.4, 1.7},  {10.6, -15.8, 0.8}, {19.4, 31.3, 5},   {16.6, 33.4, 3.8},
    {13, 34.6, 2.5},    {8.8, 34.4, 1.3},   {29, 26.8, 5},     {31.8, 25.9, 4.3},
    {34.3, 24.1, 3.6},  {36.4, 21.5, 2.9},  {37.7, 18.2, 2.1}, {38, 14.6, 1.4},
    {37.3, 10.8, 0.7}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-1);

} // TutStampingUnitTests::test_StampSlopedAbutment
//! [snip_testStampSlopedAbutment]
//! [snip_testStampGuidebank]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment with guidebank
/// end cap
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampGuidebank()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {50, 50, 15}};

  // define a cross sections at each point on the center line. This will
  // be a symetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 10;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // change to a guidebank end cap
  io.m_firstEndCap.m_type = 0;

  io.m_firstEndCap.m_guidebank.m_nPts = 10;
  io.m_firstEndCap.m_guidebank.m_radius1 = 30;
  io.m_firstEndCap.m_guidebank.m_radius2 = 15;
  io.m_firstEndCap.m_guidebank.m_side = 0; // left side
  io.m_firstEndCap.m_guidebank.m_width = 6;

  // make the last end cap the same as the first
  io.m_lastEndCap = io.m_firstEndCap;
  // change the angle on the last end cap
  io.m_lastEndCap.m_angle = 10;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  TS_ASSERT_EQUALS(144, io.m_outTin->Points().size());
} // TutStampingUnitTests::test_StampGuidebank
//! [snip_testStampGuidebank]
//! [snip_testStampIntersectBathymetry]
//------------------------------------------------------------------------------
/// \brief    Tests stamping a simple fill embankment bathymetry cut off
//------------------------------------------------------------------------------
void TutStampingUnitTests::test_StampIntersectBathymetry()
{
  // declare a XmStamperIo class to create the inputs
  xms::XmStamperIo io;

  // set as a "fill" stamp
  io.m_stampingType = 1;

  // define the center line
  io.m_centerLine = {{0, 0, 15}, {10, 10, 15}};

  // define a cross sections at each point on the center line. This will
  // be a symetric cross section. The top width will be 10 and the side
  // slope will be 1. The cross section will have a total width of 40.
  xms::XmStampCrossSection cs;
  cs.m_left = {{0, 15}, {5, 15}, {6, 14}};
  cs.m_leftMax = 20;
  cs.m_idxLeftShoulder = 1;
  cs.m_right = cs.m_left;
  cs.m_rightMax = cs.m_leftMax;
  cs.m_idxRightShoulder = cs.m_idxLeftShoulder;
  io.m_cs.push_back(cs);
  io.m_cs.push_back(cs);

  // create a TIN to represent Bathymetry
  BSHP<xms::TrTin> tin = xms::TrTin::New();
  BSHP<xms::VecPt3d> tPts(new xms::VecPt3d());
  *tPts = {{-1, 25, 6}, {15, 11, 6}, {5, -11, 10}, {20, 4, 10}};
  BSHP<xms::VecInt> tTris(new xms::VecInt());
  *tTris = {0, 1, 2, 1, 3, 2};
  tin->SetPoints(tPts);
  tin->SetTriangles(tTris);
  // set the bathymetry member of the io class
  io.m_bathemetry = tin;

  // create a XmStamper class. This class performs the stamp operation.
  BSHP<xms::XmStamper> st = xms::XmStamper::New();
  st->DoStamp(io);

  // verify the outputs
  TS_ASSERT(io.m_outTin); // the output TIN should exist
  xms::VecPt3d basePts = {{0, 0, 15},         {10, 10, 15},        {-3.54, 3.54, 15},
                          {-14.14, 14.14, 0}, {6.46, 13.54, 15},   {0.49, 19.51, 6.54},
                          {3.54, -3.54, 15},  {7.37, -7.37, 9.58}, {13.54, 6.46, 15},
                          {17.37, 2.63, 9.58}};
  TS_ASSERT_DELTA_VECPT3D(basePts, io.m_outTin->Points(), 1e-2);
} // TutStampingTests::test_StampIntersectBathymetry
  //! [snip_testStampIntersectBathymetry]

#endif
