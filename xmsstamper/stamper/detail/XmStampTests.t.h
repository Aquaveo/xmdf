#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief
/// \ingroup stamping
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

#ifdef CXX_TEST

// 3. Standard Library Headers

// 4. External Library Headers
#include <cxxtest/TestSuite.h>

// 5. Shared Headers

// 6. Non-shared Headers

////////////////////////////////////////////////////////////////////////////////
/// \brief Tests the XmStampEndCap class
class XmStampIntermediateTests : public CxxTest::TestSuite
{
public:
  void test_WingWall01();
  void test_WingWall02();
  void test_WingWall03();
  void test_wingWall04_cut();
  void test_SlopedAbutment01();
  void test_SlopedAbutment02();
  void test_SlopedAbutment03();
  void test_SlopedAbutment04_cut();
  void test_SelfIntersect();
  void test_GuideBank01();
  void test_GuideBank02();
  void test_GuideBank03();
  void test_GuideBank04();
  void test_GuideBank05();
  void test_GuideBank06();
  void test_GuideBank07();
  void test_GuideBank08_cut();
  void test_IntersectBathymetry01();
  void test_IntersectBathymetry02();
  void test_IntersectBathymetry03();
  void test_IntersectBathymetry04();
  void test_IntersectBathymetry05();
  void test_IntersectBathymetry06();
  void test_IntersectBathymetry07();
  void test_IntersectBathymetry08();
  void test_IntersectBathymetry09();
  void test_IntersectBathymetry10();
}; // XmStampIntermediateTests

#endif
