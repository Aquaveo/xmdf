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
/// \brief Tests the StStampEndCap class
class XmBathymetryIntersectorUnitTests : public CxxTest::TestSuite
{
public:
  void testCreateClass();
  void testIntersectCenterLine();
  void testIntersectXsects();
  void testClassifyPoints();
  void testDescomposeCenterLine();
}; // XmBathymetryIntersectorUnitTests

#endif
