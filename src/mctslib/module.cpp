#include <cmath>
#include <pybind11/pybind11.h>
#include "environments/python.cpp"
#include "algorithm/mcts.cpp"

namespace py = pybind11;

template<typename A, typename T, typename... Types>
using VecWrapper = std::vector<T>;

PYBIND11_MODULE(mctslib, m) {
	m.doc() = "pybind11 example plugin";
	
	py::class_<PyMCTS>(m, "MCTS")
		.def(py::init<py::object>())
		.def("move", &PyMCTS::pyMove, "Uses the given arguments to find the best move",
			py::arg("exploration_weight") = sqrt(2), py::arg("rollout_depth"), 
			py::arg("iters") = 0, py::arg("cpu_time") = 0, py::arg("invert_reward") = false);

	using DensePyHRAVE = PyHRAVE<std::map, VecWrapper>;
	py::class_<DensePyHRAVE>(m, "DenseHRAVE")
		.def(py::init<py::object, int, size_t>())
		.def("move", &DensePyHRAVE::pyMove, "Uses the given arguments to find the best move",
			py::arg("exploration_weight") = sqrt(2), py::arg("rollout_depth"), 
			py::arg("iters") = 0, py::arg("cpu_time") = 0, py::arg("invert_reward") = false);

	using SparsePyHRAVE = PyHRAVE<std::map, std::map>;
	py::class_<SparsePyHRAVE>(m, "SparseHRAVE")
		.def(py::init<py::object, int, size_t>())
		.def("move", &SparsePyHRAVE::pyMove, "Uses the given arguments to find the best move",
			py::arg("exploration_weight") = sqrt(2), py::arg("rollout_depth"), 
			py::arg("iters") = 0, py::arg("cpu_time") = 0, py::arg("invert_reward") = false);


}
