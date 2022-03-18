#include <cmath>

#include <pybind11/pybind11.h>

#include "algorithm/mcts.h"
#include "algorithm/pyalg.h"
#include "body/body.h"
#include "move/move.h"
#include "node/python_node.h"
#include "settings/settings.h"
#include "stats/stats.h"
#include "util/empty.h"

using namespace mctslib;
namespace py = pybind11;

PYBIND11_MODULE(_mctslib_mcts, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring

    //@NOTE:    Must specify template args to move that correspond to the settings of the algorithm.
    //          No way to infer this because the Settings default constructor cannot be deleted,
    //          and because of this the compiler will always choose to decide that the parameter
    //          pack is empty.

    using PyTreeIterMCTS = PyAlg<MCTS<PythonNode<MCTSStats>, decltype(mcts_iter_move_no_rng), IterMCTSSettings, false>>;
    py::class_<PyTreeIterMCTS>(m, "tree_iters_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyTreeIterMCTS::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    using PyTreeCPUMCTS = PyAlg<MCTS<PythonNode<MCTSStats>, decltype(mcts_cpu_move_no_rng), CPUMCTSSettings, false>>;
    py::class_<PyTreeCPUMCTS>(m, "tree_cpu_time_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyTreeCPUMCTS::move<uint, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2));

    using PyTreeIterRNGMCTS = PyAlg<MCTS<PythonNode<MCTSStats>, decltype(mcts_iter_move_rng), IterMCTSSettings, false>>;
    py::class_<PyTreeIterRNGMCTS>(m, "tree_iters_randomized_ties_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyTreeIterRNGMCTS::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    using PyTreeCPURNGMCTS = PyAlg<MCTS<PythonNode<MCTSStats>, decltype(mcts_cpu_move_rng), CPUMCTSSettings, false>>;
    py::class_<PyTreeCPURNGMCTS>(m, "tree_cpu_time_randomized_ties_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyTreeCPURNGMCTS::move<uint, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2));

    using PyDAGIterMCTS = PyAlg<MCTS<PythonNode<MCTSStats>, decltype(mcts_iter_move_no_rng), IterMCTSSettings, true>>;
    py::class_<PyDAGIterMCTS>(m, "dag_iters_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyDAGIterMCTS::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    using PyDAGCPUMCTS = PyAlg<MCTS<PythonNode<MCTSStats>, decltype(mcts_cpu_move_no_rng), CPUMCTSSettings, true>>;
    py::class_<PyDAGCPUMCTS>(m, "dag_cpu_time_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyDAGCPUMCTS::move<uint, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2));

    using PyDAGIterRNGMCTS = PyAlg<MCTS<PythonNode<MCTSStats>, decltype(mcts_iter_move_rng), IterMCTSSettings, true>>;
    py::class_<PyDAGIterRNGMCTS>(m, "dag_iters_randomized_ties_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyDAGIterRNGMCTS::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    using PyDAGCPURNGMCTS = PyAlg<MCTS<PythonNode<MCTSStats>, decltype(mcts_cpu_move_rng), CPUMCTSSettings, true>>;
    py::class_<PyDAGCPURNGMCTS>(m, "dag_cpu_time_randomized_ties_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyDAGCPURNGMCTS::move<uint, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2));
}
