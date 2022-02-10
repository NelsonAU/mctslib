#include <cmath>

#include <pybind11/pybind11.h>

#include "algorithm/mcts.h"
#include "algorithm/pyalg.h"
#include "body/body.h"
#include "move/move.h"
#include "node/python_node.h"
#include "settings/settings.h"
#include "stats/stats.h"

using namespace mctslib;
namespace py = pybind11;

PYBIND11_MODULE(_mctslib, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring

    //@NOTE:    Must specify template args to move that correspond to the settings of the algorithm.
    //          No way to infer this because the Settings default constructor cannot be deleted,
    //          and because of this the compiler will always choose to decide that the parameter
    //          pack is empty.

    using PyTreeIterMCTS = PyAlg<TreeMCTS<PythonNode, decltype(mcts_iter_move), IterMCTSSettings>>;
    py::class_<PyTreeIterMCTS>(m, "tree_iters_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyTreeIterMCTS::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));

    using PyTreeCPUMCTS = PyAlg<TreeMCTS<PythonNode, decltype(mcts_cpu_move), CPUMCTSSettings>>;
    py::class_<PyTreeCPUMCTS>(m, "tree_cpu_time_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyTreeCPUMCTS::move<uint, double, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("cpu_time"), py::arg("exploration_weight") = sqrt(2));
}
