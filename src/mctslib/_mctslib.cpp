#include <pybind11/pybind11.h>

#include "mctslib/algorithm/mcts.h"
#include "mctslib/algorithm/pyalg.h"
#include "mctslib/body/body.h"
#include "mctslib/move/move.h"
#include "mctslib/node/python_node.h"
#include "mctslib/stats/stats.h"

using namespace mctslib;
namespace py = pybind11;

PYBIND11_MODULE(_mctslib, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    using TreeIterMCTS = TreeMCTS<PythonNode, decltype(mcts_iter_move)>;
    using TreeCPUMCTS = TreeMCTS<PythonNode, decltype(mcts_cpu_move)>;
    
    using PyTreeIterMCTS = PyAlg<TreeIterMCTS>;
    using PyTreeCPUMCTS = PyAlg<TreeCPUMCTS>;
    //@NOTE:    Must specify template args to move that correspond to the settings of the algorithm.
    //          No way to infer this because the Settings default constructor cannot be deleted,
    //          and because of this the compiler will always choose to decide that the parameter
    //          pack is empty.

    py::class_<PyTreeIterMCTS>(m, "tree_iters_MCTS")
        .def(py::init<py::object>())
        .def("move", &TreePyIterMCTS::move<uint, uint, double, double>);

    py::class_<PyTreeCPUMCTS>(m, "tree_cpu_time_MCTS")
        .def(py::init<py::object>())
        .def("move", &TreePyCPUMCTS::move<uint, uint, double, double>);

}
