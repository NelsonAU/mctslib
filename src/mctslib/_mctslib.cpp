#include <pybind11/pybind11.h>

#include "mctslib/algorithm/mcts.h"
#include "mctslib/algorithm/pyalg.h"
#include "mctslib/body/body.h"
#include "mctslib/move/move.h"
#include "mctslib/node/python_node.h"

using namespace mctslib;
namespace py = pybind11;

PYBIND11_MODULE(_mctslib, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    using IterMCTS = MCTS<MCTSBody, PythonNode, decltype(mcts_iter_move)>;
    using CPUMCTS = MCTS<MCTSBody, PythonNode, decltype(mcts_cpu_move)>;
    
    using PyMCTS = PyAlg<IterMCTS>;
    //@NOTE:    Must specify template args to move that correspond to the settings of the algorithm.
    //          No way to infer this because the Settings default constructor cannot be deleted,
    //          and because of this the compiler will always choose to decide that the parameter
    //          pack is empty.
    //
    py::class_<PyMCTS>(m, "MCTS")
        .def(py::init<py::object>())
        .def("move", &PyMCTS::move<uint, uint, double, double>);

    

}
