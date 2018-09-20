//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <xmscore/python/misc/PyUtils.h> // pybind11.h, numpy.h, DynBitset.h, vector.h, ptsfwd.h, shared_ptr.h
#include <xmsstamper/stamper/XmStamperIo.h>
#include <xmsinterp/triangulate/TrTin.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initXmStamperIo(py::module &m) { // https://pybind11.readthedocs.io/en/stable/classes.html

  ////////////////////////////////////////////////////////////////////////////////
  /// \class XmWingWall
  /// \brief Wing wall definition for feature stamp end cap
  py::class_ < xms::XmWingWall, boost::shared_ptr<xms::XmWingWall>> wing_wall(m, "XmWingWall");

  wing_wall.def(py::init());

  // ---------------------------------------------------------------------------
  // Property: wing_wall_angle
  /// Required. Center line for the feature stamp
  // ---------------------------------------------------------------------------
  const char* wing_wall_angle_doc = R"pydoc(
    Property for the wing wall angle.
  )pydoc";
  wing_wall.def_property("wing_wall_angle",
    [](xms::XmWingWall &self) -> double {
      return self.m_wingWallAngle;
    },
    [](xms::XmWingWall &self, double wing_wall_angle) {
      self.m_wingWallAngle = wing_wall_angle;
    },
    wing_wall_angle_doc
  );

  ////////////////////////////////////////////////////////////////////////////////
  /// \class XmSlopedAbutment
  /// \brief Abutment definition for feature stamp end cap
  py::class_ < xms::XmSlopedAbutment, boost::shared_ptr<xms::XmSlopedAbutment>> sloped_abutment(m, "XmSlopedAbutment");
  sloped_abutment.def(py::init());

  // ---------------------------------------------------------------------------
  // Property: max_X
  // ---------------------------------------------------------------------------
  const char* max_x_doc = R"pydoc(
    Property for the max distance from the center line.
  )pydoc";
  sloped_abutment.def_property("max_x",
    [](xms::XmSlopedAbutment &self) -> double {
      return self.m_maxX;
    },
    [](xms::XmSlopedAbutment &self, double max_x) {
      self.m_maxX = max_x;
    },
      max_x_doc
  );

  // ---------------------------------------------------------------------------
  // Property: slope
  // ---------------------------------------------------------------------------
  const char* slope_doc = R"pydoc(
    Property for the x,y pairs defining slope from center line.
  )pydoc";
  sloped_abutment.def_property("slope",
    [](xms::XmSlopedAbutment &self) -> py::iterable {
      xms::VecPt2d &temp = self.m_slope;
      py::array_t<double, py::array::c_style> tempy({ (int)temp.size(), 2 });
      auto r = tempy.mutable_unchecked<2>();
      int i = 0;
      for (ssize_t i = 0; i < r.shape(0); i++) {
        r(i, 0) = temp[i].x;
        r(i, 1) = temp[i].y;
      }
      return tempy;
    },
    [](xms::XmSlopedAbutment &self, py::iterable slope) {
      self.m_slope.clear();
      self.m_slope.reserve(py::len(slope));
      for (auto item : slope) {
        if (!py::isinstance<py::iterable>(item)) {
          throw py::type_error("First arg must be a n-tuple of 2-tuples");
        }
        py::tuple tuple = item.cast<py::tuple>();
        if (py::len(tuple) != 2) {
          throw py::type_error("Input points must be 2-tuples");
        }
        else {
          xms::Pt2d point(tuple[0].cast<double>(), tuple[1].cast<double>());
          self.m_slope.push_back(point);
        }
      }
    },
    slope_doc
  );

  ////////////////////////////////////////////////////////////////////////////////
  /// \class XmGuidebank
  /// \brief Guidebank definition for feature stamp end cap
  py::class_ < xms::XmGuidebank, boost::shared_ptr<xms::XmGuidebank>> guidebank(m, "XmGuidebank");
  guidebank.def(py::init());

  // ---------------------------------------------------------------------------
  // Property: side
  // ---------------------------------------------------------------------------
  const char* side_doc = R"pydoc(
    Property for the position of guidebank relative to center line, 0-left, 1-right
  )pydoc";
  guidebank.def_property("side",
    [](xms::XmGuidebank &self) -> int {
      return self.m_side;
    },
    [](xms::XmGuidebank &self, int side) {
      self.m_side = side;
    },
    side_doc
  );

  // ---------------------------------------------------------------------------
  // Property: radius1
  // ---------------------------------------------------------------------------
  const char* radius1_doc = R"pydoc(
    Property for the first radius (R1) for guidebank creation
  )pydoc";
  guidebank.def_property("radius1",
    [](xms::XmGuidebank &self) -> double {
      return self.m_radius1;
    },
    [](xms::XmGuidebank &self, double radius1) {
      self.m_radius1 = radius1;
    },
    radius1_doc
  );

  // ---------------------------------------------------------------------------
  // Property: radius2
  // ---------------------------------------------------------------------------
  const char* radius2_doc = R"pydoc(
    Property for the second radius (R2) for guidebank creation
  )pydoc";
  guidebank.def_property("radius2",
    [](xms::XmGuidebank &self) -> double {
      return self.m_radius2;
    },
    [](xms::XmGuidebank &self, double radius2) {
      self.m_radius2 = radius2;
    },
    radius2_doc
  );

  // ---------------------------------------------------------------------------
  // Property: width
  // ---------------------------------------------------------------------------
  const char* width_doc = R"pydoc(
    Property for the width (R2) for guidebank creation
  )pydoc";
  guidebank.def_property("width",
    [](xms::XmGuidebank &self) -> double {
      return self.m_width;
    },
    [](xms::XmGuidebank &self, double width) {
      self.m_width = width;
    },
    width_doc
  );

  // ---------------------------------------------------------------------------
  // Property: num_points
  // ---------------------------------------------------------------------------
  const char* n_pts_doc = R"pydoc(
    Property for the number of points created along the center line to create the guidebank
  )pydoc";
  guidebank.def_property("n_pts",
    [](xms::XmGuidebank &self) -> int {
      return self.m_nPts;
    },
    [](xms::XmGuidebank &self, int n_pts) {
      self.m_nPts = n_pts;
    },
      n_pts_doc
  );

  ////////////////////////////////////////////////////////////////////////////////
  /// \class XmStamperEndCap
  /// \brief End cap definition for feature stamp
  py::class_ < xms::XmStamperEndCap, boost::shared_ptr<xms::XmStamperEndCap>> stamper_end_cap(m, "XmStamperEndCap");
  stamper_end_cap.def(py::init());

  // ---------------------------------------------------------------------------
  // Property: type
  // ---------------------------------------------------------------------------
  const char* type_doc = R"pydoc(
    Property for the type of end cap: 0- guidebank, 1- sloped abutment, 2- wing wall
  )pydoc";
  stamper_end_cap.def_property("type",
    [](xms::XmStamperEndCap &self) -> int {
      return self.m_type;
    },
    [](xms::XmStamperEndCap &self, int type) {
      self.m_type = type;
    },
    type_doc
  );

  // ---------------------------------------------------------------------------
  // Property: angle
  // ---------------------------------------------------------------------------
  const char* angle_doc = R"pydoc(
    Property for the degrees from -45 to 45
  )pydoc";
  stamper_end_cap.def_property("angle",
    [](xms::XmStamperEndCap &self) -> double {
      return self.m_angle;
    },
    [](xms::XmStamperEndCap &self, double angle) {
      self.m_angle = angle;
    },
    angle_doc
  );

  // ---------------------------------------------------------------------------
  // Property: guidebank
  // ---------------------------------------------------------------------------
  const char* guidebank_doc = R"pydoc(
    Property for the guidebank definition
  )pydoc";
  stamper_end_cap.def_property("guidebank",
    [](xms::XmStamperEndCap &self) -> xms::XmGuidebank& {
      return self.m_guidebank;
    },
    [](xms::XmStamperEndCap &self, xms::XmGuidebank& a_guidebank) {
      self.m_guidebank = a_guidebank;
    },
    guidebank_doc
  );

  // ---------------------------------------------------------------------------
  // Property: sloped_abutment
  // ---------------------------------------------------------------------------
  const char* sloped_abutment_doc = R"pydoc(
    Property for the sloped abutment definition
  )pydoc";
  stamper_end_cap.def_property("sloped_abutment",
    [](xms::XmStamperEndCap &self) -> xms::XmSlopedAbutment& {
      return self.m_slopedAbutment;
    },
    [](xms::XmStamperEndCap &self, xms::XmSlopedAbutment&  a_sloped_abutment) {
      self.m_slopedAbutment = a_sloped_abutment;
    },
    sloped_abutment_doc
  );

  // ---------------------------------------------------------------------------
  // Property: wing_wall
  // ---------------------------------------------------------------------------
  const char* wing_wall_doc = R"pydoc(
    Property for the wing wall definition
  )pydoc";
  stamper_end_cap.def_property("wing_wall",
    [](xms::XmStamperEndCap &self) -> xms::XmWingWall& {
      return self.m_wingWall;
    },
    [](xms::XmStamperEndCap &self, xms::XmWingWall& a_wing_wall) {
      self.m_wingWall = a_wing_wall;
    },
    wing_wall_doc
  );

  ////////////////////////////////////////////////////////////////////////////////
  /// \class XmStampCrossSection
  /// \brief Cross section definition for stamping
  py::class_ < xms::XmStampCrossSection, boost::shared_ptr<xms::XmStampCrossSection>> stamp_cross_section(m, "XmStampCrossSection");
  stamp_cross_section.def(py::init());

  // ---------------------------------------------------------------------------
  // Property: left_max
  // ---------------------------------------------------------------------------
  const char* stamp_cross_section_doc = R"pydoc(
    Property for the max x value for left side
  )pydoc";
  stamp_cross_section.def_property("left_max",
    [](xms::XmStampCrossSection &self) -> double {
      return self.m_leftMax;
    },
    [](xms::XmStampCrossSection &self, double left_max) {
      self.m_leftMax = left_max;
    },
    stamp_cross_section_doc
  );

  // ---------------------------------------------------------------------------
  // Property: right_max
  // ---------------------------------------------------------------------------
  const char* right_max_doc = R"pydoc(
    Property for the max x value for right side
  )pydoc";
  stamp_cross_section.def_property("right_max",
    [](xms::XmStampCrossSection &self) -> double {
      return self.m_rightMax;
    },
    [](xms::XmStampCrossSection &self, double right_max) {
      self.m_rightMax = right_max;
    },
    right_max_doc
  );

  // ---------------------------------------------------------------------------
  // Property: index_left_shoulder
  // ---------------------------------------------------------------------------
  const char* idx_left_shoulder_doc = R"pydoc(
    Property for the index to the should point in the m_left vector
  )pydoc";
  stamp_cross_section.def_property("idx_left_shoulder",
    [](xms::XmStampCrossSection &self) -> int {
      return self.m_idxLeftShoulder;
    },
    [](xms::XmStampCrossSection &self, int idx_left_shoulder) {
      self.m_idxLeftShoulder = idx_left_shoulder;
    },
    idx_left_shoulder_doc
  );

  // ---------------------------------------------------------------------------
  // Property: index_right_shoulder
  // ---------------------------------------------------------------------------
  const char* idx_right_shoulder_doc = R"pydoc(
    Property for the index to the should point in the m_right vector
  )pydoc";
  stamp_cross_section.def_property("idx_right_shoulder",
    [](xms::XmStampCrossSection &self) -> int {
      return self.m_idxRightShoulder;
    },
    [](xms::XmStampCrossSection &self, int idx_right_shoulder) {
      self.m_idxRightShoulder = idx_right_shoulder;
    },
    idx_right_shoulder_doc
  );

  // ---------------------------------------------------------------------------
  // Property: left
  // ---------------------------------------------------------------------------
  const char* left_doc = R"pydoc(
    Property for the points defining the cross section
  )pydoc";
  stamp_cross_section.def_property("left",
    [](xms::XmStampCrossSection &self) -> py::iterable {
      xms::VecPt2d &temp = self.m_left;
      py::array_t<double, py::array::c_style> tempy({ (int)temp.size(), 2 });
      auto r = tempy.mutable_unchecked<2>();
      int i = 0;
      for (ssize_t i = 0; i < r.shape(0); i++) {
        r(i, 0) = temp[i].x;
        r(i, 1) = temp[i].y;
      }
      return tempy;
    },
    [](xms::XmStampCrossSection &self, py::iterable left) {
      self.m_left.clear();
      self.m_left.reserve(py::len(left));
      for (auto item : left) {
        if (!py::isinstance<py::iterable>(item)) {
          throw py::type_error("First arg must be a n-tuple of 2-tuples");
        }
        py::tuple tuple = item.cast<py::tuple>();
        if (py::len(tuple) != 2) {
          throw py::type_error("Input points must be 2-tuples");
        }
        else {
          xms::Pt2d point(tuple[0].cast<double>(), tuple[1].cast<double>());
          self.m_left.push_back(point);
        }
      }
    },
    left_doc
  );

  // ---------------------------------------------------------------------------
  // Property: right
  // ---------------------------------------------------------------------------
  const char* right_doc = R"pydoc(
    Property for the points defining the cross section
  )pydoc";
  stamp_cross_section.def_property("right",
    [](xms::XmStampCrossSection &self) -> py::iterable {
      xms::VecPt2d &temp = self.m_right;
      py::array_t<double, py::array::c_style> tempy({ (int)temp.size(), 2 });
      auto r = tempy.mutable_unchecked<2>();
      int i = 0;
      for (ssize_t i = 0; i < r.shape(0); i++) {
        r(i, 0) = temp[i].x;
        r(i, 1) = temp[i].y;
      }
      return tempy;
    },
    [](xms::XmStampCrossSection &self, py::iterable right) {
      self.m_right.clear();
      self.m_right.reserve(py::len(right));
      for (auto item : right) {
        if (!py::isinstance<py::iterable>(item)) {
          throw py::type_error("First arg must be a n-tuple of 2-tuples");
        }
        py::tuple tuple = item.cast<py::tuple>();
        if (py::len(tuple) != 2) {
          throw py::type_error("Input points must be 2-tuples");
        }
        else {
          xms::Pt2d point(tuple[0].cast<double>(), tuple[1].cast<double>());
          self.m_right.push_back(point);
        }
      }
    },
    right_doc
  );

  ////////////////////////////////////////////////////////////////////////////////
  /// \class XmStamperCenterlineProfile
  /// \brief Cross section for stamping using distance rather than polyline index
  py::class_ < xms::XmStamperCenterlineProfile, boost::shared_ptr<xms::XmStamperCenterlineProfile>> stamper_centerline_profile(m, "XmStamperCenterlineProfile");
  stamper_centerline_profile.def(py::init());

  // ---------------------------------------------------------------------------
  // Property: distance
  // ---------------------------------------------------------------------------
  const char* distance_doc = R"pydoc(
    Property for the points defining the cross section
  )pydoc";
  stamp_cross_section.def_property("distance",
    [](xms::XmStamperCenterlineProfile &self) -> py::iterable {
      return xms::PyIterFromVecDbl(self.m_distance);
    },
    [](xms::XmStamperCenterlineProfile &self, py::iterable distance) {
      self.m_distance = *xms::VecDblFromPyIter(distance);
    },
    distance_doc
  );

  // ---------------------------------------------------------------------------
  // Property: elevation
  // ---------------------------------------------------------------------------
  const char* elevation_doc = R"pydoc(
    Property for the elevations at the cross section locations
  )pydoc";
  stamp_cross_section.def_property("elevation",
    [](xms::XmStamperCenterlineProfile &self) -> py::iterable {
      return xms::PyIterFromVecDbl(self.m_elevation);
    },
    [](xms::XmStamperCenterlineProfile &self, py::iterable elevation) {
      self.m_elevation = *xms::VecDblFromPyIter(elevation);
    },
    elevation_doc
  );

  ////////////////////////////////////////////////////////////////////////////////
  /// \class XmStamperIo
  /// \brief Cross section for stamping using distance rather than polyline index
  py::class_ < xms::XmStamperIo, boost::shared_ptr<xms::XmStamperIo>> stamper_io(m, "XmStamperIo");
  stamper_io.def(py::init());

  /// Input

  // ---------------------------------------------------------------------------
  // function: progress_status
  /// Required. Center line for the feature stamp
  // ---------------------------------------------------------------------------
  const char* center_line_doc = R"pydoc(
    Property for the center line.
  )pydoc";
  stamper_io.def_property("center_line",
    [](xms::XmStamperIo &self) -> py::iterable {
      return xms::PyIterFromVecPt3d(self.m_centerLine);
    },
    [](xms::XmStamperIo &self, py::iterable a_center_line) {
      self.m_centerLine = *xms::VecPt3dFromPyIter(a_center_line);
    },
    center_line_doc
  );

  // ---------------------------------------------------------------------------
  // Property: stamping_type
  // ---------------------------------------------------------------------------
  const char* stamping_type_doc = R"pydoc(
    Property for the Stamping type 0 - Cut, 1 - Fill
  )pydoc";
  stamper_io.def_property("stamping_type",
    [](xms::XmStamperIo &self) -> int {
      return self.m_stampingType;
    },
    [](xms::XmStamperIo &self, int stamping_type) {
      self.m_stampingType = stamping_type;
    },
    stamping_type_doc
  );

  // ---------------------------------------------------------------------------
  // Property: cross_section_count
  // ---------------------------------------------------------------------------
  const char* cross_section_size_doc = R"pydoc(
    Property for the number of cross sections
  )pydoc";
  stamper_io.def_property("cross_section_size",
    [](xms::XmStamperIo &self) -> int {
      return (int)self.m_cs.size();
    },
    [](xms::XmStamperIo &self, int cross_section_size) {
      self.m_cs.resize(cross_section_size);
    },
    cross_section_size_doc
  );

  // ---------------------------------------------------------------------------
  // set_cross_section
  // ---------------------------------------------------------------------------
  const char* set_cross_section_doc = R"pydoc(
    Method Property for the number of cross sections

    Args:
      cross_section_index (int): Index of the cross section from left to right
      cross_section (cross_section): Cross section values
  )pydoc";
  stamper_io.def("set_cross_section", [](xms::XmStamperIo &self,
    int cross_section_index, 
    xms::XmStampCrossSection& cross_section) {
      if (cross_section_index >= self.m_cs.size()) {
        throw py::type_error("The cross section index must be less than the cross section count.");
      }
      self.m_cs[cross_section_index] = cross_section;
    }, 
    set_cross_section_doc, py::arg("cross_section_index"), py::arg("cross_section")
  );
    
  // ---------------------------------------------------------------------------
  // Property: first_end_cap
  // ---------------------------------------------------------------------------
  const char* first_end_cap_doc = R"pydoc(
    Property for the end cap at beginning of polyline
  )pydoc";
  stamper_io.def_property("first_end_cap",
    [](xms::XmStamperIo &self) -> xms::XmStamperEndCap& {
      return self.m_firstEndCap;
    },
    [](xms::XmStamperIo &self, xms::XmStamperEndCap& a_first_end_cap) {
      self.m_firstEndCap = a_first_end_cap;
    },
    first_end_cap_doc
  );    
    
  // ---------------------------------------------------------------------------
  // Property: last_end_cap
  // ---------------------------------------------------------------------------
  const char* last_end_cap_doc = R"pydoc(
    Property for the end cap at beginning of polyline
  )pydoc";
  stamper_io.def_property("last_end_cap",
    [](xms::XmStamperIo &self) -> xms::XmStamperEndCap& {
      return self.m_lastEndCap;
    },
    [](xms::XmStamperIo &self, xms::XmStamperEndCap& a_last_end_cap) {
      self.m_lastEndCap = a_last_end_cap;
    },
    last_end_cap_doc
  );        
    
  // ---------------------------------------------------------------------------
  // Property: bathemetry
  // ---------------------------------------------------------------------------
  const char* bathemetry_doc = R"pydoc(
    Property for the end cap at beginning of polyline
  )pydoc";
  stamper_io.def_property("bathemetry",
    [](xms::XmStamperIo &self) -> boost::shared_ptr<xms::TrTin>& {
      return self.m_bathemetry;
    },
    [](xms::XmStamperIo &self, boost::shared_ptr<xms::TrTin>& bathemetry) {
      self.m_bathemetry = bathemetry;
    },
    bathemetry_doc
  );

  // ---------------------------------------------------------------------------
  // Property: output_tin
  // ---------------------------------------------------------------------------
  const char* out_tin_doc = R"pydoc(
    Property for the TIN created by the stamp operation
  )pydoc";
  stamper_io.def_property("output_tin",
    [](xms::XmStamperIo &self) -> boost::shared_ptr<xms::TrTin>& {
      return self.m_outTin;
    },
    [](xms::XmStamperIo &self, boost::shared_ptr<xms::TrTin>& /*out_tin*/) {
      // self.m_output_tin = output_tin;
    },
    out_tin_doc
  );

  // ---------------------------------------------------------------------------
  // Property: output_break_lines
  // ---------------------------------------------------------------------------
  const char* out_break_lines_doc = R"pydoc(
    Property for the break lines that are honored in the TIN
  )pydoc";
  stamper_io.def_property("out_break_lines",
    [](xms::XmStamperIo &self) -> py::iterable {
      return xms::PyIterFromVecInt2d(self.m_outBreakLines);
    },
    [](xms::XmStamperIo &self, py::iterable /*output_break_lines*/) {
    },
    out_break_lines_doc
  );
}
