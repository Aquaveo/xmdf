//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <boost/shared_ptr.hpp>
#include <xmscore/python/misc/PyUtils.h>
#include <xmscore/misc/DynBitset.h>
#include <xmscore/stl/vector.h>
#include <xmsstamper/stamper/XmStamper.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);
void initXmStamper(py::module &m) {
    py::class_<xms::XmStamper, boost::shared_ptr<xms::XmStamper>> stamper(m, "XmStamper");
#if(0)
    stamper
      .def(py::init([](boost::shared_ptr<xms::XmUGrid> a_ugrid) {
            return boost::shared_ptr<xms::XmStamper>(xms::XmStamper::New(a_ugrid));
        }))
        //  virtual double GetVectorMultiplier() const = 0;
        .def("get_vector_multiplier", &xms::XmStamper::GetVectorMultiplier, "Gets the vector multiplier")
        // virtual void SetVectorMultiplier(const double a_vectorMultiplier) = 0;
        .def("set_vector_multiplier", &xms::XmStamper::SetVectorMultiplier, "Sets the vector multiplier")
        // virtual double GetMaxTracingTime() const = 0;
        .def("get_max_tracing_time", &xms::XmStamper::GetMaxTracingTime, "Gets the max tracing time")
        // virtual void SetMaxTracingTime(const double a_maxTracingTime) = 0;
        .def("set_max_tracing_time", &xms::XmStamper::SetMaxTracingTime, "Sets the max tracing time")
        // virtual double GetMaxTracingDistance() const = 0;
        .def("get_max_tracing_distance", &xms::XmStamper::GetMaxTracingDistance, "Gets the max tracing distance")
        // virtual void SetMaxTracingDistance(const double a_maxTracingDistance) = 0;
        .def("set_max_tracing_distance", &xms::XmStamper::SetMaxTracingDistance, "Sets the max tracing distance")
        // virtual double GetMinDeltaTime() const = 0;
        .def("get_min_delta_time", &xms::XmStamper::GetMinDeltaTime, "Gets the minimum delta time")
        // virtual void SetMinDeltaTime(const double a_minDeltaTime) = 0;
        .def("set_min_delta_time", &xms::XmStamper::SetMinDeltaTime, "Sets the minimum delta time")
        // virtual double GetMaxChangeDistance() const = 0;
        .def("get_max_change_distance", &xms::XmStamper::GetMaxChangeDistance, "Gets the maximum change in distance")
        // virtual void SetMaxChangeDistance(const double a_maxChangeDistance) = 0;
        .def("set_max_change_distance", &xms::XmStamper::SetMaxChangeDistance, "Sets the maximum change in distance")
        // virtual double GetMaxChangeVelocity() const = 0;
        .def("get_max_change_velocity", &xms::XmStamper::GetMaxChangeVelocity, "Gets the maximum changee in velocity")
        // virtual void SetMaxChangeVelocity(const double a_maxChangeVelocity) = 0;
        .def("set_max_change_velocity", &xms::XmStamper::SetMaxChangeVelocity, "Sets the maximum changee in velocity")
        // virtual double GetMaxChangeDirectionInRadians() const = 0;
        .def("get_max_change_direction_in_radians", &xms::XmStamper::GetMaxChangeDirectionInRadians, "Gets the max change in direction in radians")
        // virtual void SetMaxChangeDirectionInRadians(const double a_maxChangeDirection) = 0;
        .def("set_max_change_direction_in_radians", &xms::XmStamper::SetMaxChangeDirectionInRadians, "Sets the max change in direction in radians")
        // virtual void AddGridScalarsAtTime(const VecPt3d& a_scalars, DataLocationEnum a_scalarLoc, xms::DynBitset& a_activity, DataLocationEnum a_activityLoc, double a_time) = 0;
        .def("add_grid_scalars_at_time", [](xms::XmStamper &self, py::iterable a_scalars,
          xms::DataLocationEnum a_scalarLoc,
          py::iterable a_activity,
          xms::DataLocationEnum a_activityLoc,
          double a_time) {
            boost::shared_ptr<xms::VecPt3d> scalars = xms::VecPt3dFromPyIter(a_scalars);
            xms::DynBitset activity = xms::DynamicBitsetFromPyIter(a_activity);
            self.AddGridScalarsAtTime(*scalars, a_scalarLoc, activity, a_activityLoc, a_time);
            return;
          }, "adds a grid scalar with a time to the tracer", py::arg("scalars"), py::arg("scalar_loc"), py::arg("activity"), py::arg("activity_loc"), py::arg("time")
        )
        // virtual void TracePoint(const Pt3d& a_pt, const double& a_ptTime, VecPt3d& a_outTrace, VecDbl& a_outTimes) = 0;
          .def("trace_point", [](xms::XmStamper &self, py::iterable a_pt, double a_ptTime) -> py::iterable {
          xms::Pt3d point = xms::Pt3dFromPyIter(a_pt);
          xms::VecPt3d outTrace;
          xms::VecDbl outTimes;
          self.TracePoint(point, a_ptTime, outTrace, outTimes);
          py::iterable resultTrace = xms::PyIterFromVecPt3d(outTrace);
          py::iterable resultTimes = xms::PyIterFromVecDbl(outTimes);
          return py::make_tuple(resultTrace, resultTimes);
        }, "Runs the Grid Trace for a point", py::arg("a_pt"), py::arg("a_ptTime"))
        // virtual std::string GetExitMessage() = 0;
        .def("get_exit_message", &xms::XmStamper::GetExitMessage,"returns a message describing what caused trace to exit")
        ;

    // DataLocationEnum
    py::enum_<xms::DataLocationEnum>(m, "data_location_enum",
                                    "data_location_enum location mapping for dataset values")
        .value("LOC_POINTS", xms::LOC_POINTS)
        .value("LOC_CELLS", xms::LOC_CELLS)
        .value("LOC_UNKNOWN", xms::LOC_UNKNOWN);
#endif
}
