#include <cmath>

#include <pybind11/pybind11.h>

#include "algorithm/hrave.h"
#include "algorithm/pyalg.h"
#include "body/body.h"
#include "move/move.h"
#include "node/python_node.h"
#include "settings/settings.h"
#include "stats/stats.h"
#include "util/empty.h"

using namespace mctslib;
namespace py = pybind11;

PYBIND11_MODULE(_mctslib_hrave, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring

    //@NOTE:    Must specify template args to move that correspond to the settings of the algorithm.
    //          No way to infer this because the Settings default constructor cannot be deleted,
    //          and because of this the compiler will always choose to decide that the parameter
    //          pack is empty.

    using PyTreeIterHRAVE = PyAlg<HRAVE<PythonNode<MCTSStats<uint>>, decltype(mcts_iter_move_no_rng), IterMCTSSettings, false>>;
    py::class_<PyTreeIterHRAVE>(m, "tree_iters_HRAVE")
        .def(py::init<py::object, uint, uint>())
        .def("move", &PyTreeIterHRAVE::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    using PyTreeCPUHRAVE = PyAlg<HRAVE<PythonNode<MCTSStats<uint>>, decltype(mcts_cpu_move_no_rng), CPUMCTSSettings, false>>;
    py::class_<PyTreeCPUHRAVE>(m, "tree_cpu_time_HRAVE")
        .def(py::init<py::object, uint, uint>())
        .def("move", &PyTreeCPUHRAVE::move<uint, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2));

    using PyTreeIterRNGHRAVE = PyAlg<HRAVE<PythonNode<MCTSStats<uint>>, decltype(mcts_iter_move_rng), IterMCTSSettings, false>>;
    py::class_<PyTreeIterRNGHRAVE>(m, "tree_iters_randomized_ties_HRAVE")
        .def(py::init<py::object, uint, uint>())
        .def("move", &PyTreeIterRNGHRAVE::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    using PyTreeCPURNGHRAVE = PyAlg<HRAVE<PythonNode<MCTSStats<uint>>, decltype(mcts_cpu_move_rng), CPUMCTSSettings, false>>;
    py::class_<PyTreeCPURNGHRAVE>(m, "tree_cpu_time_randomized_ties_HRAVE")
        .def(py::init<py::object, uint, uint>())
        .def("move", &PyTreeCPURNGHRAVE::move<uint, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2));

    using PyDAGIterHRAVE = PyAlg<HRAVE<PythonNode<MCTSStats<uint>>, decltype(mcts_iter_move_no_rng), IterMCTSSettings, true>>;
    py::class_<PyDAGIterHRAVE>(m, "dag_iters_HRAVE")
        .def(py::init<py::object, uint, uint>())
        .def("move", &PyDAGIterHRAVE::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    using PyDAGCPUHRAVE = PyAlg<HRAVE<PythonNode<MCTSStats<uint>>, decltype(mcts_cpu_move_no_rng), CPUMCTSSettings, true>>;
    py::class_<PyDAGCPUHRAVE>(m, "dag_cpu_time_HRAVE")
        .def(py::init<py::object, uint, uint>())
        .def("move", &PyDAGCPUHRAVE::move<uint, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2));

    using PyDAGIterRNGHRAVE = PyAlg<HRAVE<PythonNode<MCTSStats<uint>>, decltype(mcts_iter_move_rng), IterMCTSSettings, true>>;
    py::class_<PyDAGIterRNGHRAVE>(m, "dag_iters_randomized_ties_HRAVE")
        .def(py::init<py::object, uint, uint>())
        .def("move", &PyDAGIterRNGHRAVE::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    using PyDAGCPURNGHRAVE = PyAlg<HRAVE<PythonNode<MCTSStats<uint>>, decltype(mcts_cpu_move_rng), CPUMCTSSettings, true>>;
    py::class_<PyDAGCPURNGHRAVE>(m, "dag_cpu_time_randomized_ties_HRAVE")
        .def(py::init<py::object, uint, uint>())
        .def("move", &PyDAGCPURNGHRAVE::move<uint, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2));
}
