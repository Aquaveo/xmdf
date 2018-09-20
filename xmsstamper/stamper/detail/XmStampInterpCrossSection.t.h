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
/// \brief Tests the XmStampInterpCrossSection class
class XmStampInterpCrossSectionUnitTests : public CxxTest::TestSuite
{
public:
  void test0();
  void test1();
  void testInterpFromCenterlineProfile0();
  void testCrossSectionTutorial();
}; // XmStampInterpCrossSectionUnitTests

#endif
