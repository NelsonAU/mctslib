#include <cmath>
#include <pybind11/pybind11.h>
#include "environments/python.cpp"
#include "algorithm/mcts.cpp"
#include "util/vecwrapper.hpp"

namespace py = pybind11;


PYBIND11_MODULE(mctslib, m) {
	m.doc() = "pybind11 example plugin";
	
    using PyMCTS = mctslib::PyMCTS<std::map>;
	py::class_<PyMCTS>(m, "MCTS")
		.def(py::init<py::object>())
		.def("move", &PyMCTS::pyMove, "Uses the given arguments to find the best move",
			py::arg("rollout_depth"), py::arg("exploration_weight") = sqrt(2),
			py::arg("iters") = 0, py::arg("cpu_time") = 0, py::arg("invert_reward") = false);
    
    using HashablePyMCTS = mctslib::PyMCTS<std::unordered_map>;
	py::class_<HashablePyMCTS>(m, "HashableMCTS")
		.def(py::init<py::object>())
		.def("move", &HashablePyMCTS::pyMove, "Uses the given arguments to find the best move",
			py::arg("rollout_depth"), py::arg("exploration_weight") = sqrt(2),
			py::arg("iters") = 0, py::arg("cpu_time") = 0, py::arg("invert_reward") = false);

	using DensePyHRAVE = mctslib::PyHRAVE<std::map, VecWrapper>;
	py::class_<DensePyHRAVE>(m, "DenseHRAVE")
		.def(py::init<py::object, int, ssize_t>())
		.def("move", &DensePyHRAVE::pyMove, "Uses the given arguments to find the best move",
			py::arg("rollout_depth"), py::arg("exploration_weight") = sqrt(2),
			py::arg("iters") = 0, py::arg("cpu_time") = 0, py::arg("invert_reward") = false);

	using SparsePyHRAVE = mctslib::PyHRAVE<std::map, std::map>;
	py::class_<SparsePyHRAVE>(m, "SparseHRAVE")
		.def(py::init<py::object, int, ssize_t>())
		.def("move", &SparsePyHRAVE::pyMove, "Uses the given arguments to find the best move",
			py::arg("rollout_depth"), py::arg("exploration_weight") = sqrt(2),
			py::arg("iters") = 0, py::arg("cpu_time") = 0, py::arg("invert_reward") = false);

	using HashableDensePyHRAVE = mctslib::PyHRAVE<std::unordered_map, VecWrapper>;
	py::class_<HashableDensePyHRAVE>(m, "HashableDenseHRAVE")
		.def(py::init<py::object, int, ssize_t>())
		.def("move", &HashableDensePyHRAVE::pyMove, "Uses the given arguments to find the best move",
			py::arg("rollout_depth"), py::arg("exploration_weight") = sqrt(2),
			py::arg("iters") = 0, py::arg("cpu_time") = 0, py::arg("invert_reward") = false);

	using HashableSparsePyHRAVE = mctslib::PyHRAVE<std::unordered_map, std::map>;
	py::class_<HashableSparsePyHRAVE>(m, "HashableSparseHRAVE")
		.def(py::init<py::object, int, ssize_t>())
		.def("move", &HashableSparsePyHRAVE::pyMove, "Uses the given arguments to find the best move",
			py::arg("rollout_depth"), py::arg("exploration_weight") = sqrt(2),
			py::arg("iters") = 0, py::arg("cpu_time") = 0, py::arg("invert_reward") = false);

}
