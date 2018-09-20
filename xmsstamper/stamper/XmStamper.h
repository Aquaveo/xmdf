#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup stamping
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers

// 4. External library headers
#include <xmscore/misc/boost_defines.h>
#include <xmscore/misc/base_macros.h> // for XM_DISALLOW_COPY_AND_ASSIGN
#include <xmscore/stl/vector.h>

// 5. Shared code headers

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class XmStamperIo;
class XmStamperCenterlineProfile;
class Observer;

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class XmStamper
/// \brief Performs a feature stamp operation.
/// \see XmStamperImpl
class XmStamper
{
public:
  static BSHP<XmStamper> New();

  XmStamper();
  virtual ~XmStamper();
  /// \cond
  virtual void DoStamp(XmStamperIo& a_) = 0;
  virtual void FillStamperIoFromCenterlineProfile(XmStamperIo& a_io,
                                                  XmStamperCenterlineProfile& a_profile) = 0;

  virtual const VecPt3d& GetPoints() = 0;
  virtual const VecInt2d& GetSegments() = 0;
  virtual const VecInt& GetBreaklineTypes() = 0;

  virtual void SetObserver(BSHP<Observer> a) = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(XmStamper);
  /// \endcond
}; // XmStamper

} // namespace xms
