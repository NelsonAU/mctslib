#include <pybind11/pybind11.h>
#include "py.cpp"
#include "mcts.cpp"

namespace py = pybind11;

PYBIND11_MODULE(mctslib, m) {
	m.doc() = "pybind11 example plugin";
	
	py::class_<PyMCTS>(m, "MCTS")
		.def(py::init<double, double, int, int>())
		.def("play", &PyMCTS::pyPlay);
}
