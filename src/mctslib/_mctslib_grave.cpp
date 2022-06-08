#include <pybind11/pybind11.h>

#include "algorithms/grave.h"
#include "bindings/python_alg.h"
#include "bindings/python_node.h"

using namespace mctslib;
namespace py = pybind11;

PYBIND11_MODULE(_mctslib_grave, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring

    //@NOTE:    Must specify template args to move that correspond to the settings of the algorithm.
    //          No way to infer this because the Settings default constructor cannot be deleted,
    //          and because of this the compiler will always choose to decide that the parameter
    //          pack is empty.


    using PyCPU_Tree_NoRNG_NoCAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, false, false, false, false>>;
    py::class_<PyCPU_Tree_NoRNG_NoCAS_GRAVE>(m, PyCPU_Tree_NoRNG_NoCAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyCPU_Tree_NoRNG_NoCAS_GRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_Tree_NoRNG_NoCAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, true, false, false, false>>;
    py::class_<PyIters_Tree_NoRNG_NoCAS_GRAVE>(m, PyIters_Tree_NoRNG_NoCAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyIters_Tree_NoRNG_NoCAS_GRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_DAG_NoRNG_NoCAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, false, true, false, false>>;
    py::class_<PyCPU_DAG_NoRNG_NoCAS_GRAVE>(m, PyCPU_DAG_NoRNG_NoCAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyCPU_DAG_NoRNG_NoCAS_GRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_DAG_NoRNG_NoCAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, true, true, false, false>>;
    py::class_<PyIters_DAG_NoRNG_NoCAS_GRAVE>(m, PyIters_DAG_NoRNG_NoCAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyIters_DAG_NoRNG_NoCAS_GRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_Tree_RNG_NoCAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, false, false, true, false>>;
    py::class_<PyCPU_Tree_RNG_NoCAS_GRAVE>(m, PyCPU_Tree_RNG_NoCAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyCPU_Tree_RNG_NoCAS_GRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_Tree_RNG_NoCAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, true, false, true, false>>;
    py::class_<PyIters_Tree_RNG_NoCAS_GRAVE>(m, PyIters_Tree_RNG_NoCAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyIters_Tree_RNG_NoCAS_GRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_DAG_RNG_NoCAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, false, true, true, false>>;
    py::class_<PyCPU_DAG_RNG_NoCAS_GRAVE>(m, PyCPU_DAG_RNG_NoCAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyCPU_DAG_RNG_NoCAS_GRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_DAG_RNG_NoCAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, true, true, true, false>>;
    py::class_<PyIters_DAG_RNG_NoCAS_GRAVE>(m, PyIters_DAG_RNG_NoCAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyIters_DAG_RNG_NoCAS_GRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_Tree_NoRNG_CAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, false, false, false, true>>;
    py::class_<PyCPU_Tree_NoRNG_CAS_GRAVE>(m, PyCPU_Tree_NoRNG_CAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyCPU_Tree_NoRNG_CAS_GRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_Tree_NoRNG_CAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, true, false, false, true>>;
    py::class_<PyIters_Tree_NoRNG_CAS_GRAVE>(m, PyIters_Tree_NoRNG_CAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyIters_Tree_NoRNG_CAS_GRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_DAG_NoRNG_CAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, false, true, false, true>>;
    py::class_<PyCPU_DAG_NoRNG_CAS_GRAVE>(m, PyCPU_DAG_NoRNG_CAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyCPU_DAG_NoRNG_CAS_GRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_DAG_NoRNG_CAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, true, true, false, true>>;
    py::class_<PyIters_DAG_NoRNG_CAS_GRAVE>(m, PyIters_DAG_NoRNG_CAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyIters_DAG_NoRNG_CAS_GRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_Tree_RNG_CAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, false, false, true, true>>;
    py::class_<PyCPU_Tree_RNG_CAS_GRAVE>(m, PyCPU_Tree_RNG_CAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyCPU_Tree_RNG_CAS_GRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_Tree_RNG_CAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, true, false, true, true>>;
    py::class_<PyIters_Tree_RNG_CAS_GRAVE>(m, PyIters_Tree_RNG_CAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyIters_Tree_RNG_CAS_GRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));

    using PyCPU_DAG_RNG_CAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, false, true, true, true>>;
    py::class_<PyCPU_DAG_RNG_CAS_GRAVE>(m, PyCPU_DAG_RNG_CAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyCPU_DAG_RNG_CAS_GRAVE::move<int, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight"));

    using PyIters_DAG_RNG_CAS_GRAVE = PyAlg<GRAVE<PythonNode<RAVEStats>, true, true, true, true>>;
    py::class_<PyIters_DAG_RNG_CAS_GRAVE>(m, PyIters_DAG_RNG_CAS_GRAVE::str_id())
        .def(py::init<double, int, int, int, py::object>())
        .def("move", &PyIters_DAG_RNG_CAS_GRAVE::move<int, int, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight"));
}
