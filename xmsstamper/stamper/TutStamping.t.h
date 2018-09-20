#pragma once
#ifdef CXX_TEST
//------------------------------------------------------------------------------
/// \file
/// \ingroup tutorial
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers
#include <cxxtest/TestSuite.h>

// 5. Shared Headers

// 6. Non-shared Headers

//----- Namespace declaration --------------------------------------------------

class TutStampingUnitTests : public CxxTest::TestSuite
{
public:

  void test_StampFillEmbankment();
  void test_StampCutEmbankment();
  void test_StampWingWall();
  void test_StampSlopedAbutment();
  void test_StampGuidebank();
  void test_StampIntersectBathymetry();
};

#endif
