#include <iostream>
#include <limits>
#include <pybind11/stl.h> //allows automatic conversion of stl containers


/* 
class BaseNode {
	
	double evaluation; 
	virtual bool is_terminal(); 
	virtual BaseNode random_child(); 
	virtual std::vector<BaseNode> find_children(); 
}; */

namespace py = pybind11;

class NodeStats {
public:
	double evaluation;
	double backprop_value;
	int visits;

	NodeStats(double _evaluation) {
		visits = 1;
		evaluation = _evaluation;
		backprop_value = evaluation;
	}

	const double avg_reward () {
		if (!visits) return -std::numeric_limits<double>::max();
		return backprop_value/visits;
	}
};

class PyNode {
public:
	py::object object;
	NodeStats* stats;
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
