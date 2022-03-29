#include <cmath>

#include "python_node.h"
#include <pybind11/pybind11.h>
#include "mcts.h"

using namespace mctslib;
namespace py = pybind11;

PYBIND11_MODULE(_mctslib_mcts, m)
{
    m.doc() = "pybind11 example plugin"; // optional module docstring

    //@NOTE:    Must specify template args to move that correspond to the settings of the algorithm.
    //          No way to infer this because the Settings default constructor cannot be deleted,
    //          and because of this the compiler will always choose to decide that the parameter
    //          pack is empty.
    using PyTreeIterMCTS = PyAlg<MCTS<PythonNode<MCTSStats>, true, false, false>>;
    py::class_<PyTreeIterMCTS>(m, "tree_iters_MCTS")
        .def(py::init<py::object>())
        .def("move", &PyTreeIterMCTS::move<uint, uint, double>, "", py::kw_only(),
            py::arg("rollout_depth"), py::arg("iters"), py::arg("exploration_weight") = sqrt(2));
}
