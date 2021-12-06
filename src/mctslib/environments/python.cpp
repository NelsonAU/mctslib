#include <iostream>
#include <limits>
#include <stdexcept>
#include <memory>
#include <set>
#include <random>

#include <pybind11/stl.h>

#include "util/nodestats.cpp"
#include "algorithm/mcts.cpp"
#include "algorithm/hrave.cpp"

namespace mctslib {

namespace py = pybind11;

template<class NodeStats>
class PyNode {
public:
    static inline std::mt19937 rng;
	py::object object;
	NodeStats stats;
	std::vector<PyNode*> children;
	bool been_expanded = false;


	PyNode(py::object object) : object(object), 
		stats(object.attr("evaluation").cast<double>(), 
				object.attr("action").cast<decltype (stats.action)>()) {}

	PyNode random_child() const {
        if (been_expanded) {
            std::uniform_int_distribution<size_t> dist {0, children.size() - 1};
            size_t idx = dist(rng);
            return *children[idx];
        }
		return PyNode(object.attr("random_child")());
	}

	void create_children() {
		py::list list = object.attr("find_children")();
		size_t length = py::len(list);

		children.reserve(length);

		for (py::handle child : list) 
			children.push_back(new PyNode(py::reinterpret_borrow<py::object>(child)));

		return;
	}

    bool is_terminal() const {
        return object.attr("is_terminal")().cast<bool>();
    }

	friend bool operator< (const PyNode lhs, const PyNode rhs) {
		return lhs.object < rhs.object;
	}

	friend bool operator== (const PyNode lhs, const PyNode rhs) {
		return lhs.object.equal(rhs.object);
	}

};

template<template<class K, class V, typename...> class MapTemplate>
class PyMCTS {
public:

	MCTS<PyNode, MCTSStats, uint, MapTemplate> mcts;

	using PyNodeCls = PyNode<MCTSStats<uint>>;
	
	PyMCTS (py::object root) : mcts(new PyNodeCls(root)) {}

	py::object pyMove(int rollout_depth, double exploration_weight, uint iters, 
			double cpu_time, bool invert_reward) {

		if (!cpu_time && !iters) {
			throw std::invalid_argument("Must supply either cpu_time or iters");
		}

		MCTSSettings settings {exploration_weight, rollout_depth, iters, cpu_time, invert_reward};
		
		return mcts.move(settings).object;
	}
};


template< 
	template<class K, class V, typename...> class MapTemplate,
	template<class T, typename...> class AMAFContainerTemplate
>
class PyHRAVE {
	public:
	HRAVE<PyNode, MCTSStats, uint, MapTemplate, AMAFContainerTemplate> hrave;

	PyHRAVE (py::object root, int k, size_t action_space_size) 
		: hrave(new PyNode<MCTSStats<uint>>(root), k, action_space_size) {}

	py::object pyMove(int rollout_depth, double exploration_weight, uint iters, 
			double cpu_time, bool invert_reward) {

		if (!cpu_time && !iters) {
			throw std::invalid_argument("Must supply either cpu_time or iters");
		}

		MCTSSettings settings {exploration_weight, rollout_depth, iters, cpu_time, invert_reward};
		
		return hrave.move(settings).object;
	}
};
}

namespace std {
	template<typename NodeStats> 
	struct hash<mctslib::PyNode<NodeStats>> {
		std::size_t operator()(mctslib::PyNode<NodeStats> const& node) const {
			return (std::size_t) pybind11::hash(node.object);
		}
	};
}

