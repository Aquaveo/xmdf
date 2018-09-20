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
#include <xmsstamper/stamper/XmStamperIo.h>

// 3. Standard library headers
#include <sstream>

// 4. External library headers
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

// 5. Shared code headers
#include <xmsinterp/triangulate/TrTin.h>

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

//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmWingWall::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_wingWallAngle;
} // XmWingWall::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmSlopedAbutment::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_maxX;
  archive& m_slope;
} // XmSlopedAbutment::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmGuidebank::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_side;
  archive& m_radius1;
  archive& m_radius2;
  archive& m_width;
  archive& m_nPts;
} // XmGuidebank::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmStamperEndCap::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_type;
  archive& m_angle;
  archive& m_guidebank;
  archive& m_slopedAbutment;
  archive& m_wingWall;
} // XmStamperEndCap::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmStampCrossSection::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_left;
  archive& m_leftMax;
  archive& m_idxLeftShoulder;
  archive& m_right;
  archive& m_rightMax;
  archive& m_idxRightShoulder;
} // XmStampCrossSection::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmStamperIo::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_centerLine;
  archive& m_stampingType;
  archive& m_cs;
  archive& m_firstEndCap;
  archive& m_lastEndCap;
  archive& m_bathemetry;
  archive& m_outTin;
  archive& m_outBreakLines;
} // XmStamperIo::serialize
//------------------------------------------------------------------------------
/// \brief Use boost archive to get the sampling grid as text.
///
/// This function is in this file so it has access to XmStamperIo.
/// \return A string representation of the class.
//------------------------------------------------------------------------------
std::string XmStamperIo::ToString() const
{
  std::stringstream ss;
  {
    boost::archive::text_oarchive oa(ss);
    oa << *this;
  }
  return ss.str();
} // XmStamperIo::ToString
//------------------------------------------------------------------------------
/// \brief Use boost archive to turn the text into a sampling grid.
///
/// This function is in this file so it has access to XmStamperIo.
//------------------------------------------------------------------------------
void XmStamperIo::FromString(const std::string& a_text)
{
  std::stringstream ss(a_text);
  {
    boost::archive::text_iarchive ia(ss);
    ia >> *this;
  }
} // XmStamperIo::FromString

} // namespace xms
