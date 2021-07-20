
#include <pybind11/pybind11.h>
#include "example.cpp"

PYBIND11_MODULE(example, m) {
	m.doc() = "pybind11 example plugin";
	
	py::class_<PyMCTS>(m, "MCTS")
		.def(py::init<double, double, int, int>())
		.def("play", &PyMCTS::play);
}
