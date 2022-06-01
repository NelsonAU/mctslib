#include <pybind11/pybind11.h>

#include "algorithms/hrave.h"
#include "bindings/python_alg.h"
#include "bindings/python_node.h"

using namespace mctslib;
namespace py = pybind11;

PYBIND11_MODULE(_mctslib_hrave, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring

    //@NOTE:    Must specify template args to move that correspond to the settings of the algorithm.
    //          No way to infer this because the Settings default constructor cannot be deleted,
    //          and because of this the compiler will always choose to decide that the parameter
    //          pack is empty.
    using PyCPU_Tree_NoRNG_NoCAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, false, false, false, false>>;
    py::class_<PyCPU_Tree_NoRNG_NoCAS_HRAVE>(m, PyCPU_Tree_NoRNG_NoCAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyCPU_Tree_NoRNG_NoCAS_HRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_Tree_NoRNG_NoCAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, true, false, false, false>>;
    py::class_<PyIters_Tree_NoRNG_NoCAS_HRAVE>(m, PyIters_Tree_NoRNG_NoCAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyIters_Tree_NoRNG_NoCAS_HRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_DAG_NoRNG_NoCAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, false, true, false, false>>;
    py::class_<PyCPU_DAG_NoRNG_NoCAS_HRAVE>(m, PyCPU_DAG_NoRNG_NoCAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyCPU_DAG_NoRNG_NoCAS_HRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_DAG_NoRNG_NoCAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, true, true, false, false>>;
    py::class_<PyIters_DAG_NoRNG_NoCAS_HRAVE>(m, PyIters_DAG_NoRNG_NoCAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyIters_DAG_NoRNG_NoCAS_HRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_Tree_RNG_NoCAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, false, false, true, false>>;
    py::class_<PyCPU_Tree_RNG_NoCAS_HRAVE>(m, PyCPU_Tree_RNG_NoCAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyCPU_Tree_RNG_NoCAS_HRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_Tree_RNG_NoCAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, true, false, true, false>>;
    py::class_<PyIters_Tree_RNG_NoCAS_HRAVE>(m, PyIters_Tree_RNG_NoCAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyIters_Tree_RNG_NoCAS_HRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_DAG_RNG_NoCAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, false, true, true, false>>;
    py::class_<PyCPU_DAG_RNG_NoCAS_HRAVE>(m, PyCPU_DAG_RNG_NoCAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyCPU_DAG_RNG_NoCAS_HRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_DAG_RNG_NoCAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, true, true, true, false>>;
    py::class_<PyIters_DAG_RNG_NoCAS_HRAVE>(m, PyIters_DAG_RNG_NoCAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyIters_DAG_RNG_NoCAS_HRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_Tree_NoRNG_CAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, false, false, false, true>>;
    py::class_<PyCPU_Tree_NoRNG_CAS_HRAVE>(m, PyCPU_Tree_NoRNG_CAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyCPU_Tree_NoRNG_CAS_HRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_Tree_NoRNG_CAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, true, false, false, true>>;
    py::class_<PyIters_Tree_NoRNG_CAS_HRAVE>(m, PyIters_Tree_NoRNG_CAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyIters_Tree_NoRNG_CAS_HRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_DAG_NoRNG_CAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, false, true, false, true>>;
    py::class_<PyCPU_DAG_NoRNG_CAS_HRAVE>(m, PyCPU_DAG_NoRNG_CAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyCPU_DAG_NoRNG_CAS_HRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_DAG_NoRNG_CAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, true, true, false, true>>;
    py::class_<PyIters_DAG_NoRNG_CAS_HRAVE>(m, PyIters_DAG_NoRNG_CAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyIters_DAG_NoRNG_CAS_HRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_Tree_RNG_CAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, false, false, true, true>>;
    py::class_<PyCPU_Tree_RNG_CAS_HRAVE>(m, PyCPU_Tree_RNG_CAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyCPU_Tree_RNG_CAS_HRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_Tree_RNG_CAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, true, false, true, true>>;
    py::class_<PyIters_Tree_RNG_CAS_HRAVE>(m, PyIters_Tree_RNG_CAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyIters_Tree_RNG_CAS_HRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_DAG_RNG_CAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, false, true, true, true>>;
    py::class_<PyCPU_DAG_RNG_CAS_HRAVE>(m, PyCPU_DAG_RNG_CAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyCPU_DAG_RNG_CAS_HRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_DAG_RNG_CAS_HRAVE = PyAlg<HRAVE<PythonNode<MCTSStats>, true, true, true, true>>;
    py::class_<PyIters_DAG_RNG_CAS_HRAVE>(m, PyIters_DAG_RNG_CAS_HRAVE::str_id())
        .def(py::init<double, int, int, py::object>())
        .def("move", &PyIters_DAG_RNG_CAS_HRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));
}
