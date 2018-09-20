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

// 5. Shared code headers

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class XmStampCrossSection;
class XmStamperIo;
class XmStamperCenterlineProfile;
class Observer;

//----- Function prototypes ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class XmStampInterpCrossSection
/// \brief Interpolate cross sections for stamping
/// \see XmStampInterpCrossSectionImpl
class XmStampInterpCrossSection
{
public:
  static BSHP<XmStampInterpCrossSection> New();

  XmStampInterpCrossSection();
  virtual ~XmStampInterpCrossSection();
  /// \cond
  virtual void InterpMissingCrossSections(XmStamperIo& a_) = 0;
  virtual bool ValidCrossSectionsExist(XmStamperIo& a_) = 0;
  virtual void InterpCs(XmStampCrossSection& a_prev,
                        XmStampCrossSection& a_next,
                        double a_percent,
                        XmStampCrossSection& a_cs) = 0;
  virtual void InterpFromCenterlineProfile(XmStamperIo& a_io,
                                           XmStamperCenterlineProfile& a_profile) = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(XmStampInterpCrossSection);
  /// \endcond
}; // XmStampInterpCrossSection

} // namespace xms
