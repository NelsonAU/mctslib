#include <pybind11/pybind11.h>
#include "environments/python.cpp"
#include "algorithm/mcts.cpp"

namespace py = pybind11;

PYBIND11_MODULE(mctslib, m) {
	m.doc() = "pybind11 example plugin";
	
	py::class_<PyMCTS>(m, "MCTS")
		.def(py::init<>())
		.def("move", &PyMCTS::pyMove, "Uses the given arguments to find the best move",
			py::arg("node"), py::kw_only(), 
			py::arg("exploration_weight"), py::arg("rollout_depth"), py::arg("iters"), 
			py::arg("cpu_time"), py::arg("invert_reward"));
}
