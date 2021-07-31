#include <iostream>
#include <limits>
#include <stdexcept>
#include <pybind11/stl.h>
#include "util/nodeclass.cpp"
#include "algorithm/mcts.cpp"


namespace py = pybind11;

class PyNode {
public:
	py::object object;
	NodeStats* stats;

	PyNode() {}

	PyNode(py::object _object) {
		object = _object;
		stats = new NodeStats(object.attr("evaluation").cast<double>());
	}

	bool is_terminal() {
		return object.attr("is_terminal")().cast<bool>();
	}

	PyNode random_child() {
		return PyNode(object.attr("random_child")());
	}

	std::vector<PyNode> find_children() {
		//possible to make automatic conversion from py::list to std::vector<py::object> but
		//this still copies, and we have to convert to Node Class in either case
		py::list list = object.attr("find_children")();
		size_t length = py::len(list);
		std::vector<PyNode> children;
		children.reserve(length);

		for (py::handle child : list) {
			children.push_back(PyNode(py::reinterpret_borrow<py::object>(child)));
		}
		return children;
	}

	friend bool operator< (const PyNode lhs, const PyNode rhs) {
		return lhs.object < rhs.object;
	}
};



class PyMCTS {
public:
	MCTS<PyNode> mcts;

	PyMCTS (py::object root) : mcts(root) {
	}

	py::object pyMove(double exploration_weight, int rollout_depth, int iters, 
			double cpu_time, bool invert_reward) {

		if (!cpu_time && !iters) {
			throw std::invalid_argument("Must supply either cpu_time or iters");
		}

		MCTSSettings settings {exploration_weight, rollout_depth, iters, cpu_time, invert_reward};
		
		return mcts.move(settings).object;
	}
};

